    /*
    * =====================================================================================
    *
    *       Filename:  subscriptionServer.cpp
    *
    *    Description:  :
    *
    *
    *        Version:  1.0
    *        Created:  11/28/2013 01:26:55 PM
    *       Revision:  none
    *       Compiler:  g++
    *
    *         Author:  Abhishek Anand (), abhishek.anand@utradesolutions.com
    *   Organization:  uTrade Solutions
    *
    * =====================================================================================
    */

    #include "subscriptionServer.h"
    #include <iostream>
    #include <cstdlib>
    #include <cassert>
    #include <unistd.h>
    #include "subscriptionServMsg.h"
    #include "symbolIdFdMap.h"

    #include <fcntl.h>

    namespace MarketData
    {
    SubscriptionServer::SubscriptionServer(int port):
        _port(port),
        _pendingBufferSize(0)
    {
        std::cout << "Subscription Server: Running on port ========>" << port << std::endl;
    }

    void SubscriptionServer::operator()()
    {
        evutil_socket_t listener;
        struct sockaddr_in sin;
        struct event_base *base;
        struct event *listenerEvent;

        base = event_base_new();
        if (!base) 
        {
            std::cerr << "[Error] Could not initialize libevent!" << std::endl;
            exit(1);
        }
        
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(_port);
        
        listener = socket(AF_INET, SOCK_STREAM, 0);
        evutil_make_socket_nonblocking(listener);

    #ifndef WIN32
        int one = 1;
        setsockopt( listener, 
                    SOL_SOCKET, 
                    SO_REUSEADDR, 
                    &one, 
                    sizeof(one)
            );
    #endif
        if ( bind( listener, 
                (struct sockaddr*)&sin, 
                sizeof(sin) ) < 0
        ) 
        {
            std::cerr << "[Error] Bind error on port : "<< _port << std::endl;
            return;
        } 
        
        if (listen(listener, 16)<0) 
        {
            std::cerr << "[Error] Listen error." << std::endl;
            return;
        }

        listenerEvent = event_new( base, 
                                listener, 
                                EV_READ|EV_PERSIST, 
                                invokeAcceptHandler,//acceptHandle, 
                                (void*)base
                        );

        event_add(listenerEvent, NULL);
        event_base_dispatch(base);
        
    } //operator


    void SubscriptionServer::invokeAcceptHandler( evutil_socket_t fd, 
                                                    short events, 
                                                    void *arg
                                            ) 
    {
        event_base *base = (event_base *)arg;
        (static_cast<SubscriptionServer*>(arg))->acceptHandle(fd, events, base);
    }



    void SubscriptionServer::acceptHandle( evutil_socket_t listener, 
                        short event, 
                        event_base * base
                    )
    {
        sockaddr_storage ss;
        socklen_t slen = sizeof(ss);
        int fd = accept(listener, (struct sockaddr*)&ss, &slen);
        if (fd < 0) 
        { 
            std::cerr << "[Error] Accept" << std::endl;
        } 
        else if (fd > FD_SETSIZE) 
        {
            close(fd); 
        } 
        else 
        {
            evutil_make_socket_nonblocking(fd);
            FdState *state = allocFdState(base, fd);
            assert(state);
            event_add(state->readEvent, NULL);
        }
    } //acceptHandle

    FdState* SubscriptionServer::allocFdState( struct event_base *base,
                                                evutil_socket_t   fd )
    {
        FdState *state = new  FdState ();
        if (!state)
            return NULL;
        state->readEvent = event_new( base, 
                                    fd, 
                                    EV_READ|EV_PERSIST, 
                                    invokeReadHandler, 
                                    state
                            );
        if (!state->readEvent) 
        {
            free(state);
            return NULL;
        } 

        state->buffer_used = 0;
        return state;
    }//allocFdState


    void SubscriptionServer::invokeReadHandler( evutil_socket_t fd, 
                                                short events, 
                                                void *arg
                                            ) 
    {
        FdState *state = (FdState *)arg;
        (static_cast<SubscriptionServer*>(arg))->readHandle(fd, events, state);
    }


    void SubscriptionServer::readHandle(  evutil_socket_t fd, 
                                            short events, 
                                            FdState *state )
    {
        char buf[MAXDATABUFFER];
        ssize_t result;
        result = recv(fd, buf, MAXDATABUFFER, 0);
        
        int initialSize =  result;
        if(_pendingBufferSize> 0)
        {
            initialSize += _pendingBufferSize;
            memcpy(_pendingBuffer+_pendingBufferSize, buf, result);
            memcpy(buf, _pendingBuffer, initialSize);
        }

        std::cout << "[Dispatcher] Received req od size " << result << std::endl;
        int dataFd = 0;
        int isLoacal = 0;

        if (result == 0) 
        {
            std::cout << "[Dispatcher] Error result = 0" << std::endl;
            SymbolIdFdMap::getInstance()->deleteAllSymbolOfFd(fd);
            freeFdState(state);
        }
        else if (result < 0) 
        {
            std::cout << "[Dispatcher] Error result < 0" << std::endl;
            if (errno == EAGAIN) 
                return;
            std::cerr << "[Error] recv" << std::endl;
        }
        else
        {
            int remainingSize = initialSize;  
            int currentPos = 0;
            std::cout << " ========= remainingSize " << remainingSize << std::endl;
            while(remainingSize >0)
            {
                UNSIGNED_SHORT tmpShort1 = 0, tmpShort2 = 0;
                int offset =0;
                int size = 0;
                DESERIALIZE_16(tmpShort1, tmpShort2, size = tmpShort2, buf, offset);
                std::cout << __LINE__ << " ========= Size " << size << std::endl;
                std::cout << __LINE__ << " ========= currentPos " << currentPos << std::endl;
                size +=2;
                if(size > remainingSize)
                {
                    _pendingBufferSize = remainingSize;
                    memcpy(_pendingBuffer, buf, remainingSize);
                    break;
                }
                if(_pendingBufferSize!= 0)
                {
                    memset(_pendingBuffer, 0, MAXDATABUFFER);
                    _pendingBufferSize = 0;
                }
                remainingSize -= size;
                std::cout << __LINE__ << " ========= remainingSize " << remainingSize << std::endl;
                std::cout << __LINE__ << " ========= Size " << size << std::endl;

                SubscriptionServMsg msg((std::string(buf, size)).c_str());
                msg.dump(); 
                
                currentPos += size;
                std::cout << __LINE__ << " ========= currentPos " << currentPos << std::endl;
                //buf = buf+currentPos;
                //char tempBuff[MAXDATABUFFER];
                //memcpy(buf, buf+currentPos-1, remainingSize);
                memcpy(buf, buf+currentPos, remainingSize);

                isLoacal = msg.getIsRunningLocally();
                
                if(isLoacal)
                {  
                    dataFd = open(msg.getFdName(), O_RDWR | O_NONBLOCK, 0);
                    std::cout << "Fd of "<< msg.getFdName() << " is "<< dataFd << std::endl;
                }

                if(msg.isSubscriptionReq() == 1)
                {
                    int count = msg.getSymbolCount();
                    for(int i=0; i<count; ++i)
                    {
                        if(isLoacal)
                        {   
                            if(msg.getIsConsolidatedFeed())
                                SymbolIdFdMap::getInstance()->addFdInConsolidatedMap(msg.getSymbolId(i),
                                                                                                                dataFd
                                    );
                             else
                                SymbolIdFdMap::getInstance()->addFd(msg.getSymbolId(i),
                                                                msg.getFeedType(i),
                                                                dataFd);
                        }    
                        else
                            SymbolIdFdMap::getInstance()->addFd(msg.getSymbolId(i),
                                                                msg.getFeedType(i),
                                                                fd);
                    }
                }
                else
                {
                    int count = msg.getSymbolCount();
                    for(int i=0; i<count; ++i)
                    {
                        if(isLoacal)
                            SymbolIdFdMap::getInstance()->deleteFd( msg.getSymbolId(i),
                                                                    msg.getFeedType(i),
                                                                    dataFd);
                        else
                            SymbolIdFdMap::getInstance()->deleteFd( msg.getSymbolId(i),
                                                                    msg.getFeedType(i),
                                                                    fd);
                    }
                    if(msg.isUnlinkFifo()&& msg.getIsRunningLocally())
                    {
                        close(dataFd);
                        unlink(msg.getFdName());
                    }
                }
            }
        }
        if(isLoacal)
        {
            close(fd);
            freeFdState(state);
            std::cout << "============== "<< __PRETTY_FUNCTION__ << "========="
                        <<std::endl;
        }
    } // readHandle

    void SubscriptionServer::freeFdState(FdState *state)
    {
        event_free(state->readEvent);
        delete state;
    }

}//MarketData
