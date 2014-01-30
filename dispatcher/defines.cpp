#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h> 
#endif

#include <boost/asio.hpp>
#include <string.h>
#include <iostream>
#include <errno.h>


#ifdef __GNUC__
#include <sys/time.h>
#include <time.h>
#else
#define _WINSOCKAPI_
#include < windows.h >
#endif

#include "defines.h"

UNSIGNED_SHORT htons_16(uint16_t x)
{
    return(IS_BIG_ENDIAN?x:SWAP16(x));
}

UNSIGNED_SHORT ntohs_16(uint16_t x)
{
    return(IS_BIG_ENDIAN?x:SWAP16(x));
}

UNSIGNED_INTEGER htonl_32(uint32_t x)
{
    return(IS_BIG_ENDIAN?x:SWAP32(x));
}

UNSIGNED_INTEGER ntohl_32(uint32_t x)
{
    return(IS_BIG_ENDIAN?x:SWAP32(x));
}

UNSIGNED_LONG htonl_64(uint64_t x)
{
    return(IS_BIG_ENDIAN?x:SWAP64(x));
}

UNSIGNED_LONG ntohl_64(uint64_t x)
{
    return(IS_BIG_ENDIAN?x:SWAP64(x));
}

SIGNED_LONG signed_htonl_64(int64_t x)
{
    return(IS_BIG_ENDIAN?x:SWAP64(x));
}

SIGNED_LONG signed_ntohl_64(int64_t x)
{
    return(IS_BIG_ENDIAN?x:SWAP64(x));
}


#ifdef __GNUC__

//#ifndef FRONTEND_COMPILATION
// Linux compilation
// Enter this function, only when bytes are available to be read
// No way to get out of this function without reading a packet
char*
getPacket(int socketFd, 
          char recvbuf[], 
          int bufSize, 
          int& bytesAvailable, 
          int& bytesProcessed,
          bool& noMoreBytes,
          bool& clientDisconnected)
{
  UNSIGNED_SHORT tmp1 = 0, tmp2 = 0;
  char* packet = 0;
  UNSIGNED_SHORT packetSize = 0;
  int bytes = 0;
  bool firstAttempt = true;

  while(!packet)
  {
    if (bytesAvailable <= 0 && (noMoreBytes || clientDisconnected) ) {
      return 0;
    }

    if (bytesAvailable == 0 || !firstAttempt) {
      if ( !noMoreBytes && !clientDisconnected) {
        int requestBytes = bufSize - bytesAvailable;
        bytes = read(socketFd, recvbuf + bytesAvailable, requestBytes);
        if (bytes > 0) {
          // Some bytes are received, process them
          bytesAvailable += bytes;
        } else if (bytes == -1) { 
          noMoreBytes = true;
        } else if (bytes == 0) {
          // Socket might have sent a close or EOF signal
          clientDisconnected = true;
          std::cout << "Client is detected as disconnected: requestBytes: " << requestBytes  
                    << " bufSize : " << bufSize
                    << " bytesAvailable : " << bytesAvailable << std::endl;
        } 
      }
    }
    firstAttempt = false;

    // Check if bytes for size are available
    if ( (int)bytesAvailable >= (int)(sizeof(UNSIGNED_SHORT)) )
    {
        DESERIALIZE_16(tmp1, tmp2, packetSize = tmp2, recvbuf, bytesProcessed);
        bytesAvailable -= sizeof(UNSIGNED_SHORT);
        if (bytesAvailable >= packetSize)
        {
            packet = recvbuf + bytesProcessed;
            bytesAvailable -= packetSize;
            if (bytesAvailable > 0)
                bytesProcessed += packetSize;
            else
                bytesProcessed = 0;
        }
        else
        {
            bytesAvailable += sizeof(UNSIGNED_SHORT);
            memcpy(recvbuf, recvbuf + bytesProcessed - sizeof(UNSIGNED_SHORT), bytesAvailable);
            bytesProcessed = 0;
        }
    } else {
      memcpy(recvbuf, recvbuf + bytesProcessed, bytesAvailable);
      bytesProcessed = 0;
    }
  }

  return packet;
}
#endif

#ifndef FRONTEND_COMPILATION
static pthread_mutex_t uniqueOrderIdMutex = PTHREAD_MUTEX_INITIALIZER;

#if defined(_WIN32) || defined(_WIN64)
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone 
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag = 0;

  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    tmpres /= 10;  /*convert into microseconds*/
    /*converting file time to unix epoch*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS; 
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }

  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }

  return 0;
}
#endif

long
generateUniqueOrderId(int strategyId)
{
  // PASSED param is ignored, but kept there so not to change signature

  static int orderIdCounter = 1;
  const int maxOrdersPerSecond = 10000;
  const int digitsForSeconds = 100000;
  struct timeval timestamp;
  // Take lock
  pthread_mutex_lock( &uniqueOrderIdMutex ); 
  gettimeofday(&timestamp, NULL); 
  long orderId = timestamp.tv_sec % digitsForSeconds +
                 orderIdCounter * digitsForSeconds; 
  orderIdCounter++;
  if (orderIdCounter == maxOrdersPerSecond)
    orderIdCounter = 0;
  // Release lock
  pthread_mutex_unlock( &uniqueOrderIdMutex ); 
  
  return orderId; 
}
#endif



