#ifndef SDEFINES_H
#define SDEFINES_H

#include <stdint.h>
#include <string.h>
#include <sstream>
#include <string>

typedef uint8_t UNSIGNED_CHARACTER;
typedef uint16_t UNSIGNED_SHORT;
typedef uint32_t UNSIGNED_INTEGER;
typedef uint64_t UNSIGNED_LONG;
typedef int64_t SIGNED_LONG;

#define SYMBOL_SIZE 24
#define SCRIP_NAME_SIZE 100
#define SYMBOL_ALIAS_SIZE 50
#define ISIN_SIZE 50
#define MARKET_PICTURE_BROADCAST_FLAG_SIZE 10
#define PRODUCT_CODE_SIZE 20
#define MARKET_NAME_SIZE 24
#define SERIES_SIZE 20
#define EXCHANGE_ORDERID_SIZE 50
#define TRADEID_SIZE 50
#define PASSWORD_SIZE 100
#define PASSWORD_SALT_SIZE 100
#define CHARACTER_FIELD_SIZE 100
#define ACCOUNT_FIELD_SIZE 12
#define MAX_LOAD_ORDERS 300000
#define MAX_LOAD_CONFIRMATIONPARAMS 300000

#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

#define SWAP16(a) ( \
    ((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8))

#define SWAP32(a) ( \
  ((a & 0x000000FF) << 24) | \
  ((a & 0x0000FF00) <<  8) | \
  ((a & 0x00FF0000) >>  8) | \
  ((a & 0xFF000000) >> 24))

#define SWAP64(a) ( \
    ((a & 0x00000000000000FFULL) << 56) | \
    ((a & 0x000000000000FF00ULL) << 40) | \
    ((a & 0x0000000000FF0000ULL) << 24) | \
    ((a & 0x00000000FF000000ULL) <<  8) | \
    ((a & 0x000000FF00000000ULL) >>  8) | \
    ((a & 0x0000FF0000000000ULL) >> 24) | \
    ((a & 0x00FF000000000000ULL) >> 40) | \
    ((a & 0xFF00000000000000ULL) >> 56))

UNSIGNED_SHORT htons_16(uint16_t x);
UNSIGNED_SHORT ntohs_16(uint16_t x);
UNSIGNED_INTEGER   htonl_32(uint32_t x);
UNSIGNED_INTEGER   ntohl_32(uint32_t x);
UNSIGNED_LONG  htonl_64(uint64_t x);
SIGNED_LONG  signed_htonl_64(int64_t x);
UNSIGNED_LONG  ntohl_64(uint64_t x);
SIGNED_LONG  signed_ntohl_64(int64_t x);

/////////////////////////////////////////////////////////////

#define SERIALIZE_8(tmp, getMethod, buf, bytes) \
    tmp = getMethod; \
    memcpy(buf+bytes, &tmp, sizeof(tmp)); \
    bytes += sizeof(tmp);

#define SERIALIZE_16(tmp, getMethod, buf, bytes) \
    tmp = htons_16(getMethod);\
    memcpy(buf+bytes, &tmp, sizeof(tmp));\
    bytes += sizeof(tmp);

#define SERIALIZE_32(tmp, getMethod, buf, bytes) \
    tmp = htonl_32(getMethod);\
    memcpy(buf+bytes, &tmp, sizeof(tmp));\
    bytes += sizeof(tmp);

#define SERIALIZE_64(tmp, getMethod, buf, bytes) \
    tmp = htonl_64(getMethod);\
    memcpy(buf+bytes, &tmp, sizeof(tmp));\
    bytes += sizeof(tmp);

#define SERIALIZE_SIGNED_64(tmp, getMethod, buf, bytes) \
    tmp = signed_htonl_64(getMethod);\
    memcpy(buf+bytes, &tmp, sizeof(tmp));\
    bytes += sizeof(tmp);

/////////////////////////////////////////////////////////////

#define DESERIALIZE_8(tmp, setMethod, buf, offset)\
    memcpy(&tmp, buf + offset, sizeof(tmp));\
    setMethod;\
    offset  += sizeof(tmp);

#define DESERIALIZE_16(tmp, tmpData, setMethod, buf, offset)\
    memcpy(&tmp, buf + offset, sizeof(tmp));\
    tmp = ntohs_16(tmp);\
    memcpy(&tmpData, &tmp, sizeof(tmpData));\
    setMethod;\
    offset  += sizeof(tmp);

#define DESERIALIZE_32(tmp, tmpData, setMethod, buf, offset)\
    memcpy(&tmp, buf + offset, sizeof(tmp));\
    tmp = ntohl_32(tmp);\
    memcpy(&tmpData, &tmp, sizeof(tmpData));\
    setMethod;\
    offset  += sizeof(tmp);

#define DESERIALIZE_64(tmp, tmpData, setMethod, buf, offset)\
    memcpy(&tmp, buf + offset, sizeof(tmp));\
    tmp = ntohl_64(tmp);\
    memcpy(&tmpData, &tmp, sizeof(tmpData));\
    setMethod;\
    offset  += sizeof(tmp);

#define DESERIALIZE_SIGNED_64(tmp, tmpData, setMethod, buf, offset)\
    memcpy(&tmp, buf + offset, sizeof(tmp));\
    tmp = signed_ntohl_64(tmp);\
    memcpy(&tmpData, &tmp, sizeof(tmpData));\
    setMethod;\
    offset  += sizeof(tmp);

/////////////////////////////////////////////////////////////


#ifndef FRONTEND_COMPILATION
char*getPacket(int socketFd,
          char recvbuf[], 
          int bufSize, 
          int& bytesAvailable, 
          int& bytesProcessed,
          bool& noMoreBytes,
          bool& clientDisconnected);
#endif

/////////////////////////////////////////////////////////////


template <class T>
inline std::string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}



#endif
