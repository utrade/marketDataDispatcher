/*
 * =====================================================================================
 *
 *       Filename:  subscriptionServMsg.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/02/2013 10:50:35 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Abhishek Anand (), abhishek.anand@utradesolutions.com
 *   Organization:  uTrade Solutions
 *
 * =====================================================================================
 */

#include "subscriptionServMsg.h"
#include <iostream>

namespace MarketData
{

  SubscriptionServMsg::SubscriptionServMsg(const char *buf)
  {
    int offset = 2;
    UNSIGNED_LONG tmpLong1 = 0, tmpLong2 = 0;
    UNSIGNED_SHORT tmpShort1 = 0, tmpShort2 = 0;

    DESERIALIZE_16(tmpShort1, tmpShort2, setSubscriptionReq(tmpShort2), buf, offset);
    DESERIALIZE_16(tmpShort1, tmpShort2, setUnlinkFifo(tmpShort2), buf, offset);
    
    DESERIALIZE_16(tmpShort1, tmpShort2, setSymbolCount(tmpShort2), buf, offset);
    
    int i;
    for(i=0; i<getSymbolCount(); ++i)
    {
      DESERIALIZE_64(tmpLong1, tmpLong2, setSymbolId(i ,tmpLong2), buf, offset);
      DESERIALIZE_16(tmpShort1, tmpShort2, setFeedType(i, tmpShort2), buf, offset);
      //std::cout << "Symbol id = " << tmpLong2 << std::endl;
    }

    //std::cout <<"buf ========== ";
    //for(int i=0; i<FD_LENGTH; ++i)
    //  std::cout << buf[i];
    //setFdName(strdup(buf));
    
    memcpy( _fdName, buf+offset, FD_LENGTH);

    DESERIALIZE_16(tmpShort1, tmpShort2, setIsRunningLocally(tmpShort2), buf, offset);
    DESERIALIZE_16(tmpShort1, tmpShort2, setIsConsolidatedFeed(tmpShort2), buf, offset);
  }

  int SubscriptionServMsg::serialize(char *buf)
  {
    int bytes = 0;
    UNSIGNED_LONG tmpLong = 0;
    UNSIGNED_SHORT tmpShort = 0;
    
    bytes = sizeof(UNSIGNED_SHORT);  // Leave 2 bytes for packet size

    SERIALIZE_16(tmpShort, isSubscriptionReq(), buf, bytes);
    SERIALIZE_16(tmpShort, isUnlinkFifo(), buf, bytes);

    SERIALIZE_16(tmpShort, getSymbolCount(), buf, bytes);

    
    for(int i=0; i<getSymbolCount(); ++i)
    {
      SERIALIZE_64(tmpLong, getSymbolId(i), buf, bytes);
      SERIALIZE_16(tmpShort, getFeedType(i), buf, bytes);
    }

    memcpy(buf + bytes, getFdName(), FD_LENGTH);
    bytes +=  FD_LENGTH;

    SERIALIZE_16(tmpShort, getIsRunningLocally(), buf, bytes);
    SERIALIZE_16(tmpShort, getIsConsolidatedFeed(), buf, bytes);

    UNSIGNED_SHORT dummyBytes = 0;
    // Put size as the first field after deducting 2 bytes reserved for size
    SERIALIZE_16( tmpShort, 
                  (UNSIGNED_SHORT)(bytes-sizeof(UNSIGNED_SHORT)), 
                  buf, 
                  dummyBytes
              );
    return bytes;

  }

  void SubscriptionServMsg::dump()
  {
    std::cout << "\nxxxxxxxxxx Subscription Serv Msg xxxxxxxxxx\n";
    std::cout << "\nisSubscriptionReq() " << (int) isSubscriptionReq();
    std::cout << "\nisUnlinkFifo()      " << (int) isUnlinkFifo();
    std::cout << "\ngetSymbolCount()    " <<  getSymbolCount()<<"\n";

    for(int i =0; i<getSymbolCount(); ++i)
      std::cout << _symbolId[i] <<":" <<(int) _feedType[i] << ", ";

    std::cout << "\ngetFdName()         " << getFdName();
    std::cout << "\ngetIsRunningLocally " << getIsRunningLocally();
    std::cout << "\ngetIsConsolidatedFeed " << getIsConsolidatedFeed();
    std::cout << "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;

  }
}
