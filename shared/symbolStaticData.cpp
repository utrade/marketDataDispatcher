
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/unordered_map.hpp>
#include <fstream>
#include <commands.h>
#include <response.h>
#include "symbolStaticData.h"
#include <boost/unordered_map.hpp>
#include <string>
#include <ctime>
#include <sstream>
#include <common.h>
#include <map>


// Declare the map as global so that can be filled from main thread
// And accesible from all strategies
boost::unordered_map <long, SG::SymbolStaticData*> gSymbolStaticDataStore;
std::map<long, SG::SymbolStaticData*> gOrderedSymbolStaticDataStore;

// global map to search static data for instruments
boost::unordered_map<std::string, SG::SymbolStaticData*> gApiSymbolStaticData;

void insertApiData(SG::SymbolStaticData& data)
{
  std::string key;

  if ( data.securityType == CMD::SecurityType_COMMON_STOCK )
  {
    key = data.marketId + " " + data.symbol + " " + data.groupName;
    gApiSymbolStaticData [ key ] = &data;
  }
  else if ( data.securityType == CMD::SecurityType_FUTURE )
  {
    key = data.marketId + " " + data.symbol + " " +
      boost::lexical_cast<std::string>( (data.maturityYearmon * 100)
                                        + data.maturityDay );
    gApiSymbolStaticData [ key ] = &data;
  }
  else if ( data.securityType == CMD::SecurityType_OPTION )
  {
    key = data.marketId + " " + data.symbol + " " +
//      boost::lexical_cast<std::string>( (data.maturityYearmon * 100)
//                                        + data.maturityDay ) + " " +
      boost::lexical_cast<std::string>( (data.maturityDay * 1000000 + 
                                          (data.maturityYearmon%100) * 10000)
                                        + data.maturityYearmon/100 ) + " " +
      boost::lexical_cast<std::string>(data.strikePrice) + " " +
      ( (data.optionMode == CMD::OptionMode_CALL) ? "C" : "P" );
      std::cout << "=== key "<< key << " data.maturityDay "
                << data.maturityDay << " data.maturityYearmon " 
                << data.maturityYearmon   <<std::endl;

    gApiSymbolStaticData [ key ] = &data;
  }

  // also allow security to be searchable by securityId
  key = data.marketId + " " + boost::lexical_cast<std::string>(data.securityId);
  gApiSymbolStaticData [ key ] = &data;
}


std::string sanitizeApiSymbol(const std::string& str)
{
  typedef boost::tokenizer< boost::char_separator<char> >
    tokenizer;

  boost::char_separator<char> sep(" ");
  tokenizer tokenFields(str, sep);

  std::vector<std::string> vecTokenFields;
  for (tokenizer::iterator iter = tokenFields.begin();
       iter != tokenFields.end();
       iter++ )
    vecTokenFields.push_back( *iter );

  if ( vecTokenFields.size() == 3 &&
       vecTokenFields.size() == 5 )
  {
    try
    {
      boost::lexical_cast<int>( vecTokenFields[2] );
      return str;
    }
    catch(std::exception& e)
    {
    }

    try
    {
      int day = boost::lexical_cast<int>(
        vecTokenFields[2].substr(0, 2)
        );
      std::string strMonth = vecTokenFields[2].substr(2, 3);

      int month = 0;
      if ( strMonth == "JAN" )
        month = 1;
      else if ( strMonth == "FEB" )
        month = 2;
      else if ( strMonth == "MAR" )
        month = 3;
      else if ( strMonth == "APR" )
        month = 4;
      else if ( strMonth == "MAY" )
        month = 5;
      else if ( strMonth == "JUN" )
        month = 6;
      else if ( strMonth == "JUL" )
        month = 7;
      else if ( strMonth == "AUG" )
        month = 8;
      else if ( strMonth == "SEP" )
        month = 9;
      else if ( strMonth == "OCT" )
        month = 10;
      else if ( strMonth == "NOV" )
        month = 11;
      else if ( strMonth == "DEC" )
        month = 12;

      int year = boost::lexical_cast<int>(
        vecTokenFields[2].substr(5, 4)
        );

      vecTokenFields[2] = boost::lexical_cast<std::string>(
        (((year * 100) + month) * 100) + day
        );

      std::string returnVal;
      for (std::vector<std::string>::iterator it = vecTokenFields.begin();
           it != vecTokenFields.end();
           it ++ )
        returnVal += (*it + " ");

      return returnVal.substr(0, returnVal.size() - 1);

    }
    catch(std::exception& e)
    {
      return str;
    }
  }

  return str;
}

