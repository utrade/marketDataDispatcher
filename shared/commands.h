#ifndef COMMANDS_H
#define COMMANDS_H

#include "defines.h"

#include <vector>

#ifndef FRONTEND_COMPILATION
//#include <symbolStaticData.h>
#include "common.h"
#endif

namespace CMD {

enum ExchangeId {
  ExchangeId_BSE = 1,
  ExchangeId_NSE,
  ExchangeId_ESMNSE,
  ExchangeId_SGX,
  ExchangeId_NSECDS,
  ExchangeId_BSEETI,
  ExchangeId_CFH,
  ExchangeId_MAX
};

#define EXCHANGE_ID_BASE 10000000

#define GET_SYMBOL_ID(scripCode, exchangeId) ((exchangeId)*EXCHANGE_ID_BASE + scripCode)

#define GET_SCRIP_CODE(symbolId) (symbolId % EXCHANGE_ID_BASE)

#define GET_EXCHANGE_ID(symbolId) ((CMD::ExchangeId)(symbolId/EXCHANGE_ID_BASE))
  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  enum CommandCategory
  {
    CommandCategory_SINGLE_ORDER,               // New, modify, cancel single order
    CommandCategory_DUMMY_TWO_LEG_STRATEGY,     // New strategy, Modify not supported 
    CommandCategory_TWO_LEG_ARBITRAGE_STRATEGY, // New strategy, Modify not supported 
    CommandCategory_MARKET_MAKING_STRATEGY, // Market making strategy 
    CommandCategory_STRATEGY_COMMAND,
    CommandCategory_PAUSE_STRATEGY,                  // cancel Ask/Bid(s), wait for RUN/TERMINATE_SQUAREOFF
    CommandCategory_RUN_STRATEGY,                    // Run Strategy 
    CommandCategory_TERMINATE_STRATEGY,              // Cancel Ask/Bid(s), positions remains, close thread
    CommandCategory_TERMINATE_SQUAREOFF,             // Cancel Ask/Bids, square off/hedge postions, close thread
    CommandCategory_LOGIN,                      // First command
    CommandCategory_LOGOUT,                     // Logout command
    CommandCategory_RMS_UPDATE,
    CommandCategory_REQUEST_RMS_CONFIGURATION,
    CommandCategory_REQUEST_OFFLINE_DATA,
    CommandCategory_HEART_BEAT_MESSAGE,
    CommandCategory_THREE_LEG_ARBITRAGE_STRATEGY,
    CommandCategory_DUMMY_THREE_LEG_ARBITRAGE_STRATEGY,
    CommandCategory_BSE_DISCONNECTED,
    CommandCategory_NSECM_DISCONNECTED,
    CommandCategory_NSEFO_DISCONNECTED,
    CommandCategory_TBTCM_DISCONNECTED,
    CommandCategory_TBTFO_DISCONNECTED,
    CommandCategory_BSE_SNAPSHOTFEED_DISCONNECTED,
    CommandCategory_NSEFO_SNAPSHOTFEED_DISCONNECTED,
    CommandCategory_NSECM_SNAPSHOTFEED_DISCONNECTED,
    CommandCategory_MARKET_MAKING_TURNOVER_STRATEGY, // Market making strategy 
    CommandCategory_TBT_MARKET_DATA,
    CommandCategory_FOUR_LEG_BUTTERFLY_STRATEGY,
    CommandCategory_BOX_STRATEGY,
    CommandCategory_TWO_LEG_THREE_LEG_ARBITRAGE_STRATEGY,
    CommandCategory_MODIFY_STRATEGY,                 // Modify running Strategy
    CommandCategory_RMS_LOGIN_PARAMETERS,
    CommandCategory_RMS_ORDER_LIMITS,
    CommandCategory_RMS_GLOBAL_PARAMETERS,
    CommandCategory_RMS_RISK_PERMISSIONS,
    CommandCategory_GET_RMS_PARAMETERS,
    CommandCategory_CHANGE_PASSWORD,
    CommandCategory_SEND_SCRIP_MASTER_DATA,
    CommandCategory_GET_TOTAL_RECORDS_SCRIP_MASTER,
    CommandCategory_GET_TOTAL_ORDER_CONFIRMATIONS,
    CommandCategory_GET_CONNECTIVITY_STATUS,
    CommandCategory_RMS_SECURITY_WISE_LIMITS,
    CommandCategory_IMPLIED_VOLATILITY_STRATEGY,    // New strategy, Modify not supported
    CommandCategory_CASH_CASH_STRATEGY,
    CommandCategory_GET_CLIENT_CONNECTIVITY_STATUS,
    CommandCategory_FORCE_LOG_OFF_CLIENT,
	  CommandCategory_PAIRS_STRATEGY,
    CommandCategory_PAIRS_CLOSE_POSITIONS_NO_TERMINATE,         //Close Current Strategy Positions without terminating strategy
    CommandCategory_TBTCDS_DISCONNECTED,
    CommandCategory_NSECDS_DISCONNECTED,
    CommandCategory_BSEETI_DISCONNECTED,
    CommandCategory_SGX_DISCONNECTED,
    CommandCategory_INSTRUMENT_SEARCH_REQUEST,
    CommandCategory_MARKET_DATA_SUBSCRIPTION,
    CommandCategory_API_TRADED_SYMBOLS_COUNT_REQUEST, 
    CommandCategory_API_REPLAY_TRADED_SYMBOLS_REQUEST,
    CommandCategory_MAX
  };

 
class ScripMasterDataRequest
{
  public: 
    ScripMasterDataRequest() {}
    ScripMasterDataRequest(const char* buf);
    
