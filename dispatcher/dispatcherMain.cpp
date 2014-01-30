#include <iostream>
#include "symbolIdParser.h"
#include "symbolIdFdMap.h"

#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include "subscriptionServer.h"
#include "fdDispatcher.h"

int main()
{ 
  namespace bpo=boost::program_options;

  std::string configFile = "./utrade.conf";
  int subscribtionServPort;
  int dispatcherThreads;
  int exchangeID;
  bool mclUseDispatcher;
  bool useDispatcherFO;
  bool useDispatcherCM;

  try
  {
	  bpo::options_description config("Configuration");
	  config.add_options()
    
    ( "TBT-NSE-FO.subscribtionServPort", 
      bpo::value<int>(&subscribtionServPort)->default_value(9225), 
      "Port on which subscription server will be running")
    ( "TBT-NSE-FO.dispatcherThreads", 
      bpo::value<int>(&dispatcherThreads)->default_value(3), 
      "Number of dispatcher threads")
    ( "TBT-NSE-FO.exchangeID", 
      bpo::value<int>(&exchangeID)->default_value(0), 
      "Exchange Id of symbol")
    ( "MCL.use_dispatcher", 
      bpo::value<bool>(&mclUseDispatcher)->default_value(0), 
      "0, to disable use dispatcher for mcl")
    ( "TBT-NSE-FO.useDispatcher", 
      bpo::value<bool>(&useDispatcherFO)->default_value(0), 
      "0, to disable use dispatcher for FO")
    ( "TBT-NSE-CM.useDispatcher", 
      bpo::value<bool>(&useDispatcherCM)->default_value(0), 
      "0, to disable use dispatcher for CM")
    ;
  
    bpo::options_description configFileOptions ;
    configFileOptions.add( config );

    bpo::variables_map vm ;
    std::ifstream ifs( configFile.c_str() ) ;

    if(ifs)
    {
      store( parse_config_file( ifs, configFileOptions, true ), vm ) ;
      bpo::notify ( vm ) ;
    }
    else
    {
      return 0 ;
    }
  }
  catch(boost::program_options::error &e)
  { 
	  std::cout << "Bad Options: " << e.what()
		          << std::endl;
	  return 1;
  }
  catch(std::exception& e)
  { 
	  return 1;
  }

  MarketData::SymbolIdParser::setExchangeId(exchangeID);
  MarketData::FdDispatcher::setUseDispatcherFO(useDispatcherFO);
  MarketData::FdDispatcher::setUseDispatcherCM(useDispatcherCM);
  MarketData::FdDispatcher::setMclUseDispatcher(mclUseDispatcher);

  MarketData::SymbolIdParser msp;
  msp.initializeSymbolIdNameMap();

  MarketData::SubscriptionServer serv(subscribtionServPort);
  boost::thread t(serv);


  MarketData::FdDispatcher fdd(dispatcherThreads);
  fdd();
  t.join();


  return 0;
}
