#ifndef RESPONSE_H
#define RESPONSE_H

#include "defines.h"
#include "commands.h"
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <iomanip>

namespace RSP {


  enum ResponseCategory
  {
    ResponseCategory_CONNECTION,
    ResponseCategory_STRATEGY, // Same for new, modify, cancel strategy
    ResponseCategory_ORDER_CONFIRMATION,
    ResponseCategory_ORDER, // Same for new, modify, cancel
    ResponseCategory_DUMMY_TWO_LEG_STRATEGY,
    ResponseCategory_TWO_LEG_ARBITRAGE_STRATEGY,
    ResponseCategory_MARKET_MAKING_STRATEGY,
    ResponseCategory_OFFLINE_REQUEST_COMPLETE,
    ResponseCategory_DUMMY_THREE_LEG_ARBITRAGE_STRATEGY,
    ResponseCategory_TWO_LEG_THREE_LEG_ARBITRAGE_STRATEGY,
		ResponseCategory_THREE_LEG_ARBITRAGE_STRATEGY,
    ResponseCategory_CONNECTIVITY_STATUS, // = 11
    ResponseCategory_FILE_UPLOAD_SUCCESS,
    ResponseCategory_MORE_DATA_PENDING,
    ResponseCategory_MARKET_MAKING_TURNOVER_STRATEGY,
    ResponseCategory_TBT_MARKET_DATA,
    ResponseCategory_RMS_LOGIN_PARAMS,
    ResponseCategory_RMS_RISK_PERMISSIONS,
    ResponseCategory_RMS_ORDER_LIMITS,
    ResponseCategory_GLOBAL_RMS_PARAMS,
    ResponseCategory_RMS_PARAMS_COMPLETE,
    ResponseCategory_SCRIP_MASTER_ROW,
    ResponseCategory_MORE_SCRIP_MASTER_ROWS_PENDING,
    ResponseCategory_SCRIP_MASTER_ROWS_REPLAY_COMPLETE,
    ResponseCategory_TOTAL_RECORDS_RESPONSE,
    ResponseCategory_CONNECTIVITY_STATUS_REPLAY_COMPLETE,
    ResponseCategory_IMPLIED_VOLATILITY_STRATEGY,
    ResponseCategory_CASH_CASH_STRATEGY,
    ResponseCategory_CLIENT_CONNECTIVITY_UPDATE,
    ResponseCategory_FORCE_LOG_OFF_FROM_RMS,
    Responsecategory_PAIRS_STRATEGY,
    ResponseCategory_REQUEST_STATUS,
    ResponseCategory_API_TRADED_SYMBOLS_COUNT,
    ResponseCategory_API_TRADED_SYMBOLS_REPLAY_DONE,
    Responsecategory_MDQuote,
    Responsecategory_MDQuoteConsolidated,
    ResponseCategory_MAX
  };

  enum ResponseType
  {
    ResponseType_SUCCESS,
    ResponseType_FAILURE,
    ResponseType_TERMINATE_SUCCESS,
    ResponseType_TERMINATE_FAILURE,
    ResponseType_TERMINATE_SQUAREOFF_SUCCESS,
    ResponseType_TERMINATE_SQUAREOFF_FAILURE,
    ResponseType_PAUSE_STRATEGY_SUCCESS,
    ResponseType_PAUSE_STRATEGY_FAILURE,
    ResponseType_STRATEGY_RUNNING, // = 8
		ResponseType_TERMINATE_SPREAD_BREACHED,
    ResponseType_ALGO_NOT_ALLOWED,
    ResponseType_INCREASE_COUNTER,
    ResponseType_MAX
  };

 
  enum RequestType
  {
    RequestType_MARKET_DATA_SUBSCRIPTION,
    RequestType_INSTRUMENT_REQUEST_STATUS,
    RequestType_MAX
  };

  enum RequestStatus
  {
    RequestStatus_SUCCESS,
    RequestStatus_FAILURE,
    RequestStatus_MAX
  };

  

}

#endif
