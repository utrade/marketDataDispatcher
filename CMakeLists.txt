cmake_minimum_required(VERSION 2.6)

project(disaptch)
set(disaptch_VERSION_MAJOR 0)
set(disaptch_VERSION_MINOR 0)
set(disaptch_VERSION_PATCH 1)

set(disaptch_VERSION_STRING
  "${disaptch_VERSION_MAJOR}.${disaptch_VERSION_MINOR}.${disaptch_VERSION_PATCH}"
  )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message(STATUS "Setting Default Build Type : Debug")
endif(NOT CMAKE_BUILD_TYPE)

#FIND_PACKAGE(Subversion)
#IF(Subversion_FOUND)
#  Subversion_WC_INFO(${CMAKE_HOME_DIRECTORY} disaptch)
#  MESSAGE("Current revision is ${disaptch_WC_REVISION}")
#ENDIF(Subversion_FOUND)

configure_file (
  "${disaptch_SOURCE_DIR}/config.h.in"
  "${disaptch_SOURCE_DIR}/config.h"
  )

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Boost 1.42 REQUIRED COMPONENTS
  system
  filesystem
  thread
  program_options
  regex
  )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=gnu++11 -Wall -fPIC -fpermissive")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=gnu++11 -Wall -fPIC -fpermissive")

include_directories(${disaptch_SOURCE_DIR}/dispatcher)
include_directories(${disaptch_SOURCE_DIR}/shared)

SET(EXECUTABLE_OUTPUT_PATH ${disaptch_SOURCE_DIR}/bin)                                                                                                   
SET(LIBRARY_OUTPUT_PATH ${disaptch_SOURCE_DIR}/bin) 

add_subdirectory(dispatcher)
add_subdirectory(shared)