int
initializeSymbolStaticDataStore()
{
  std::ifstream        data("config/scrip_master_data.txt");
  std::string          line;
  SG::SymbolStaticData *symbolData = 0;
  long                 symbolId = 0;
  std::string          cell;

  if (!data.is_open())
    return -1;
 
  while( std::getline(data,line) ) {
    symbolData = new SG::SymbolStaticData();
    std::stringstream  lineStream(line);

    std::string          tempMarketId;
    GETSTRING(tempMarketId)
    GETINT(symbolData->securityId)
    if(tempMarketId.empty())                           // i.e. marketId field is empty
      symbolId = INTNULL;
    else if (tempMarketId == "BSE")
      symbolId =  GET_SYMBOL_ID(symbolData->securityId, CMD::ExchangeId_BSE);
    else if (tempMarketId == "NSE")
      symbolId =  GET_SYMBOL_ID(symbolData->securityId, CMD::ExchangeId_NSE);
    else if (tempMarketId == "ESMNSE")
      symbolId =  GET_SYMBOL_ID(symbolData->securityId, CMD::ExchangeId_ESMNSE);
	  else if (tempMarketId == "SGX")
      symbolId =  GET_SYMBOL_ID(symbolData->securityId, CMD::ExchangeId_SGX);
	  else if (tempMarketId == "BSEETI")
      symbolId =  GET_SYMBOL_ID(symbolData->securityId, CMD::ExchangeId_BSEETI);
	  else if (tempMarketId == "NSECDS")
      symbolId =  GET_SYMBOL_ID(symbolData->securityId, CMD::ExchangeId_NSECDS);
    else if (tempMarketId == "CFH")
      symbolId =  GET_SYMBOL_ID(symbolData->securityId, CMD::ExchangeId_CFH);
    else
      symbolId = INTNULL;

    symbolData->marketId = tempMarketId;

    GETSTRING(symbolData->symbol)
    GETCHAR(symbolData->securityType)
    if(symbolData->securityType == 'C')
      symbolData->securityType = CMD::SecurityType_COMMON_STOCK;
    else if(symbolData->securityType == 'F')
      symbolData->securityType = CMD::SecurityType_FUTURE;
    else if(symbolData->securityType == 'O')
      symbolData->securityType = CMD::SecurityType_OPTION;
    else
      symbolData->securityType = CMD::SecurityType_MAX;

    GETSTRING(symbolData->scripName)
    GETSTRING(symbolData->groupName)
    //Set timeElapsed field

    long tempYearMon;
    struct tm time;
    time_t t_of_day;

    std::getline(lineStream,cell,',');
    if( ! (cell.empty()) ) {
      tempYearMon = atol(cell.c_str());
      symbolData->maturityYearmon = tempYearMon;
      time.tm_mon = (tempYearMon % 100) - 1;
      time.tm_year = (tempYearMon / 100) - 1900;
    }


    std::getline(lineStream,cell,',');
    if( ! (cell.empty()) ) {
      time.tm_mday = atol(cell.c_str());
      symbolData->maturityDay = atol(cell.c_str());
    }
    time.tm_hour = 14;     
    time.tm_min = 30;
    time.tm_sec = 00;
    time.tm_isdst = -1;   
    t_of_day = mktime(&time);                                //Seconds since the epoch(1970)


    long timezone_offset = 5.5 * 3600;                    // Timezone offset (India)
    long secondsForTenYears = 24 * 3600 * 365.2 * 10; 
    long timeElapsedBse = t_of_day + timezone_offset;
    long timeElapsedNse = timeElapsedBse - secondsForTenYears;

    if ( (symbolData->securityType == CMD::SecurityType_FUTURE ) || (symbolData->securityType == CMD::SecurityType_OPTION ) ) {
      symbolData->timeElapsedBse = timeElapsedBse;
      symbolData->timeElapsedNse = timeElapsedNse; 
    } else {
      symbolData->timeElapsedBse = symbolData->timeElapsedNse = 0;            // Applicable for future & options only
    }

    GETCHAR(symbolData->optionMode)
    if (symbolData->optionMode == 'P')
      symbolData->optionMode = CMD::OptionMode_PUT;
    else if (symbolData->optionMode == 'C')
        symbolData->optionMode = CMD::OptionMode_CALL;
    else symbolData->optionMode = CMD::OptionMode_MAX;

    GETCHAR(symbolData->optionAttribute)
    if (symbolData->optionAttribute == 'E')
      symbolData->optionAttribute = CMD::OptionAttribute_EUROPEAN;
    else if (symbolData->optionAttribute == 'A')
        symbolData->optionAttribute = CMD::OptionAttribute_AMERICAN;
    else symbolData->optionAttribute = CMD::OptionAttribute_MAX;

    GETCHAR(symbolData->instrumentType)
    if (symbolData->instrumentType == 'I')
      symbolData->instrumentType = CMD::InstrumentType_INDEX;
    else if (symbolData->instrumentType == 'S')
        symbolData->instrumentType = CMD::InstrumentType_STOCK;
    else symbolData->instrumentType = CMD::InstrumentType_MAX;

    GETINT(symbolData->strikePrice)
    GETCHAR(symbolData->noDelivery)
    GETINT(symbolData->marketLot)
    GETINT(symbolData->tickSize)
    GETCHAR(symbolData->suspended)
    GETCHAR(symbolData->avmFlag)
    GETINT(symbolData->avmBuyMargin)
    GETINT(symbolData->avmSellMargin)
    GETINT(symbolData->spanMargin)
    GETINT(symbolData->exposureMargin)

    char tempSettleMentType;
    //GETCHAR(symbolData->settlementType)
    GETCHAR(tempSettleMentType)

    GETSTRING(symbolData->isinNumber)
    GETINT(symbolData->haircut)
    GETINT(symbolData->bcastFlag)
    GETSTRING(symbolData->mktPicBcastFlag)

    GETSTRING(symbolData->productCode)

    long tempUnderlyingId = 0;
    GETINT(tempUnderlyingId)
    if(tempMarketId.empty())                           // i.e. marketId field is empty
      symbolData->underlyingId = INTNULL;
    else if (tempMarketId == "BSE")
      symbolData->underlyingId =  GET_SYMBOL_ID(tempUnderlyingId, CMD::ExchangeId_BSE);
    else if (tempMarketId == "NSE")
      symbolData->underlyingId =  GET_SYMBOL_ID(tempUnderlyingId, CMD::ExchangeId_NSE);
    else if (tempMarketId == "ESMNSE")
      symbolData->underlyingId =  GET_SYMBOL_ID(tempUnderlyingId, CMD::ExchangeId_ESMNSE);
	  else if (tempMarketId == "SGX")
      symbolData->underlyingId =  GET_SYMBOL_ID(tempUnderlyingId, CMD::ExchangeId_SGX);
	  else if (tempMarketId == "BSEETI")
      symbolData->underlyingId =  GET_SYMBOL_ID(tempUnderlyingId, CMD::ExchangeId_BSEETI);
	  else if (tempMarketId == "NSECDS")
      symbolData->underlyingId =  GET_SYMBOL_ID(tempUnderlyingId, CMD::ExchangeId_NSECDS);
	  else if (tempMarketId == "CFH")
      symbolData->underlyingId =  GET_SYMBOL_ID(tempUnderlyingId, CMD::ExchangeId_CFH);
    else 
      symbolData->underlyingId = INTNULL;
    
    GETINT(symbolData->is_auction_eligible)
    GETINT(symbolData->hi_52_wk)
    GETINT(symbolData->lo_52_wk)

    GETINT(symbolData->lowerBandPrice)
    GETINT(symbolData->upperBandPrice)
    GETINT(symbolData->freezeQuantity)
    GETINT(symbolData->closePrice)
    symbolData->symbolId = symbolId;
    if(symbolId != INTNULL) { 
      // i.e. security id is not zero
      // TBD:- ADD CHECK FOR ALREADY EXISTING
      gSymbolStaticDataStore[symbolId] = symbolData;
      gOrderedSymbolStaticDataStore[symbolId] = symbolData;

      // insert data also to the API data store
      insertApiData( *symbolData );
    } else {
      std::cout<<"Incorrect Symbol Id, Data not Inserted to Map" << std::endl;
    }
    GETINT(symbolData->scripPrecision)
  }
  /*
  for(std::map<long, SG::SymbolStaticData*>::iterator it = gOrderedSymbolStaticDataStore.begin(); it != gOrderedSymbolStaticDataStore.end(); ++it)
  {
    std::cout << "SymbolId stored in Ordered Map : " << it->first << std::endl;
  }
  */
  return 0;
}