    int serialize(char* buf);

    void setScripMasterDataRequestType (UNSIGNED_CHARACTER scripMasterDataRequestType) 
    {_scripMasterDataRequestType = scripMasterDataRequestType  ;} 
    void setClientId (UNSIGNED_INTEGER clientId) {_clientId = clientId  ;}
    void setRecordNumber (UNSIGNED_LONG recordNumber) {_recordNumber = recordNumber  ;}
    void setSecurityId (UNSIGNED_LONG securityId) {_securityId = securityId  ;}
    void setSymbolId (UNSIGNED_LONG symbolId) {_symbolId = symbolId  ;}
    void setExchangeId (UNSIGNED_SHORT exchangeId) {_exchangeId = exchangeId  ;}
    
    void setSymbol(const char * symbol) { strcpy(_symbol, symbol);}
    void setSeries(const char * series) { strcpy(_series, series);}
    void setMarketName(const char * marketName) { strcpy(_marketName, marketName);}
    void setSymbolAlias(const char * symbolAlias) { strcpy(_symbolAlias, symbolAlias);}

    void setOptionType (UNSIGNED_CHARACTER optionType) {_optionType = optionType  ;}
    void setOptionMode (UNSIGNED_CHARACTER optionMode) {_optionMode = optionMode  ;}
    void setSecurityType (UNSIGNED_CHARACTER securityType) {_securityType = securityType  ;}
    void setStrikePrice (UNSIGNED_LONG strikePrice) {_strikePrice = strikePrice  ;}
    void setExpiryYearMon (UNSIGNED_INTEGER expiryYearMon) {_expiryYearMon = expiryYearMon  ;}
    void setExpiryDate (UNSIGNED_INTEGER expiryDate) {_expiryDate = expiryDate  ;}
    void setNumberOfRecords (UNSIGNED_INTEGER numberOfRecords) {_numberOfRecords = numberOfRecords  ;}
    
    UNSIGNED_CHARACTER getScripMasterDataRequestType() { return _scripMasterDataRequestType; } 
    UNSIGNED_INTEGER getClientId() { return _clientId; }
    UNSIGNED_LONG getRecordNumber() { return _recordNumber; }
    UNSIGNED_LONG getSecurityId() { return _securityId; }
    UNSIGNED_LONG getSymbolId() { return _symbolId; }
    UNSIGNED_SHORT getExchangeId() { return _exchangeId; }
    char * getSymbol() { return _symbol; }
    char * getSeries() { return _series; }
    char * getMarketName() { return _marketName; }
    char * getSymbolAlias() { return _symbolAlias; }
    UNSIGNED_CHARACTER getOptionType() { return _optionType; }
    UNSIGNED_CHARACTER getOptionMode() { return _optionMode; }
    UNSIGNED_CHARACTER getSecurityType() { return _securityType; }
    UNSIGNED_LONG getStrikePrice() { return _strikePrice; }
    UNSIGNED_INTEGER getExpiryYearMon() { return _expiryYearMon; }
    UNSIGNED_INTEGER getExpiryDate() { return _expiryDate; }
    UNSIGNED_INTEGER getNumberOfRecords() { return _numberOfRecords; }

