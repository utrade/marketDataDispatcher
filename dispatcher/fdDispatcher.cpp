/*
 * =====================================================================================
 *
 *       Filename:  fdDispatcher.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/04/2013 11:56:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Abhishek Anand (), abhishek.anand@utradesolutions.com
 *   Organization:  uTrade Solutions
 *
 * =====================================================================================
 */

#include <iostream>
#include "fdDispatcher.h"
#include "symbolIdFdMap.h"
#include <deque>


namespace MarketData
{

    bool FdDispatcher::_useDispatcherFO = true;
    bool FdDispatcher::_useDispatcherCM = true;
    bool FdDispatcher::_mclUseDispatcher = true;

    FdDispatcher::FdDispatcher(int num)
        : _numberOfDispatcherThread(num),
            _threads(num) 
    {
    }
  
    //int FdDi
    int FdDispatcher::openFifo(const char *fifo)
    {
        int socket;
        socket = open(fifo, O_RDONLY | O_NONBLOCK, 0);
        if (socket == -1) 
        {
            perror("open");
            exit(1);
        }
        return socket;
    }


    int FdDispatcher::createFifo(const char *fifo)
    {
        struct stat st;
        int socket;

        if (lstat(fifo, &st) == 0) 
        {
            if ((st.st_mode & S_IFMT) == S_IFREG) 
            {
                errno = EEXIST;
                perror("lstat");
                exit(1);
            }
        }

        unlink(fifo);

        if (mkfifo(fifo, 0666) == -1) 
        {
            perror("mkfifo");
            exit(1);
        }

        socket = open(fifo, O_RDONLY | O_NONBLOCK, 0);
        if (socket == -1) 
        {
            perror("open");
            exit(1);
        }
        return socket;
    }

    void FdDispatcher::operator()()
    {

        struct event *evfifoFo, *evfifoCm, *evfifoMcl;
        struct event_base* base;

        const char *fifoFo = "pipes/fo.event.fifo";
        const char *fifoCm = "pipes/cm.event.fifo";
        const char *fifoMcl = "pipes/mcl.event.fifo";

        int socketFo, socketCm, socketMcl;

        if(_useDispatcherFO)  
            socketFo = openFifo(fifoFo);

        if(_useDispatcherCM)
            socketCm = openFifo(fifoCm);

        if(_mclUseDispatcher)
            socketMcl = createFifo(fifoMcl);

        /* Initalize the event library */
        base = event_base_new();

        /* Initalize one event */
        if(_useDispatcherFO)
        {
            evfifoFo = event_new( base, 
                                socketFo, 
                                EV_READ|EV_PERSIST,
                                invokeFifoReadTbt,
                                this);//event_self_cbarg());

            event_add(evfifoFo, NULL);
        }
        if(_useDispatcherCM)
        {  
            evfifoCm = event_new( base, 
                                socketCm, 
                                EV_READ|EV_PERSIST, 
                                invokeFifoReadTbt,
                                this);//event_self_cbarg());
            event_add(evfifoCm, NULL);
        }
        if(_mclUseDispatcher)
        {
            evfifoMcl = event_new(  base, 
                                    socketMcl, 
                                    EV_READ|EV_PERSIST, 
                                    invokeFifoReadMcl,
                                    this);//event_self_cbarg());
            event_add(evfifoMcl, NULL);
        }

        event_base_dispatch(base);
        event_base_free(base);
        #ifdef _WIN32
        CloseHandle(socket);
        #else
        close(socketFo);
        close(socketCm);
        unlink(fifoCm);
        unlink(fifoFo);
        #endif
        libevent_global_shutdown();
    }

    void FdDispatcher::dispatchFd(int symbolId, int feedType)
    {
        std::deque<std::pair<int, int> > deq;
        try
        {
            deq = MarketData::SymbolIdFdMap::getInstance()->getFdForSymbol(symbolId);
            int num = deq.size();
            for (int i = 0; i < _numberOfDispatcherThread; i++)
            {
                int from = (i * num)/_numberOfDispatcherThread;
                int to = ((i+1) * num)/_numberOfDispatcherThread;

                
                if(from != to)
                {
                    _threads.post( boost::bind(&FdDispatcher::writeOnFd,
                        this, 
                        from,
                        to,
                        symbolId,
                        feedType)
                    );
                }
            }
        }
        catch(std::exception &e)
        {
            std::cout <<"Exception: "<<e.what()<<std::endl;
        }
    }

    void FdDispatcher::writeOnFd(int from, int to, int symbolId, int feedType)
    {
        try
        {
            auto q = 
                    SymbolIdFdMap::getInstance()->getFdForSymbol(symbolId);
            auto it = q.begin();
            for(int i = from; i != to; ++i)
            {
                if(std::get<0>(*it) == feedType)
                {
                    write(((std::get<1>(*it))+i), &symbolId, sizeof(int));
                }
            }
        }
        catch(std::exception &e)
        {
            std::cout <<"Exception: "<<e.what() << std::endl;
        }
    }


