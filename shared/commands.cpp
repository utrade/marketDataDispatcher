#include "commands.h"
#include <iostream>
#include "response.h"


namespace CMD {

/////////////////////////////////////////////////////////////////

ScripMasterDataRequest::ScripMasterDataRequest(const char * buf)
{
  int offset = 0;
  UNSIGNED_INTEGER tmpInt1 = 0, tmpInt2 = 0;
  UNSIGNED_CHARACTER tmpChar = 0;
  UNSIGNED_LONG tmpLong1 = 0, tmpLong2 = 0;
  UNSIGNED_SHORT tmpShort1 = 0, tmpShort2 = 0;
  
  DESERIALIZE_8(tmpChar, setScripMasterDataRequestType(tmpChar), buf, offset);
  DESERIALIZE_32(tmpInt1, tmpInt2, setClientId(tmpInt2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setRecordNumber(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setSecurityId(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setSymbolId(tmpLong2), buf, offset);
  DESERIALIZE_16(tmpShort1, tmpShort2, setExchangeId(tmpShort2), buf, offset);

  setSymbol(strdup(buf + offset));
  offset += SYMBOL_SIZE;
  setSeries(strdup(buf + offset));
  offset += SERIES_SIZE;
  setMarketName(strdup(buf + offset));
  offset += MARKET_NAME_SIZE;
  DESERIALIZE_8(tmpChar, setOptionType(tmpChar), buf, offset);
  DESERIALIZE_8(tmpChar, setOptionMode(tmpChar), buf, offset);
  DESERIALIZE_8(tmpChar, setSecurityType(tmpChar), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setStrikePrice(tmpLong2), buf, offset);
  DESERIALIZE_32(tmpInt1, tmpInt2, setExpiryYearMon(tmpInt2), buf, offset);
  DESERIALIZE_32(tmpInt1, tmpInt2, setExpiryDate(tmpInt2), buf, offset);
  DESERIALIZE_32(tmpInt1, tmpInt2, setNumberOfRecords(tmpInt2), buf, offset);
  setSymbolAlias(strdup(buf + offset));
  offset += SYMBOL_ALIAS_SIZE;

}

int ScripMasterDataRequest::serialize(char *buf)
{
  int bytes = 0;
    UNSIGNED_INTEGER tmp = 0;
    UNSIGNED_SHORT tmpShort = 0;
    UNSIGNED_CHARACTER tmpChar = 0;
    UNSIGNED_LONG tmpLong = 0;

    bytes = sizeof(UNSIGNED_SHORT);  // Leave 2 bytes for packet size
    // Put category of command
    SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(CMD::CommandCategory_SEND_SCRIP_MASTER_DATA), buf, bytes);  //Command Category

    // Put fields of this class
    SERIALIZE_8(tmpChar, getScripMasterDataRequestType(), buf, bytes);
    SERIALIZE_32(tmp, getClientId(), buf, bytes);
    SERIALIZE_64(tmpLong, getRecordNumber(), buf, bytes);
    SERIALIZE_64(tmpLong, getSecurityId(), buf, bytes);
    SERIALIZE_64(tmpLong, getSymbolId(), buf, bytes);
    SERIALIZE_16(tmpShort, getExchangeId(), buf, bytes);

    memcpy(buf + bytes, getSymbol(), SYMBOL_SIZE);
    bytes += SYMBOL_SIZE;
    memcpy(buf + bytes, getSeries(), SERIES_SIZE);
    bytes += SERIES_SIZE;
    memcpy(buf + bytes, getMarketName(), SERIES_SIZE);
    bytes += MARKET_NAME_SIZE;
    SERIALIZE_8(tmpChar, getOptionType(), buf, bytes);
    SERIALIZE_8(tmpChar, getOptionMode(), buf, bytes);
    SERIALIZE_8(tmpChar, getSecurityType(), buf, bytes);
    SERIALIZE_64(tmpLong, getStrikePrice(), buf, bytes);
    SERIALIZE_32(tmp, getExpiryYearMon(), buf, bytes);
    SERIALIZE_32(tmp, getExpiryDate(), buf, bytes);
    SERIALIZE_32(tmp, getNumberOfRecords(), buf, bytes);
    memcpy(buf + bytes, getSymbolAlias(), SYMBOL_ALIAS_SIZE);
    bytes += SYMBOL_ALIAS_SIZE;

    UNSIGNED_SHORT dummyBytes = 0;
    // Put size as the first field after deducting 2 bytes reserved for size
    SERIALIZE_16(tmpShort, (UNSIGNED_SHORT)(bytes-sizeof(UNSIGNED_SHORT)), buf, dummyBytes);

    return bytes;
}

void ScripMasterDataRequest::dump()
{

  std::cout << "ScripMasterDataRequest dump : " << std::endl;
  std::cout << getScripMasterDataRequestType() << std::endl; 
  std::cout << getClientId() << std::endl; 
  std::cout << getRecordNumber() << std::endl; 
  std::cout << getSecurityId() << std::endl; 
  std::cout << getSymbolId() << std::endl; 
  std::cout << getExchangeId() << std::endl; 
  std::cout << getSymbol() << std::endl; 
  std::cout << getSeries() << std::endl; 
  std::cout << getMarketName() << std::endl; 
  std::cout << getOptionType() << std::endl; 
  std::cout << getOptionMode() << std::endl; 
  std::cout << getSecurityType() << std::endl; 
  std::cout << getStrikePrice() << std::endl; 
  std::cout << getExpiryYearMon() << std::endl; 
  std::cout << getExpiryDate() << std::endl; 
  std::cout << getNumberOfRecords() << std::endl;
  std::cout << getSymbolAlias() << std::endl;
  std::cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n" << std::endl;
    
}


InstrumentRequest::InstrumentRequest(const char* buf) //deserializing
{
  setMnemonic(strdup(buf));
  std::cout << "Mnemonic : "<< buf <<std::endl;
}

int InstrumentRequest::serialize(char* buf)
{
  int bytes = 0;
  UNSIGNED_CHARACTER tmpChar = 0;
  UNSIGNED_SHORT tmpShort = 0;
  bytes = sizeof(UNSIGNED_SHORT);  // Leave 2 bytes for packet size
  // Put category of command
  SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(CMD::CommandCategory_INSTRUMENT_SEARCH_REQUEST), buf, bytes);  //Command Category
  
  memcpy(buf + bytes, getMnemonic(), MNEMONIC_LENGTH);
  bytes += MNEMONIC_LENGTH;

  UNSIGNED_SHORT dummyBytes = 0;
  // Put size as the first field after deducting 2 bytes reserved for size
  SERIALIZE_16(tmpShort, (UNSIGNED_SHORT)(bytes-sizeof(UNSIGNED_SHORT)), buf, dummyBytes);

  return bytes;
}

MDSubscribeRequest::MDSubscribeRequest(const char* buf) //deserializing
{
  int offset = 0;
  UNSIGNED_LONG tmpLong1 = 0, tmpLong2 = 0;
  UNSIGNED_SHORT tmpShort1 = 0, tmpShort2 = 0;

  DESERIALIZE_16(tmpShort1, tmpShort2, setSubscriptionReq(tmpShort2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setSymbolId(tmpLong2), buf, offset);
  //setLoginToken(strdup(buf));
  DESERIALIZE_16(tmpShort1, tmpShort2, setUserId(tmpShort2), buf, offset);
}

int MDSubscribeRequest::serialize(char* buf)
{
  int bytes = 0;
  UNSIGNED_CHARACTER tmpChar = 0;
  UNSIGNED_LONG tmpLong = 0;
  UNSIGNED_SHORT tmpShort = 0;

  bytes = sizeof(UNSIGNED_SHORT);  // Leave 2 bytes for packet size
  
  // Put category of command
  SERIALIZE_8(tmpChar, 
      (UNSIGNED_CHARACTER)(CMD::CommandCategory_MARKET_DATA_SUBSCRIPTION), 
      buf, 
      bytes);  //Command Category

  // Put fields of this class
  SERIALIZE_16(tmpShort, isSubscriptionReq(), buf, bytes);
  SERIALIZE_64(tmpLong, getSymbolId(), buf, bytes);
  //memcpy(buf + bytes, getLoginToken(), TOKEN_LENGTH);
  SERIALIZE_16(tmpShort, getUserId(), buf, bytes);
  
  UNSIGNED_SHORT dummyBytes = 0;
  // Put size as the first field after deducting 2 bytes reserved for size
  SERIALIZE_16(tmpShort, (UNSIGNED_SHORT)(bytes-sizeof(UNSIGNED_SHORT)), buf, dummyBytes);

  return bytes;
}


MDQuote::MDQuote()
{
  _symbolId = 0;
  _numberOfTrades = 0;
  _volume = 0;
  _value = 0;
  _lastTradePrice = 0;
  _lastTradeQty = 0;
  _openPrice = 0;
  _closePrice = 0;
  _highPrice = 0;
  _lowPrice = 0;
  _totalBidQty = 0;
  _totalAskQty = 0;
  _lowerCktLimit = 0;
  _upperCktLimit = 0;
  _depth = 0;
  memset(_bidPrice, 0, MAX_LOOKUP_LEVEL); 
  memset(_bidQty,   0, MAX_LOOKUP_LEVEL); 
  memset(_askPrice, 0, MAX_LOOKUP_LEVEL); 
  memset(_askQty,   0, MAX_LOOKUP_LEVEL); 
}

MDQuote::MDQuote(const char* buf) //deserializing
{
  int offset = 0;
  UNSIGNED_CHARACTER tmpChar = 0;
  UNSIGNED_LONG tmpLong1 = 0, tmpLong2 = 0;

  DESERIALIZE_64(tmpLong1, tmpLong2, setSymbolId(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setNummberOfTrades(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setVolume(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setValue(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setLastTradePrice(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setLastTradeQty(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setOpenPrice(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setClosePrice(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setHighPrice(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setLowPrice(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setTotalBidQty(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setTotalAskQty(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setLowerCktLimit(tmpLong2), buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, setUpperCktLimit(tmpLong2), buf, offset);
  DESERIALIZE_8(tmpChar, setDepth(tmpChar), buf, offset);
  memcpy( _bidPrice, buf+offset, QUOTEDATA_LENGTH);
  offset += QUOTEDATA_LENGTH;
  memcpy(_bidQty, buf+offset, QUOTEDATA_LENGTH);
  offset += QUOTEDATA_LENGTH;
  memcpy( _askPrice, buf+offset, QUOTEDATA_LENGTH);
  offset += QUOTEDATA_LENGTH;
  memcpy(_askQty, buf+offset, QUOTEDATA_LENGTH);
  offset += QUOTEDATA_LENGTH;
}

int MDQuote::serialize(char *buf)
{
  int bytes = 0;
  UNSIGNED_CHARACTER tmpChar = 0;
  UNSIGNED_LONG tmpLong = 0;
  UNSIGNED_SHORT tmpShort = 0;

  bytes = sizeof(UNSIGNED_SHORT);  // Leave 2 bytes for packet size
  SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(RSP::Responsecategory_MDQuote), buf, bytes);  //Response Category

  SERIALIZE_64(tmpLong, getSymbolId(), buf, bytes);
  SERIALIZE_64(tmpLong, getNumberOfTrades(), buf, bytes);
  SERIALIZE_64(tmpLong, getVolume(), buf, bytes);
  SERIALIZE_64(tmpLong, getValue(), buf, bytes);
  SERIALIZE_64(tmpLong, getLastTradePrice(), buf, bytes);
  SERIALIZE_64(tmpLong, getLastTradeQty(), buf, bytes);
  SERIALIZE_64(tmpLong, getOpenPrice(), buf, bytes);
  SERIALIZE_64(tmpLong, getClosePrice(), buf, bytes);
  SERIALIZE_64(tmpLong, getHighPrice(), buf, bytes);
  SERIALIZE_64(tmpLong, getLowPrice(), buf, bytes);
  SERIALIZE_64(tmpLong, getTotalBidQty(), buf, bytes);
  SERIALIZE_64(tmpLong, getTotalAskQty(), buf, bytes);
  SERIALIZE_64(tmpLong, getLowerCktLimit(), buf, bytes);
  SERIALIZE_64(tmpLong, getUpperCktLimit(), buf, bytes);
  SERIALIZE_8(tmpChar, getDepth(), buf, bytes);
  memcpy(buf+bytes, _bidPrice, QUOTEDATA_LENGTH);
  bytes += QUOTEDATA_LENGTH;
  memcpy(buf+bytes, _bidQty, QUOTEDATA_LENGTH);
  bytes += QUOTEDATA_LENGTH;
  memcpy(buf+bytes, _askPrice, QUOTEDATA_LENGTH);
  bytes += QUOTEDATA_LENGTH;
  memcpy(buf+bytes, _askQty, QUOTEDATA_LENGTH);
  bytes += QUOTEDATA_LENGTH;

  UNSIGNED_SHORT dummyBytes = 0;
  // Put size as the first field after deducting 2 bytes reserved for size
  SERIALIZE_16(tmpShort, (UNSIGNED_SHORT)(bytes-sizeof(UNSIGNED_SHORT)), buf, dummyBytes);
  
  return bytes;
}

void MDQuote::dumpDepth(UNSIGNED_LONG *data/*[MAX_LOOKUP_LEVEL]*/, UNSIGNED_CHARACTER depth)
{
  for( int i =0; i<depth ; ++i )
  {
    std::cout <<std::setw(10) << data[i] ; 
  }
}

void MDQuote::dump()
{
  std::cout << "\nxxxxxxxxxxxxxxxxxxx MDQuote xxxxxxxxxxxxxxxxxxxxxxxx\n" << std::endl;
  std::cout << "\ngetSymbolId()       " << getSymbolId();
  std::cout << "\ngetNumberOfTrades() " << getNumberOfTrades();
  std::cout << "\ngetVolume()         " << getVolume();
  std::cout << "\ngetValue()          " << getValue();
  std::cout << "\ngetLastTradePrice() " << getLastTradePrice();
  std::cout << "\ngetLastTradeQty()   " << getLastTradeQty();
  std::cout << "\ngetOpenPrice()      " << getOpenPrice();
  std::cout << "\ngetClosePrice()     " << getClosePrice();
  std::cout << "\ngetHighPrice()      " << getHighPrice();
  std::cout << "\ngetLowPrice()       " << getLowPrice();
  std::cout << "\ngetTotalBidQty()    " << getTotalBidQty();
  std::cout << "\ngetTotalAskQty()    " << getTotalAskQty();
  std::cout << "\ngetLowerCktLimit()  " << getLowerCktLimit();
  std::cout << "\ngetUpperCktLimit()  " << getUpperCktLimit();
  std::cout << "\ngetDepth()          " << (UNSIGNED_INTEGER) getDepth();
  std::cout << "\nBidPrice            " ; dumpDepth(_bidPrice, getDepth());
  std::cout << "\nBidQty              " ; dumpDepth(_bidQty, getDepth());
  std::cout << "\nAskPrice            " ; dumpDepth(_askPrice, getDepth());
  std::cout << "\nAskQty              " ; dumpDepth(_askQty, getDepth());
  std::cout << "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n" << std::endl;
}

    MDQuoteConsolidated::MDQuoteConsolidated()
    {
    _numSymbols = 0;
    _depth = 0;
    //memset(_bidPrice, 0, MAX_LOOKUP_LEVEL); 
    //memset(_bidQty,   0, MAX_LOOKUP_LEVEL); 
    //memset(_askPrice, 0, MAX_LOOKUP_LEVEL); 
    //memset(_askQty,   0, MAX_LOOKUP_LEVEL); 
    }

    MDQuoteConsolidated::MDQuoteConsolidated(const char* buf) //deserializing
    {
        int offset = 0;
        UNSIGNED_CHARACTER tmpChar = 0;
        UNSIGNED_LONG tmpLong1 = 0, tmpLong2 = 0;

        DESERIALIZE_64(tmpLong1, tmpLong2, setNumberOfSymbols(tmpLong2), buf, offset);
        for(int i=0; i<getNumberOfSymbols(); ++i)
        {
            DESERIALIZE_64(tmpLong1, tmpLong2, setSymbolId(i, tmpLong2), buf, offset);
            DESERIALIZE_64(tmpLong1, tmpLong2, setLastTradePrice(i, tmpLong2), buf, offset);
            DESERIALIZE_64(tmpLong1, tmpLong2, setLastTradeQty(i, tmpLong2), buf, offset);
            DESERIALIZE_64(tmpLong1, tmpLong2, setOpenPrice(i, tmpLong2), buf, offset);
            DESERIALIZE_64(tmpLong1, tmpLong2, setClosePrice(i, tmpLong2), buf, offset);
            DESERIALIZE_64(tmpLong1, tmpLong2, setHighPrice(i, tmpLong2), buf, offset);
            DESERIALIZE_64(tmpLong1, tmpLong2, setLowPrice(i, tmpLong2), buf, offset);
        }
        DESERIALIZE_8(tmpChar, setDepth(tmpChar), buf, offset);
        _bidPrice.resize(_numSymbols);
        _bidQty.resize(_numSymbols);
        _askPrice.resize(_numSymbols);
        _askQty.resize(_numSymbols);

        for(int i=0; i<_numSymbols; ++i)
        {
            _bidPrice[i].resize(_depth);
            _bidQty[i].resize(_depth);
            _askPrice[i].resize(_depth);
            _askQty[i].resize(_depth);
        }

        for(int i=0; i<_numSymbols; ++i)
        {
            for(int j=0; j<_depth; ++j)
            {
            DESERIALIZE_64(tmpLong1, tmpLong2, _bidPrice[i][j]=tmpLong2, buf, offset);
            DESERIALIZE_64(tmpLong1, tmpLong2, _bidQty[i][j]=tmpLong2, buf, offset);
            DESERIALIZE_64(tmpLong1, tmpLong2, _askPrice[i][j]=tmpLong2, buf, offset);
            DESERIALIZE_64(tmpLong1, tmpLong2, _askQty[i][j]=tmpLong2, buf, offset);
            }
        }
    }
    
    int MDQuoteConsolidated::serialize(char *buf)
    {
        int bytes = 0;
        UNSIGNED_CHARACTER tmpChar = 0;
        UNSIGNED_LONG tmpLong = 0;
        UNSIGNED_SHORT tmpShort = 0;

        bytes = sizeof(UNSIGNED_SHORT);  // Leave 2 bytes for packet size
        SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)
                                (RSP::Responsecategory_MDQuoteConsolidated),
                    buf,
                    bytes);  //Response Category

        SERIALIZE_64(tmpLong, getNumberOfSymbols(), buf, bytes);
        for(int i=0; i<getNumberOfSymbols(); ++i)
        {
            SERIALIZE_64(tmpLong, getSymbolId(i), buf, bytes);
            SERIALIZE_64(tmpLong, getLastTradePrice(i), buf, bytes);
            SERIALIZE_64(tmpLong, getLastTradeQty(i), buf, bytes);
            SERIALIZE_64(tmpLong, getOpenPrice(i), buf, bytes);
            SERIALIZE_64(tmpLong, getClosePrice(i), buf, bytes);
            SERIALIZE_64(tmpLong, getHighPrice(i), buf, bytes);
            SERIALIZE_64(tmpLong, getLowPrice(i), buf, bytes);
        }
        SERIALIZE_8(tmpChar, getDepth(), buf, bytes);

        for(int i=0; i<_numSymbols; ++i)
        {
            for(int j=0; j<_depth; ++j)
            {
            SERIALIZE_64(tmpLong, _bidPrice[i][j], buf, bytes);
            SERIALIZE_64(tmpLong, _bidQty[i][j], buf, bytes);
            SERIALIZE_64(tmpLong, _askPrice[i][j], buf, bytes);
            SERIALIZE_64(tmpLong, _askQty[i][j], buf, bytes);
            }
        }
        UNSIGNED_SHORT dummyBytes = 0;
        // Put size as the first field after deducting 2 bytes reserved for size
        SERIALIZE_16(tmpShort, (UNSIGNED_SHORT)(bytes-sizeof(UNSIGNED_SHORT)), buf, dummyBytes);

        return bytes;
    }

    void MDQuoteConsolidated::dumpDepth(std::vector<UNSIGNED_LONG>& data,
                                        UNSIGNED_CHARACTER depth)
    {
        for( int i =0; i<depth ; ++i )
        {
            std::cout <<std::setw(10) << data[i] ;
        }
    }
    
    void MDQuoteConsolidated::dump()
    {
        std::cout << "\nxxxxxxxxxxxxxxxxxxx MDQuote xxxxxxxxxxxxxxxxxxxxxxxx\n" << std::endl;
        std::cout << "\ngetNumberOfSymbols()       " << getNumberOfSymbols();
        for(int i=0; i<getNumberOfSymbols(); ++i)
        {
            std::cout << "\ngetSymbolId()         " << getSymbolId(i);
            std::cout << "\ngetLastTradePrice() " << getLastTradePrice(i);
            std::cout << "\ngetLastTradeQty()   " << getLastTradeQty(i);
            std::cout << "\ngetOpenPrice()        " << getOpenPrice(i);
            std::cout << "\ngetClosePrice()       " << getClosePrice(i);
            std::cout << "\ngetHighPrice()        " << getHighPrice(i);
            std::cout << "\ngetLowPrice()         " << getLowPrice(i);
            std::cout << "\ngetDepth()              " << (UNSIGNED_INTEGER) getDepth();
            std::cout << "\nBidPrice                  " ; dumpDepth(_bidPrice[i], getDepth());
            std::cout << "\nBidQty                    " ; dumpDepth(_bidQty[i], getDepth());
            std::cout << "\nAskPrice                  " ; dumpDepth(_askPrice[i], getDepth());
            std::cout << "\nAskQty                    " ; dumpDepth(_askQty[i], getDepth());
        }
        std::cout << "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n" << std::endl;
    }





} // namespace CMD