    void dump();
  
  private:
    UNSIGNED_CHARACTER _scripMasterDataRequestType; 
    UNSIGNED_INTEGER _clientId;
    UNSIGNED_LONG _recordNumber;
    UNSIGNED_LONG _securityId;
    UNSIGNED_LONG _symbolId;
    UNSIGNED_SHORT _exchangeId;
    char _symbol[SYMBOL_SIZE];
    char _series[SERIES_SIZE];
    char _marketName[MARKET_NAME_SIZE];
    UNSIGNED_CHARACTER _optionType;
    UNSIGNED_CHARACTER _optionMode;
    UNSIGNED_CHARACTER _securityType;
    UNSIGNED_LONG _strikePrice;
    UNSIGNED_INTEGER _expiryYearMon;
    UNSIGNED_INTEGER _expiryDate;
    UNSIGNED_INTEGER _numberOfRecords;
    char _symbolAlias[SYMBOL_ALIAS_SIZE];
};



/**
 *
 * \brief Instrument search request
 *
 * In response, the server will send SymbolStaticData if the instrument was
 * successfully found, otherwise it will just return a FAILURE message.
 *
 */

class InstrumentRequest {

  static const int MNEMONIC_LENGTH = 100;

public:

  InstrumentRequest();
  InstrumentRequest(const char* buf);
  int serialize(char* buf);
  void initialize();

  const char* getMnemonic() const { return _mnemonic; }
  void setMnemonic(const char* m) { strncpy(_mnemonic, m, MNEMONIC_LENGTH); }

private:
  char _mnemonic[MNEMONIC_LENGTH];

};

/**
 *
 * \brief MarketData Subscribe/Unsubscribe Request
 *
 * Server will send a successful response if the subscription was successful,
 * otherwise it send a unsuccessful response.
 */

class MDSubscribeRequest {

  static const int TOKEN_LENGTH = 100;

public:
  MDSubscribeRequest(){};
  MDSubscribeRequest(const char* buf);
  int serialize(char* buf);
  void initialize();

  UNSIGNED_LONG getSymbolId() const               { return _symbolId; }
  void setSymbolId(UNSIGNED_LONG val)             { _symbolId = val; }

  UNSIGNED_CHARACTER isSubscriptionReq() const    { return _isSubscription; }
  void setSubscriptionReq(UNSIGNED_CHARACTER val) { _isSubscription = val; }

  //const char* getLoginToken() const { return _loginToken; }
  //void setLoginToken(const char* m) { strncpy(_loginToken, m, TOKEN_LENGTH); }
  
  UNSIGNED_SHORT getUserId() const    { return _userId; }
  void setUserId(UNSIGNED_SHORT val) { _userId = val; }

private:
  /*
   * value 0 - unsusbscribe request
   * value 1 - susbscribe request
   */
  UNSIGNED_SHORT     _isSubscription;
  UNSIGNED_LONG      _symbolId;
  UNSIGNED_SHORT     _userId;
  //char               _loginToken[TOKEN_LENGTH];

};

/**
 *
 * \brief MarketData Quote 
 * @anchor MDQuote
 * Server will send the quote to the subscribed members.
 */
class MDQuote {
  static const int MAX_LOOKUP_LEVEL = 10;
  static const int QUOTEDATA_LENGTH = 80;
  
  public:
    MDQuote();
    MDQuote(const char* buf);
    void dumpDepth(UNSIGNED_LONG *data/*[MAX_LOOKUP_LEVEL]*/, UNSIGNED_CHARACTER depth);
    void dump();

    int serialize(char* buf);
    void initialize();