SG::SymbolStaticData::SymbolStaticData() 
{ 
  marketId = "";
  securityId = 0;
  symbol = "";
  securityType = CMD::SecurityType_MAX;
  scripName = "";
  groupName = "";
  maturityYearmon = 0;
  maturityDay = 0;
  optionMode = CMD::OptionMode_MAX;
  optionAttribute = CMD::OptionAttribute_MAX;
  timeElapsedBse = 0;
  timeElapsedNse = 0;
  instrumentType = CMD::InstrumentType_MAX;
  strikePrice = 0;
  noDelivery = 0;
  marketLot = 0;
  tickSize = 0;
  suspended = 0;
  avmFlag = 0;
  avmBuyMargin = 0;
  avmSellMargin = 0;
  spanMargin = 0;
  exposureMargin = 0;
  settlementType = 0;
  isinNumber = "";
  haircut = 0;
  bcastFlag = 0;
  mktPicBcastFlag = "";
  productCode = "";
  underlyingId = 0;   
  is_auction_eligible = 0;
  hi_52_wk = 0;
  lo_52_wk = 0;
  lowerBandPrice = 0;
  upperBandPrice = 0;
  freezeQuantity = 0;
  closePrice = 0;
  symbolId = 0;
  scripPrecision = 0;
  symbolAlias = "";
}

