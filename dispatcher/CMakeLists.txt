add_executable( dispatcher 
  symbolIdParser.cpp
  dispatcherMain.cpp
  fdDispatcher.cpp
  subscriptionServer.cpp
  subscriptionServMsg.cpp
  symbolIdFdMap.cpp
  mdStorage.cpp
)

target_link_libraries( dispatcher
      /usr/local/lib/libevent.a
      pthread
      ${Boost_PROGRAM_OPTIONS_LIBRARY}
      boost_system
      boost_thread
      rt
      shared
)