    void setSymbolId (UNSIGNED_LONG val) { _symbolId = val; }
    void setNummberOfTrades (UNSIGNED_LONG val) { _numberOfTrades = val; }
    void setVolume (UNSIGNED_LONG val) { _volume = val; }
    void setValue (UNSIGNED_LONG val) { _value = val; }
    void setLastTradePrice (UNSIGNED_LONG val) { _lastTradePrice = val; }
    void setLastTradeQty (UNSIGNED_LONG val) { _lastTradeQty = val; }
    void setOpenPrice (UNSIGNED_LONG val) { _openPrice = val; }
    void setClosePrice (UNSIGNED_LONG val) { _closePrice = val; }
    void setHighPrice (UNSIGNED_LONG val) { _highPrice = val; }
    void setLowPrice (UNSIGNED_LONG val) { _lowPrice = val; }
    void setTotalBidQty (UNSIGNED_LONG val) { _totalBidQty = val; }
    void setTotalAskQty (UNSIGNED_LONG val) { _totalAskQty = val; }
    void setLowerCktLimit (UNSIGNED_LONG val) { _lowerCktLimit = val; }
    void setUpperCktLimit (UNSIGNED_LONG val) { _upperCktLimit = val; }
    void setDepth (UNSIGNED_CHARACTER val) { _depth = val; }
    void setBidPrice ( UNSIGNED_LONG val[]) { memcpy(_bidPrice, val, _depth * sizeof(UNSIGNED_LONG)); }
    void setBidQty (UNSIGNED_LONG val[]) { memcpy(_bidQty, val, _depth * sizeof(UNSIGNED_LONG)); }
    void setAskPrice ( UNSIGNED_LONG val[]) { memcpy(_askPrice, val, _depth * sizeof(UNSIGNED_LONG)); }
    void setAskQty (UNSIGNED_LONG val[]) { memcpy(_askQty, val, _depth * sizeof(UNSIGNED_LONG)); }

    UNSIGNED_LONG getSymbolId () { return _symbolId; }
    UNSIGNED_LONG getNumberOfTrades () { return _numberOfTrades; }
    UNSIGNED_LONG getVolume () { return _volume; }
    UNSIGNED_LONG getValue () { return _value; }
    UNSIGNED_LONG getLastTradePrice () { return _lastTradePrice; }
    UNSIGNED_LONG getLastTradeQty () { return _lastTradeQty; }
    UNSIGNED_LONG getOpenPrice () { return _openPrice; }
    UNSIGNED_LONG getClosePrice () { return _closePrice; }
    UNSIGNED_LONG getHighPrice () { return _highPrice; }
    UNSIGNED_LONG getLowPrice () { return _lowPrice; }
    UNSIGNED_LONG getTotalBidQty () { return _totalBidQty; }
    UNSIGNED_LONG getTotalAskQty () { return _totalAskQty; }
    UNSIGNED_LONG getLowerCktLimit () { return _lowerCktLimit; }
    UNSIGNED_LONG getUpperCktLimit () { return _upperCktLimit; }
    UNSIGNED_CHARACTER getDepth () { return _depth; }
    UNSIGNED_LONG* getBidPrice ( ) { return _bidPrice; }
    UNSIGNED_LONG* getBidQty () { return _bidQty; }
    UNSIGNED_LONG* getAskPrice ( ) { return _askPrice; }
    UNSIGNED_LONG* getAskQty ( ) { return _askQty; }



  private:
    UNSIGNED_LONG         _symbolId;
    UNSIGNED_LONG         _numberOfTrades;
    UNSIGNED_LONG         _volume;
    UNSIGNED_LONG         _value;
    UNSIGNED_LONG         _lastTradePrice;
    UNSIGNED_LONG         _lastTradeQty;
    UNSIGNED_LONG         _openPrice;
    UNSIGNED_LONG         _closePrice;
    UNSIGNED_LONG         _highPrice;
    UNSIGNED_LONG         _lowPrice;
    UNSIGNED_LONG         _totalBidQty;
    UNSIGNED_LONG         _totalAskQty;
    UNSIGNED_LONG         _lowerCktLimit;
    UNSIGNED_LONG         _upperCktLimit;
    UNSIGNED_CHARACTER    _depth;
    UNSIGNED_LONG         _bidPrice[MAX_LOOKUP_LEVEL];
    UNSIGNED_LONG         _bidQty[MAX_LOOKUP_LEVEL];
    UNSIGNED_LONG         _askPrice[MAX_LOOKUP_LEVEL];
    UNSIGNED_LONG         _askQty[MAX_LOOKUP_LEVEL];

};

/**
 *
 * \brief MarketData Quote Consolidated 
 * @anchor MDQuoteConsolidated
 * Server will send the consolidated quote to the subscribed members.
 */
class MDQuoteConsolidated {

  static const int MAX_LOOKUP_LEVEL = 10;
  static const int QUOTEDATA_LENGTH = 80;

  public:
    MDQuoteConsolidated();
    MDQuoteConsolidated(const char* buf);
    void dumpDepth(std::vector<UNSIGNED_LONG>& data, UNSIGNED_CHARACTER depth);
    void dump();

    int serialize(char* buf);
    void initialize();