int
getSymbolStaticData(long symbolId, SG::SymbolStaticData* symbolData)
{
// TBD:- ADD CHECK FOR NON-EXISTENT 
  SG::SymbolStaticData* foundData = gSymbolStaticDataStore[symbolId]; 
  if (foundData) {
    // Copy all the elements
    symbolData->marketId = foundData->marketId;
    symbolData->securityId = foundData->securityId;
    symbolData->symbol = foundData->symbol;
    symbolData->securityType = foundData->securityType;
    symbolData->scripName = foundData->scripName;
    symbolData->groupName = foundData->groupName;
    symbolData->timeElapsedBse = foundData->timeElapsedBse;
    symbolData->timeElapsedNse = foundData->timeElapsedNse;
    symbolData->optionMode = foundData->optionMode;
    symbolData->optionAttribute = foundData->optionAttribute;
    symbolData->instrumentType = foundData->instrumentType;
    symbolData->strikePrice = foundData->strikePrice;
    symbolData->noDelivery = foundData->noDelivery;
    symbolData->marketLot = foundData->marketLot;
    symbolData->tickSize = foundData->tickSize;
    symbolData->suspended = foundData->suspended;
    symbolData->avmFlag = foundData->avmFlag;
    symbolData->avmBuyMargin = foundData->avmBuyMargin;
    symbolData->avmSellMargin = foundData->avmSellMargin;
    symbolData->spanMargin = foundData->spanMargin;
    symbolData->exposureMargin = foundData->exposureMargin;
    //symbolData->settlementType = foundData->settlementType;
    symbolData->isinNumber = foundData->isinNumber;
    symbolData->haircut = foundData->haircut;
    symbolData->bcastFlag = foundData->bcastFlag;
    symbolData->mktPicBcastFlag = foundData->mktPicBcastFlag;
    symbolData->productCode = foundData->productCode;
    symbolData->underlyingId = foundData->underlyingId;
    symbolData->lowerBandPrice = foundData->lowerBandPrice;
    symbolData->upperBandPrice = foundData->upperBandPrice;
    symbolData->freezeQuantity = foundData->freezeQuantity;
    symbolData->closePrice = foundData->closePrice;
    symbolData->maturityDay = foundData->maturityDay;
    symbolData->maturityYearmon = foundData->maturityYearmon;
    symbolData->scripPrecision = foundData->scripPrecision;
    symbolData->symbolId = foundData->symbolId;
        
    return 0;
  } else {
    return -1;
  }
}

