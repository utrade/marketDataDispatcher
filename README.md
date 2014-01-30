muTrade Market Data Disptacher

Version 0.0.1 

URL:- http://www.utradesolutions.com/

Introduction : 
  It is subscription based market data event provider. Developer need to subsribe the symbol from our subscription server. Once symbol is subscribed user will get the event as snapshot for every tick update of respective symbol.


COPYRIGHT
  (C) 2014 uTrade Solutions Pvt Ltd. All Rights reserved.

Language Supported : C++

Directory structure:
  CMakeLists.txt
  config.h  
  config.h.in
  dispatcher/
    CMakeLists.txt  
    dispatcherMain.cpp
    subscriptionServer.cpp
    subscriptionServMsg.h
    commands.h      
    subscriptionServer.h
    symbolIdParser.cpp
    defines.cpp
    fdDispatcher.cpp
    mdStorage.cpp
    symbolIdFdMap.cpp
    symbolIdParser.h
    defines.h
    fdDispatcher.h
    mdStorage.h
    subscriptionServMsg.cpp
    symbolIdFdMap.h
    threadPool.h
  shared/
    CMakeLists.txt
    commands.cpp
    commands.h
    common.h
    defines.cpp
    defines.h
    response.h
    symbolStaticData.cpp
    symbolStaticData.h



Dependencies:
  cmake       (http://www.cmake.org/)
  muTrade Backend Server. (http://www.utradesolutions.com/)
  boost       (http://www.boost.org/)
  libevent      (http://libevent.org/)