    void FdDispatcher::dispatchFd(int symbolId, int feedType, CMD::MDQuote& mdq)
    { 
        std::deque<std::pair<int, int> > deq;
        try
        {
            deq = MarketData::SymbolIdFdMap::getInstance()->getFdForSymbol(symbolId);
            std::cout << "============ Afere deq ==========="<<std::endl;
            int num = deq.size();
            for (int i = 0; i < _numberOfDispatcherThread; i++)
            {
                int from = (i * num)/_numberOfDispatcherThread;
                int to = ((i+1) * num)/_numberOfDispatcherThread;

                
                if(from != to)
                {
                    _threads.post( boost::bind(&FdDispatcher::writeOnFd,
                            this, 
                            from,
                            to,
                            symbolId,
                            feedType,
                            std::ref(mdq))
                    );
                }
            }
        }
        catch(std::exception &e)
        {
            std::cout <<"Exception: "<<e.what()<<std::endl;
        }
        catch(...)
        {
            std::cout << "Unhandeled Exception caught" <<std::endl;
        }
    }

    
    void FdDispatcher::getConsolidatedDepthBid(  std::map< int, int >& symbolIdDepthMap, 
                                                                                 std::vector<CMD::MDQuote> &mdq,   
                                                                                 CMD::MDQuoteConsolidated& mdqConsolidated)
    {
        for(int i=0; i<5; ++i)
        {
            int max = -1;
            for(int j=0; j<symbolIdDepthMap.size(); ++j)
            {
                if(mdq[j].getBidPrice()[symbolIdDepthMap[mdq[j].getSymbolId()]] > max)
                    max = mdq[j].getBidPrice()[symbolIdDepthMap[mdq[j].getSymbolId()]];
            }    
            for(int j=0; j<symbolIdDepthMap.size(); ++j)
            {
                if(mdq[j].getBidPrice()[symbolIdDepthMap[mdq[j].getSymbolId()]] == max)
                {
                    mdqConsolidated.setBidPrice(i, max);
                    mdqConsolidated.setBidQty(i, mdq[j].getBidQty()[symbolIdDepthMap[mdq[j].getSymbolId()]]);
                    symbolIdDepthMap[mdq[j].getSymbolId()]++;
                }
                else
                {
                    mdqConsolidated.setBidPrice(i, 0);
                    mdqConsolidated.setBidQty(i, 0);
                }
            }
        }
    }


    void FdDispatcher::getConsolidatedDepthAsk(  std::map< int, int >& symbolIdDepthMap, 
                                                                                 std::vector<CMD::MDQuote> &mdq,
                                                                                 CMD::MDQuoteConsolidated& mdqConsolidated)
    {
        for(int i=0; i<5; ++i)
        {
            long min = LONG_MAX;
            for(int j=0; j<symbolIdDepthMap.size(); ++j)
            {
                if(mdq[j].getAskPrice()[symbolIdDepthMap[mdq[j].getSymbolId()]] != 0 &&
                    mdq[j].getAskPrice()[symbolIdDepthMap[mdq[j].getSymbolId()]] < min )
                    min = mdq[j].getAskPrice()[symbolIdDepthMap[mdq[j].getSymbolId()]];
            }    
            for(int j=0; j<symbolIdDepthMap.size(); ++j)
            {
                if(mdq[j].getAskPrice()[symbolIdDepthMap[mdq[j].getSymbolId()]] == min)
                {
                    mdqConsolidated.setAskPrice(i, min);
                    mdqConsolidated.setAskQty(i, mdq[j].getBidQty()[symbolIdDepthMap[mdq[j].getSymbolId()]]);
                    symbolIdDepthMap[mdq[j].getSymbolId()]++;
                }
                else
                {
                    mdqConsolidated.setAskPrice(i, 0);
                    mdqConsolidated.setAskQty(i, 0);
                }
            }
        }        
    }

    
    void FdDispatcher::dispatchOnConsolidatedFd(int symbolId, CMD::MDQuote& mdq)
    { 
        std::deque<int> deq;
        try
        {
            deq = MarketData::SymbolIdFdMap::getInstance()->getFdForSymbolConsolidated(symbolId);
            
            for(auto it=deq.begin(); it!=deq.end(); ++it)
            {
                try{
                    auto symbolIdQue = MarketData::SymbolIdFdMap::getInstance()->getSymbolForFdConsolidated(*it);
                    std::map <int, int> symbolIdDepthMap;
                    CMD::MDQuoteConsolidated mdqConsolidated;
                    mdqConsolidated.setNumberOfSymbols(symbolIdQue.size());
                    int index=0;
                    std::vector<CMD::MDQuote> mdq(symbolIdQue.size());
                    for(  auto queItr=symbolIdQue.begin() ; queItr!=symbolIdQue.end(); ++queItr, ++index)
                    {
                        symbolIdDepthMap[*queItr]=0;
                        mdq[index] = MarketData::MDStorage::getInstance()->get(*queItr);
                        mdqConsolidated.setSymbolId(index, *queItr);
                        mdqConsolidated.setLastTradePrice(index, mdq[index].getLastTradePrice());
                        mdqConsolidated.setLastTradeQty(index, mdq[index].getLastTradeQty());
                        mdqConsolidated.setOpenPrice(index, mdq[index].getOpenPrice());
                        mdqConsolidated.setClosePrice(index, mdq[index].getClosePrice());
                        mdqConsolidated.setHighPrice(index, mdq[index].getHighPrice());
                        mdqConsolidated.setLowPrice(index, mdq[index].getLowPrice());
                        mdqConsolidated.setDepth(5);
                    }
                    getConsolidatedDepthBid(symbolIdDepthMap, mdq, mdqConsolidated);
                    getConsolidatedDepthAsk(symbolIdDepthMap, mdq, mdqConsolidated);
                    
                    char buf[1024];
                    int size = mdqConsolidated.serialize(buf);
                    
                    write(*it, buf, size);
                    
                }
                catch(...)
                {
                    std::cout << "Fd is not in Map!" << std::endl;
                }
            }   

        }
        catch(std::exception &e)
        {
            std::cout <<"Exception: "<<e.what()<<std::endl;
        }
        catch(...)
        {
            std::cout << "Unhandeled Exception caught" <<std::endl;
        }
    }

    
    
    
    void FdDispatcher::writeOnFd(int from, int to, int symbolId, int feedType, CMD::MDQuote& mdq)
    {
        try
        {
            auto q = 
            SymbolIdFdMap::getInstance()->getFdForSymbol(symbolId);
            auto it = q.begin();
            char buf[1024];
            std::cout << "[Dispatcher]  Writing on Fd" << std::endl;
            std::cout << "[Dispatcher]  from : to = " << from <<" : "<< to << std::endl;

            int size = mdq.serialize(buf);

            for(int i = from; i != to; ++i)
            {
                if(std::get<0>(*it) == feedType)
                {
                    std::cout << "Fd is " << ((std::get<1>(*it))+i) << std::endl;
                    write(((std::get<1>(*it))+i), buf, size);
                }
            }
        }
        catch(std::exception &e)
        {
            std::cout <<"Exception: "<<e.what() << std::endl;
        }
    }