int getSymbolStaticData(const std::string& symbol,
                        SG::SymbolStaticData* symbolData)
{
  std::cout << "========== Searching for " << symbol << std::endl;

  SG::SymbolStaticData* foundData = gApiSymbolStaticData [ symbol ];
  if (foundData) {
    // Copy all the elements
    symbolData->marketId = foundData->marketId;
    symbolData->securityId = foundData->securityId;
    symbolData->symbol = foundData->symbol;
    symbolData->securityType = foundData->securityType;
    symbolData->scripName = foundData->scripName;
    symbolData->groupName = foundData->groupName;
    symbolData->timeElapsedBse = foundData->timeElapsedBse;
    symbolData->timeElapsedNse = foundData->timeElapsedNse;
    symbolData->optionMode = foundData->optionMode;
    symbolData->optionAttribute = foundData->optionAttribute;
    symbolData->instrumentType = foundData->instrumentType;
    symbolData->strikePrice = foundData->strikePrice;
    symbolData->noDelivery = foundData->noDelivery;
    symbolData->marketLot = foundData->marketLot;
    symbolData->tickSize = foundData->tickSize;
    symbolData->suspended = foundData->suspended;
    symbolData->avmFlag = foundData->avmFlag;
    symbolData->avmBuyMargin = foundData->avmBuyMargin;
    symbolData->avmSellMargin = foundData->avmSellMargin;
    symbolData->spanMargin = foundData->spanMargin;
    symbolData->exposureMargin = foundData->exposureMargin;
    //symbolData->settlementType = foundData->settlementType;
    symbolData->isinNumber = foundData->isinNumber;
    symbolData->haircut = foundData->haircut;
    symbolData->bcastFlag = foundData->bcastFlag;
    symbolData->mktPicBcastFlag = foundData->mktPicBcastFlag;
    symbolData->productCode = foundData->productCode;
    symbolData->underlyingId = foundData->underlyingId;
    symbolData->lowerBandPrice = foundData->lowerBandPrice;
    symbolData->upperBandPrice = foundData->upperBandPrice;
    symbolData->freezeQuantity = foundData->freezeQuantity;
    symbolData->closePrice = foundData->closePrice;
    symbolData->maturityDay = foundData->maturityDay;
    symbolData->maturityYearmon = foundData->maturityYearmon;
    symbolData->symbolAlias = symbol;
    return 0;
  } else {
    return -1;
  }
}

