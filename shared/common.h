#ifndef COMMON_H
#define COMMON_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>

//#include <sys/time.h>

#define USE_FIX_MESSAGE 1

// TBD:- Change it as MK evolves
#define MAX_SYMBOLS 100

#define MAX_DATA_PACKETS 20000
#define MAX_SYMBOL_CHARS 200
#define MAX_PIPES_PER_SYMBOL 50000
#define MAX_STRATEGIES 3000
#define MAX_BUF_SIZE 1024
#define MAX_SYMBOLS_PER_STRATEGY 5
#define MAX_CONFIRMATIONS_PER_STRATEGY 100
#define MAX_ADMIN_CLIENTS 10
#define MAX_ADMIN_CONNECTIONS MAX_ADMIN_CLIENTS+2
#define LISTEN_PORT 8285
#define DEFAULT_STRATEGY_ID 0
#define MAX_SOCKETS 1000
#define MAX_EPOLL_EVENTS 50
#define CONFIRMATION_MULTIPLIER_STRATEGY_ID     1000000
#define SSL_CERTIFICATE_NAME_LENGTH 100
#define MAX_CONNECTIVITY_MESSAGES 10
#define MAX_IML_IDS 100

#if 0
static struct timeval timestamp;

#define CURRENT_TIMESTAMP \
  gettimeofday(&timestamp, NULL); \
  std::cout << " " << timestamp.tv_sec \
            << " : " << timestamp.tv_usec \
            << std::endl;
#endif

#define STRATEGY_ID_DIGITS 10000

#ifndef FRONTEND_COMPILATION
long
generateUniqueOrderId(int strategyId);
#endif


#define INTNULL    -1
#define CHARNULL   'Z' 
#define STRNULL    "NULL"
#define GETINT(field) \
  std::getline(lineStream,cell,',');\
  if(cell.empty()) \
    field = INTNULL; \
  else \
    field = atol(cell.c_str()); 

#define GETCHAR(field) \
  std::getline(lineStream,cell,','); \
  if(cell.empty()) \
    field = CHARNULL; \
 else \
   field = cell[0]; 

#define GETSTRING(field) \
  std::getline(lineStream,field,','); \
  if(field.empty()) \
    field = STRNULL;


#endif