    void FdDispatcher::invokeFifoReadTbt( evutil_socket_t fd, 
                                        short events, 
                                        void *arg ) 
    {
        event_base *base = (event_base *)arg;
        (static_cast<FdDispatcher*>(arg))->fifoReadTbt(fd, events, base);
    }


    void FdDispatcher::fifoReadTbt(evutil_socket_t fd,
                                short event,
                                void *arg )
    {
        int symbolId;
        int len;
        struct event *ev = (struct event *)arg;

        len = read(fd, &symbolId, sizeof(int));
            
        //fprintf(stderr, "fifo_read called with fd: %d, 
        //                   event: %d, arg: %p,  symbolId: %d\n", 
        //                   (int)fd, event, arg, symbolId);

        if (len <= 0)
        {
            if (len == -1)
                perror("read");
            else if (len == 0)
                fprintf(stderr, "Connection closed\n");
            event_del(ev);
            event_base_loopbreak(event_get_base(ev));
            return;
        }

        dispatchFd(symbolId, TBT);
    }


    void FdDispatcher::invokeFifoReadMcl(evutil_socket_t fd, 
                                    short events, 
                                    void *arg ) 
    {
        event_base *base = (event_base *)arg;
        (static_cast<FdDispatcher*>(arg))->fifoReadMcl(fd, events, base);
    }


    void FdDispatcher::fifoReadMcl(evutil_socket_t fd,
                                short event,
                                void *arg )
    {
        int symbolId;
        int len;
        //struct event *ev = (struct event *)arg;

        char buff[1024];
        std::cout <<"[Disptacher] MDQuote size = "<<sizeof(CMD::MDQuote) << std::endl;
        len = read(fd, buff, 516);//sizeof(CMD::MDQuote));
        CMD::MDQuote *mdq = new CMD::MDQuote(buff+3);
        symbolId = mdq->getSymbolId();
        mdq->dump();
        MDStorage::getInstance()->save(symbolId, mdq);
        //len = read(fd, &symbolId, sizeof(int));

        //fprintf(stderr, "fifo_read called with fd: %d, event: %d, 
        //                  arg: %p,  symbolId: %d\n", (int)fd, 
        //                  event, arg, symbolId);

        if (len <= 0)
        {
            if (len == -1)
            perror("read");
            else if (len == 0)
            fprintf(stderr, "HFT-MCL: Connection closed\n");
            //event_del(ev);
            //event_base_loopbreak(event_get_base(ev));
            //event_base_free(event_get_base(ev));

            exit(1);
            return;
        }
        else
        {  
            std::cout << "==== Address of MDQ " << mdq << std::endl;
            dispatchFd(symbolId, MCL, *mdq);
            dispatchOnConsolidatedFd(symbolId, *mdq);
        }
        delete mdq;
    }



    FdDispatcher::~FdDispatcher()
    {
    }
    
}