void SG::SymbolStaticData::dump() 
{
  const char *SecurityType[]    = { "COMMON_STOCK", "FUTURE", "OPTION", "MAX_SECURITY_TYPE" };
  const char *OptionMode[]      = { "PUT", "CALL", "MAX_OPTION_MODES" };
  const char *OptionAttribute[] = { "AMERICAN", "EUROPEAN", "MAX_OPTION_ATTRIBUTES" };
  const char *InstrumentType[]  = { "INDEX", "STOCK", "CURRENCY", "MAX_INSTRUMENT_TYPE" };
  
  std::cout << "Symbol Static Data Dump : ";
  std::cout << std::endl << "symbolId : " << symbolId;
  std::cout << std::endl << "marketId : " << marketId;
  std::cout << std::endl << "securityId : " << securityId;
  std::cout << std::endl << "symbol : " << symbol;
  std::cout << std::endl << "securityType : " << SecurityType[static_cast<short>(securityType)];
  std::cout << std::endl << "scripName : " << scripName;
  std::cout << std::endl << "groupName : " << groupName;
  std::cout << std::endl << "maturityYearmon : " << maturityYearmon;
  std::cout << std::endl << "maturityDay : " << maturityDay;
  std::cout << std::endl << "optionMode : " << OptionMode[static_cast<short>(optionMode)];
  std::cout << std::endl << "optionAttribute : " << OptionAttribute[static_cast<short>(optionAttribute)];
  std::cout << std::endl << "timeElapsedBse : " << timeElapsedBse;
  std::cout << std::endl << "timeElapsedNse : " << timeElapsedNse;
  std::cout << std::endl << "instrumentType : " << InstrumentType[static_cast<short>(instrumentType)];
  std::cout << std::endl << "strikePrice : " << strikePrice;
  std::cout << std::endl << "noDelivery : " << noDelivery;
  std::cout << std::endl << "marketLot : " << marketLot;
  std::cout << std::endl << "tickSize : " << tickSize;
  std::cout << std::endl << "suspended : " << suspended;
  std::cout << std::endl << "avmFlag : " << avmFlag;
  std::cout << std::endl << "avmBuyMargin : " << avmBuyMargin;
  std::cout << std::endl << "avmSellMargin : " << avmSellMargin;
  std::cout << std::endl << "spanMargin : " << spanMargin;
  std::cout << std::endl << "exposureMargin : " << exposureMargin;
  std::cout << std::endl << "settlementType : " << settlementType;
  std::cout << std::endl << "isinNumber : " << isinNumber;
  std::cout << std::endl << "haircut : " << haircut;
  std::cout << std::endl << "bcastFlag : " << bcastFlag;
  std::cout << std::endl << "mktPicBcastFlag : " << mktPicBcastFlag;
  std::cout << std::endl << "productCode : " << productCode;
  std::cout << std::endl << "underlyingId : " << underlyingId;   
  std::cout << std::endl << "is_auction_eligible : " << is_auction_eligible;
  std::cout << std::endl << "hi_52_wk : " << hi_52_wk;
  std::cout << std::endl << "lo_52_wk : " << lo_52_wk;
  std::cout << std::endl << "lowerBandPrice : " << lowerBandPrice;
  std::cout << std::endl << "upperBandPrice : " << upperBandPrice;
  std::cout << std::endl << "freezeQuantity : " << freezeQuantity;
  std::cout << std::endl << "closePrice : " << closePrice;
  std::cout << std::endl << "scripPrecision : " << scripPrecision;
  std::cout << std::endl << "symbolAlias : " << symbolAlias;
  std::cout << std::endl;

  return;
}

