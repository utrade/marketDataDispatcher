/*
 * =====================================================================================
 *
 *       Filename:  mdStorage.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/17/2014 02:15:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Abhishek Anand (), abhishek.anand@utradesolutions.com
 *   Organization:  uTrade Solutions
 *
 * =====================================================================================
 */
#ifndef _MDSTORAGE_H_
#define _MDSTORAGE_H_

#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>
#include <symbolStaticData.h>
#include <commands.h>

namespace MarketData
{
    /**
   * @anchor MDStorage
   *
   * @brief MDStorage Class
   *
   * This class is responcible for storing the quote. 
   * This is useful for getting consolidated feed for multiple symbols.
   */
  
  class MDStorage
  {
      typedef boost::unordered_map< long,
                                    CMD::MDQuote* >
                                          SymbolQuoteMap;
    public:
      /**
      * @brief Function to save market quote for every symbol.
      *
      * @param symbol Id
      * @param MDQuote
      *
      */  
      void save( const long , const CMD::MDQuote* );
      
      /**
      * @brief Function to get market quote for given symbol.
      *
      * @param symbol Id
      * 
      * @return MDQuote
      */
      CMD::MDQuote get( const long symbolId );

      /**
       * @brief MDStorage is is a Singleton class, which will 
       * have only one instance. This instance can be accessed using
       * the getInstance method.
       *
       * @return MDStorage*
       */
      static MDStorage* getInstance()
      {
        static MDStorage instance;
        return &instance;
      }
      
      
      CMD::MDQuote* quoteToMDQuote( const long symbolId,
                                    const CMD::MDQuote* quote);
    private:
      MDStorage()
      { }
      MDStorage( const MDStorage& );

      static const int        MAX_ORDER_DEPTH = 5;
      SymbolQuoteMap   _symbolQuoteMap;
      int                           _depth;
  };

} //MarketData

#endif
