#ifndef UT_THREAD_POOL_H
#define UT_THREAD_POOL_H

#include <boost/thread.hpp>
#include <boost/asio.hpp>

/*******************************************************************************
* To use this thread pool                                                                                                         *
* ThreadPool t( 5 )                                                                                                                 *
* t.post( boost::bind( &ClassName::MethodName, &Object ) )                                                 *
* i.e. Address of the method & reference to the object                                                                *
*********************************************************************************/

namespace MarketData
{
  class ThreadPool
  {
    public :
      ThreadPool( int noOfThreads = 1) ;
      ~ThreadPool() ;

      template< class func >
        void post( func f ) ;

      boost::asio::io_service &getIoService() ;

    private :
      boost::asio::io_service _ioService; 
      boost::asio::io_service::work _work ;
      boost::thread_group _threads;
  };

  inline ThreadPool::ThreadPool( int noOfThreads ) 
    : _work( _ioService )
  {
    for(int i = 0; i < noOfThreads ; ++i) 
      _threads.create_thread(boost::bind(&boost::asio::io_service::run, &_ioService));
  }

  inline ThreadPool::~ThreadPool() 
  {
    _ioService.stop() ;
    _threads.join_all() ;
  }

  inline boost::asio::io_service &ThreadPool::getIoService() 
  {
    return _ioService ;
  }

  template< class func >
    void ThreadPool::post( func f ) 
    {
      _ioService.post( f ) ;
    }
}

#endif 