SG::SymbolStaticData::SymbolStaticData(const char * buf) // deserializing
{
  int offset = 0;

  UNSIGNED_CHARACTER  tmpChar = 0;
  UNSIGNED_INTEGER tmp1 = 0, tmp2 = 0;
  UNSIGNED_LONG tmpLong1 = 0, tmpLong2 = 0;

  marketId = buf + offset;
  offset += MARKET_NAME_SIZE;
  DESERIALIZE_64(tmpLong1, tmpLong2, securityId = tmpLong2, buf, offset);
  symbol = buf + offset;
  offset += SYMBOL_SIZE;
  DESERIALIZE_8(tmpChar, securityType = tmpChar, buf, offset);
  scripName = buf + offset;
  offset += SCRIP_NAME_SIZE;
  groupName = buf + offset;
  offset += SERIES_SIZE;
  
  DESERIALIZE_32(tmp1, tmp2, maturityYearmon = tmp2, buf, offset);
  DESERIALIZE_32(tmp1, tmp2, maturityDay = tmp2, buf, offset);

  DESERIALIZE_64(tmpLong1, tmpLong2, timeElapsedBse = tmpLong2, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, timeElapsedNse = tmpLong2, buf, offset);
  DESERIALIZE_8(tmpChar, optionMode = tmpChar, buf, offset);
  DESERIALIZE_8(tmpChar, optionAttribute = tmpChar, buf, offset);
  DESERIALIZE_8(tmpChar, instrumentType = tmpChar, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, strikePrice = tmpLong2, buf, offset);
  DESERIALIZE_8(tmpChar, noDelivery = tmpChar, buf, offset);
  DESERIALIZE_32(tmp1, tmp2, marketLot = tmp2, buf, offset);
  DESERIALIZE_32(tmp1, tmp2, tickSize = tmp2, buf, offset);
  DESERIALIZE_8(tmpChar, suspended = tmpChar, buf, offset);
  DESERIALIZE_8(tmpChar, avmFlag = tmpChar, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, avmBuyMargin = tmpLong2, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, avmSellMargin = tmpLong2, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, spanMargin = tmpLong2, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, exposureMargin = tmpLong2, buf, offset);
  DESERIALIZE_8(tmpChar, settlementType = tmpChar, buf, offset);
  isinNumber = buf + offset;
  offset += ISIN_SIZE;
  DESERIALIZE_64(tmpLong1, tmpLong2, haircut = tmpLong2, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, bcastFlag = tmpLong2, buf, offset);
  mktPicBcastFlag = buf + offset;
  offset += MARKET_PICTURE_BROADCAST_FLAG_SIZE;
  productCode = buf + offset;
  offset += PRODUCT_CODE_SIZE;
  DESERIALIZE_64(tmpLong1, tmpLong2, underlyingId = tmpLong2, buf, offset);
  DESERIALIZE_32(tmp1, tmp2, is_auction_eligible = tmp2, buf, offset);
  DESERIALIZE_32(tmp1, tmp2, hi_52_wk = tmp2, buf, offset);
  DESERIALIZE_32(tmp1, tmp2, lo_52_wk = tmp2, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, lowerBandPrice = tmpLong2, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, upperBandPrice = tmpLong2, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, freezeQuantity = tmpLong2, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, closePrice = tmpLong2, buf, offset);
  DESERIALIZE_64(tmpLong1, tmpLong2, symbolId = tmpLong2, buf, offset);
  DESERIALIZE_32(tmp1, tmp2, scripPrecision = tmp2, buf, offset);
  symbolAlias = buf + offset;
  offset += SYMBOL_ALIAS_SIZE;
}