    void setNumberOfSymbols (UNSIGNED_SHORT val) { _numSymbols = val; }
    void setSymbolId (UNSIGNED_SHORT index, UNSIGNED_LONG val) { _symbolId[index] = val; }
    void setLastTradePrice (UNSIGNED_SHORT index, UNSIGNED_LONG val) { _lastTradePrice[index] = val; }
    void setLastTradeQty (UNSIGNED_SHORT index, UNSIGNED_LONG val) { _lastTradeQty[index] = val; }
    void setOpenPrice (UNSIGNED_SHORT index, UNSIGNED_LONG val) { _openPrice[index] = val; }
    void setClosePrice (UNSIGNED_SHORT index, UNSIGNED_LONG val) { _closePrice[index] = val; }
    void setHighPrice (UNSIGNED_SHORT index, UNSIGNED_LONG val) { _highPrice[index] = val; }
    void setLowPrice (UNSIGNED_SHORT index, UNSIGNED_LONG val) { _lowPrice[index] = val; }
    void setDepth (UNSIGNED_CHARACTER val) { _depth = val; }
    void setBidPrice (UNSIGNED_SHORT index, UNSIGNED_LONG val[])
    {
      std::copy(val, val+_depth, _bidPrice[index].begin());
    }
    void setBidQty (UNSIGNED_SHORT index, UNSIGNED_LONG val[])
    {
      std::copy(val, val+_depth, _bidQty[index].begin());
    }
    void setAskPrice (UNSIGNED_SHORT index, UNSIGNED_LONG val[])
    {
      std::copy(val, val+_depth, _askPrice[index].begin());
    }
    void setAskQty (UNSIGNED_SHORT index, UNSIGNED_LONG val[])
    {
      std::copy(val, val+_depth, _askQty[index].begin());
    }

    UNSIGNED_SHORT getNumberOfSymbols () { return _numSymbols; }
    UNSIGNED_LONG getSymbolId (UNSIGNED_SHORT index) { return _symbolId[index]; }
    UNSIGNED_LONG getLastTradePrice (UNSIGNED_SHORT index) { return _lastTradePrice[index]; }
    UNSIGNED_LONG getLastTradeQty (UNSIGNED_SHORT index) { return _lastTradeQty[index]; }
    UNSIGNED_LONG getOpenPrice (UNSIGNED_SHORT index) { return _openPrice[index]; }
    UNSIGNED_LONG getClosePrice (UNSIGNED_SHORT index) { return _closePrice[index]; }
    UNSIGNED_LONG getHighPrice (UNSIGNED_SHORT index) { return _highPrice[index]; }
    UNSIGNED_LONG getLowPrice (UNSIGNED_SHORT index) { return _lowPrice[index]; }
    UNSIGNED_CHARACTER getDepth () { return _depth; }

    std::vector <UNSIGNED_LONG>& getBidPrice (UNSIGNED_SHORT index) { return _bidPrice[index]; }
    //std::vector<long long>& getBidPrice (UNSIGNED_SHORT index) { return _bidPrice[index]; }
    std::vector<UNSIGNED_LONG>& getBidQty (UNSIGNED_SHORT index) { return _bidQty[index]; }
    std::vector<UNSIGNED_LONG>& getAskPrice (UNSIGNED_SHORT index) { return _askPrice[index]; }
    std::vector<UNSIGNED_LONG>& getAskQty (UNSIGNED_SHORT index) { return _askQty[index]; }

  private:
    UNSIGNED_SHORT        _numSymbols;
    std::vector<UNSIGNED_LONG>         _symbolId;
    std::vector<UNSIGNED_LONG>         _lastTradePrice;
    std::vector<UNSIGNED_LONG>         _lastTradeQty;
    std::vector<UNSIGNED_LONG>         _openPrice;
    std::vector<UNSIGNED_LONG>         _closePrice;
    std::vector<UNSIGNED_LONG>         _highPrice;
    std::vector<UNSIGNED_LONG>         _lowPrice;
    UNSIGNED_CHARACTER    _depth;
    std::vector< std::vector<UNSIGNED_LONG> > _bidPrice;
    std::vector< std::vector<UNSIGNED_LONG> > _bidQty;
    std::vector< std::vector<UNSIGNED_LONG> > _askPrice;
    std::vector< std::vector<UNSIGNED_LONG> > _askQty;
};





}// namespace CMD

#endif //COMMANDS_H
