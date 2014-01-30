/*==================================================================
  Subscription server:- It accepts request for symbol subscription 
  from the client and updates symbol-fds map. Which  is used by 
  disppatcher to send event for every tick update.
  =================================================================*/

#ifndef  _SUBSCRIPTION_SERVER_H_
#define _SUBSCRIPTION_SERVER_H_

#include <cstring>
#include <string>
#include <cerrno>
#include <cstdio>
#include <signal.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include <event2/event.h>

#define MAXLINE 1024
#define MAXDATABUFFER 16000

namespace MarketData
{

  /**
   *
   * @mainpage muTrade Market Data Disptacher documentation
   * 
   * @section Introduction
   * It is subscription based market data event provider. Developer need to
   * subsribe the symbol from our subscription server. Once symbol is subscribed
   * user will get the event as snapshot for every tick update of respective 
   * symbol.
   *
   * @b Code @b Flow
   * @n 1) Developer need to subscribe the symbol using @ref SubscriptionServMsg with
   *       TCP based connection.
   * @n 2) Once subscription is done appliaction will get event as @ref MDQuote on the
   *       same socket.
   * 
   *@n @n Currently market data feed server supports four types of event.
   * @n 1) Event as symbol Id for every snapshot update.
   * @n 2) Event as symbol Id for every TBT update.
   * @n 3) Event as top 5 snapshot market depth for every snapshot update.
   * @n 4) Snapshot of consolidated order depth of two or more symbols.
   * 
   * @n @n
   * Sanpshot for single symbol is available as @ref MDQuote where as consolidated 
   * feed is available as @ref MDQuoteConsolidated .
   * 
   */




  struct FdState 
  {
    char          buffer[MAXLINE];
    size_t        buffer_used;

    struct event  *readEvent;
  };

  /**
   * @anchor SubscriptionServer
   *
   * @class Subscription Server Class 
   *
   * @brief It accepts request for symbol subscription from the 
   * client and updates symbol-fds map. Which  is used by 
   * disppatcher to send event for every tick update.
   *
   */
  class SubscriptionServer
  {
    public:
        SubscriptionServer(int);

        /**
         * @brief overloaded function operator.
         */
        void operator()();
        

    private:
                
        /**
         * @brief Free allocated state memory.
         *
         * @param state
         */
        void freeFdState ( FdState *state);
        
        /**
         * @brief Invoke Accept Handler
         * Since libevent is a C library, it doesn't 
         * directly provide a mechanism to associate 
         * an instance and an instance method, so 
         * we need to do it ourself.
         *
         * @param fd  Socket fd
         * @param events Flags
         * @param *arg Pointer to event base  
         */
        static void invokeAcceptHandler( evutil_socket_t fd,
                                         short events,
                                         void *arg
                              );
        
        /**
         * @brief Accept Handle
         *
         * @param listener Socket fd
         * @param event Flags
         * @param *base Pointer to event base 
         */
        void acceptHandle ( evutil_socket_t listener, 
                            short event, 
                            event_base * base
                  );
        /**
         * @brief Invoke Read Handler
         * Since libevent is a C library, it doesn't 
         * directly provide a mechanism to associate 
         * an instance and an instance method, so 
         * we need to do it ourself.
         *
         * @param fd  Socket fd
         * @param events Flags
         * @param *arg Pointer to event base  
         */
        static void invokeReadHandler(  evutil_socket_t fd,
                                        short events,
                                        void *arg
                              );

        /**
         * @brief Read Handle
         *
         * @param fd  Socket fd
         * @param events Flags
         * @param *state 
         */
        void readHandle  ( evutil_socket_t fd, 
                           short events, 
                           FdState *state
                    );

        /**
         * @brief Allocate FdState
         *
         * @param base
         * @param fd
         *
         * @return FdState
         */
        FdState* allocFdState( struct event_base *base, 
                               evutil_socket_t   fd );

        int              _port; //Server listening port.
        char             _pendingBuffer[MAXDATABUFFER];
        int              _pendingBufferSize;

  };  //SubscriptionServer

} //MarketData

#endif