int SG::SymbolStaticData::serialize(char * buf)
{
  int bytes = 0;

  UNSIGNED_CHARACTER tmpChar;
  UNSIGNED_INTEGER tmpInt;
  UNSIGNED_SHORT tmpShort = 0;
  UNSIGNED_LONG tmpLong1;
  bytes = sizeof(UNSIGNED_SHORT);  // Leave 2 bytes for packet size
  // Put category of response
  SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(RSP::ResponseCategory_SCRIP_MASTER_ROW), buf, bytes);  //Command Category
  memcpy(buf + bytes, marketId.c_str(), MARKET_NAME_SIZE);
  bytes += MARKET_NAME_SIZE;
  SERIALIZE_64(tmpLong1, securityId, buf, bytes);
  memcpy(buf + bytes, symbol.c_str(), SYMBOL_SIZE);
  bytes += SYMBOL_SIZE;
  SERIALIZE_8(tmpChar, securityType, buf, bytes);
  memcpy(buf + bytes, scripName.c_str(), SCRIP_NAME_SIZE);
  bytes += SCRIP_NAME_SIZE;
  memcpy(buf + bytes, groupName.c_str(), SERIES_SIZE);
  bytes += SERIES_SIZE;
  SERIALIZE_32(tmpInt, maturityYearmon, buf, bytes);
  SERIALIZE_32(tmpInt, maturityDay, buf, bytes);
  SERIALIZE_64(tmpLong1, timeElapsedBse, buf, bytes);
  SERIALIZE_64(tmpLong1, timeElapsedNse, buf, bytes);
  SERIALIZE_8(tmpChar, optionMode, buf, bytes);
  SERIALIZE_8(tmpChar, optionAttribute, buf, bytes);
  SERIALIZE_8(tmpChar, instrumentType, buf, bytes);
  SERIALIZE_64(tmpLong1, strikePrice, buf, bytes);
  SERIALIZE_8(tmpChar, noDelivery, buf, bytes);
  SERIALIZE_32(tmpInt, marketLot, buf, bytes);
  SERIALIZE_32(tmpInt, tickSize, buf, bytes);
  SERIALIZE_8(tmpChar, suspended, buf, bytes);
  SERIALIZE_8(tmpChar, avmFlag, buf, bytes);
  SERIALIZE_64(tmpLong1, avmBuyMargin, buf, bytes);
  SERIALIZE_64(tmpLong1, avmSellMargin, buf, bytes);
  SERIALIZE_64(tmpLong1, spanMargin, buf, bytes);
  SERIALIZE_64(tmpLong1, exposureMargin, buf, bytes);
  SERIALIZE_8(tmpChar, settlementType, buf, bytes);
  memcpy(buf + bytes, isinNumber.c_str(), ISIN_SIZE);
  bytes += ISIN_SIZE;
  SERIALIZE_64(tmpLong1, haircut, buf, bytes);
  SERIALIZE_64(tmpLong1, bcastFlag, buf, bytes);
  memcpy(buf + bytes, mktPicBcastFlag.c_str(), MARKET_PICTURE_BROADCAST_FLAG_SIZE);
  bytes += MARKET_PICTURE_BROADCAST_FLAG_SIZE;
  memcpy(buf + bytes, productCode.c_str(), PRODUCT_CODE_SIZE);
  bytes += PRODUCT_CODE_SIZE;
  SERIALIZE_64(tmpLong1, GET_SCRIP_CODE(underlyingId), buf, bytes);
  SERIALIZE_32(tmpInt, is_auction_eligible, buf, bytes);
  SERIALIZE_32(tmpInt, hi_52_wk, buf, bytes);
  SERIALIZE_32(tmpInt, lo_52_wk, buf, bytes);
  SERIALIZE_64(tmpLong1, lowerBandPrice, buf, bytes);                               
  SERIALIZE_64(tmpLong1, upperBandPrice, buf, bytes);                               
  SERIALIZE_64(tmpLong1, freezeQuantity, buf, bytes);                               
  SERIALIZE_64(tmpLong1, closePrice, buf, bytes);                               
  SERIALIZE_64(tmpLong1, symbolId, buf, bytes);                               
  SERIALIZE_32(tmpInt, scripPrecision, buf, bytes);
  memcpy(buf + bytes, symbolAlias.c_str(), SYMBOL_ALIAS_SIZE);
  bytes += SYMBOL_ALIAS_SIZE;

  UNSIGNED_SHORT dummyBytes = 0;
  // Put size as the first field after deducting 2 bytes reserved for size
  SERIALIZE_16(tmpShort, (UNSIGNED_SHORT)(bytes-sizeof(UNSIGNED_SHORT)), buf, dummyBytes);

  return bytes;
}
