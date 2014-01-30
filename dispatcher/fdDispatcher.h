#ifndef _FDDISPATCHER_H_
#define _FDDISPATCHER_H_

#include <boost/asio.hpp>
#include "threadPool.h"

#include <event2/event-config.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#else
#include <winsock2.h>
#include <windows.h>
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <event2/event.h>

#include <commands.h>

#include "mdStorage.h"

namespace MarketData
{
  /**
   * @anchor FdDispatcher 
   *
   * @brief FdDispatcher Class
   *
   * This class is responcible for writing event on
   * every subscribed symbols.
   */
  class FdDispatcher
  {
    public:
      /**
       * @brief Constructor
       *
       * @param num  number of dispatcher threads 
       */
      FdDispatcher(int num=5);
     

      /**
      * @brief Overloaded function operator.
      */
      void operator()();

      /**
      * @brief Function to create fifo if dispather and client both 
      * are on the same system.
      *
      * @param fifo name
      *
      * @return 0/-1 Success/Failure
      */
      int createFifo  (const char *fifo);
     

      /**
      * @brief Function to open fifo if dispather and client both 
      * are on the same system.
      *
      * @param fifo fifo name.
      *
      * @return 0/-1 Success/Failure
      */
      int openFifo    (const char *fifo);

      
      /**
      * @brief Internally used function to set whether to use TBT FO 
      * dispatcher or not. Value is updated from setting file.
      *
      * @param val True/False
      */
      static void setUseDispatcherFO(bool val){ _useDispatcherFO = val; }

     /**
      * @brief Internally used function to set whether to use TBT CM
      * dispatcher or not. Value is updated from setting file.
      *
      * @param val True/False
      */
      static void setUseDispatcherCM(bool val){ _useDispatcherCM = val; }
 
     /**
      * @brief Internally used function to set whether to use MCL 
      * dispatcher or not. Value is updated from setting file.
      *
      * @param val True/False
      */
      static void setMclUseDispatcher(bool val){ _mclUseDispatcher = val; }

     /**
      * @brief Internally used function to check whether to use TBT FO 
      * dispatcher or not. Value is updated from setting file.
      *
      * @param val True/False
      */
      static bool getUseDispatcherFO(){ return _useDispatcherFO; }
     
     /**
      * @brief Internally used function to check whether to use TBT CM
      * dispatcher or not. Value is updated from setting file.
      *
      * @param val True/False
      */
      static bool getUseDispatcherCM(){ return  _useDispatcherCM; }
     
     /**
      * @brief Internally used function to check whether to use MCL 
      * dispatcher or not. Value is updated from setting file.
      *
      * @param val True/False
      */
      static bool getMclUseDispatcher(){ return _mclUseDispatcher; }

      /**
      * @brief Destructor
      */
      ~FdDispatcher();

    private:

      /**
      * @brief Feed Dispatcher Function
      * Function Called whene there is an event for market data feed. 
      *
      * @param int Symbol Id
      * @param int Feed Type(TBT/Snapshot)
      */
      void dispatchFd ( int, int);

      /**
      * @brief Feed Dispatcher Function
      * Function Called whene there is an event for market data feed. 
      *
      * @param int Symbol Id
      * @param int Feed Type(TBT/Snapshot)
      * @param MDQuote reference
      */
      void dispatchFd ( int, int, CMD::MDQuote &);

      /**
      * @brief Feed Dispatcher Function
      * Function Called whene there is an event for market data feed. 
      *
      * @param int Symbol Id
      * @param MDQuote reference
      */
      void dispatchOnConsolidatedFd(int, CMD::MDQuote&);
      
      void getConsolidatedDepthBid(  std::map <int, int> &symbolIdDepthMap,
                                                            std::vector<CMD::MDQuote> &mdq,
                                                            CMD::MDQuoteConsolidated &mdqConsolidated);
      
      void getConsolidatedDepthAsk(  std::map <int, int> &symbolIdDepthMap,
                                                            std::vector<CMD::MDQuote> &mdq,
                                                            CMD::MDQuoteConsolidated &mdqConsolidated);
      
      /**
      * @brief Write event on fd 
      * Function responsible for writing the event with symbol id on
      * socket/fd.
      *
      * @param int from  
      * @param int to
      * @param int Symbol Id
      * @param int Feed Type
      * 
      */
      void writeOnFd  (int, int, int, int);

      /**
      * @brief Write event on fd 
      * Function responsible for writing the event with symbol id and 
      * snapshot on socket/fd.
      *
      * @param int from
      * @param int to
      * @param int Symbol Id
      * @param int Feed Type
      * @param MDQuote
      * 
      */
      void writeOnFd  (int, int, int, int, CMD::MDQuote&);

      /**
      * @brief Read Tbt Data From Fifo
      * Function is called to reed the feed when there is an event for
      * tbt feed.
      *
      * @param evutil_socket_t
      * @param short
      * @param 
      *
      * @return 
      */
      void fifoReadTbt              ( evutil_socket_t, short, void *);

      /**
      * @brief Invoke Fifo Read Tbt 
      * Since libevent is a C library, it doesn't 
      * directly provide a mechanism to associate 
      * an instance and an instance method, so 
      * we need to do it ourself.
      *
      * @param evutil_socket_t fd
      * @param short event
      * @param void pointer(struct event)
      */
      static void invokeFifoReadTbt ( evutil_socket_t, short, void *);

      
      /**
      * @brief Fifo Read Mcl
      * Function is called to reed the feed when there is an event for
      * mcl feed.
      * @param evutil_socket_t fd
      * @param short event
      * @param void pointer (struct event)
      */
      void fifoReadMcl              ( evutil_socket_t, short, void *);

      /**
      * @brief Invoke Fifo Read
      * Since libevent is a C library, it doesn't 
      * directly provide a mechanism to associate 
      * an instance and an instance method, so 
      * we need to do it ourself.
      *
      * @param evutil_socket_t fd
      * @param short event
      * @param void pointer(struct event)
      */
      static void invokeFifoReadMcl ( evutil_socket_t, short, void *);



      int           _numberOfDispatcherThread;  
      ThreadPool    _threads;     
      static bool   _useDispatcherFO;
      static bool   _useDispatcherCM;
      static bool   _mclUseDispatcher;
  };
} 
#endif
