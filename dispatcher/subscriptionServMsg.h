#ifndef _SUBSCRIPTION_SERVER_MSG_H_
#define _SUBSCRIPTION_SERVER_MSG_H_

#include <cstring>
#include <cstdlib>
#include <string>
#include <defines.h>
#include <map>

namespace MarketData
{
  /**
   * @anchor SubscriptionServMsg 
   * @class SubscriptionServMsg 
   * @brief This class is responsible for serialization
   * of request data to the server.
   */
  class SubscriptionServMsg
  {
      typedef std::map<UNSIGNED_LONG, UNSIGNED_LONG> SymbolIdMap;
      typedef std::map<UNSIGNED_LONG, UNSIGNED_LONG> FeedTypeMap;

      static const int FD_LENGTH = 100;
      static const int MAX_SYMBOL_COUNT = 10;

    public:
      SubscriptionServMsg() {}
      SubscriptionServMsg(const char *buf);
      /**
       * @brief Serialize data.
       *
       * @param buf 
       *
       * @return Size of serialized data
       */
      int serialize(char* buf);
      /**
       * @brief Dump data.
       */
      void dump();

      /**
       * @brief To check whether the request is to subscribe or unsusbscribe. 
       *
       * @return 1 to Subscribe 0 to Unsubscribe
       */
      UNSIGNED_CHARACTER isSubscriptionReq() const      { return _isSubscription; }
      /**
       * @brief To check whether the request is to subscribe or unsusbscribe. 
       *
       * @param val
       */
      void setSubscriptionReq(UNSIGNED_CHARACTER val)   { _isSubscription = val; }
 
      /**
       * @brief 
       *
       * @return 
       */
      UNSIGNED_CHARACTER isUnlinkFifo() const           { return _isUnlinkFifo; }
      void setUnlinkFifo(UNSIGNED_CHARACTER val)        { _isUnlinkFifo = val; }

      /**
       * @brief 
       *
       * @return 
       */
      UNSIGNED_SHORT  getSymbolCount() const            { return _symbolCount;  }       
      void setSymbolCount(UNSIGNED_SHORT val)           { _symbolCount = val;   }

      UNSIGNED_LONG getSymbolId(int subs)               { return _symbolId[subs];  }
      void setSymbolId(int subs, int val)               { _symbolId[subs] = val; }
    
      UNSIGNED_CHARACTER getFeedType(int subs)          { return _feedType[subs]; }
      void setFeedType(int subs, UNSIGNED_CHARACTER val)          
                                    { _feedType[subs] = val; }
      
      const char* getFdName() const                     { return _fdName; }
      void setFdName(const char* m)                     { strncpy(_fdName, m, FD_LENGTH); }
      
      UNSIGNED_SHORT getIsRunningLocally()                  { return _isRunningLocally; }
      void  setIsRunningLocally(UNSIGNED_SHORT val=1)           { _isRunningLocally = val; }
      
      UNSIGNED_SHORT getIsConsolidatedFeed()                           { return _isConsolidatedFeed; }
      void  setIsConsolidatedFeed(UNSIGNED_SHORT val=0)         { _isConsolidatedFeed = val; }


    private:
      /**
       * value 0 - unsusbscribe request
       * value 1 - susbscribe request
       */ 
      UNSIGNED_SHORT    _isSubscription;

      /**
       * value 0 - Leave
       * value 1 - Unlink
       */ 
      UNSIGNED_SHORT    _isUnlinkFifo;

      UNSIGNED_SHORT    _symbolCount;
      
      
      SymbolIdMap       _symbolId;
      FeedTypeMap       _feedType;

      char              _fdName[FD_LENGTH];
      
      /**
       * 
       * value 1 - Both are on same system(Default value)
       * value 0 - Differeent
       */
      UNSIGNED_SHORT    _isRunningLocally;
      /**
       * 
       * value 1 - Request Consolidated Data
       * value 0 - Normal Data
       */
      UNSIGNED_SHORT    _isConsolidatedFeed;
      
  };


}//namespace


#endif
