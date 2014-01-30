#ifndef SYMBOL_STATIC_DATA_H
#define SYMBOL_STATIC_DATA_H
#include <common.h>

namespace SG {
  struct SymbolStaticData {
    std::string marketId;//new
    long        securityId;
    std::string symbol;
    char        securityType;
    std::string scripName;
    std::string groupName;
    unsigned int         maturityYearmon;//new
    int         maturityDay;//new
    char        optionMode;
    char        optionAttribute;
    long        timeElapsedBse;//un
    long        timeElapsedNse;//un
    char        instrumentType;
    long        strikePrice;
    char        noDelivery;
    int         marketLot;
    int         tickSize;
    char        suspended;
    char        avmFlag;
    long        avmBuyMargin;
    long        avmSellMargin;
    long        spanMargin;
    long        exposureMargin;
    char        settlementType;
    std::string isinNumber;
    long        haircut;
    long        bcastFlag;
    std::string mktPicBcastFlag;
    std::string productCode;
    long        underlyingId;   // SymbolId
    int         is_auction_eligible;//new
    int         hi_52_wk;//new
    int         lo_52_wk;//new
    long        lowerBandPrice;
    long        upperBandPrice;
    long        freezeQuantity;
    long        closePrice;
    long        symbolId;
    int         scripPrecision;
    std::string symbolAlias;

    void dump();
    
    SymbolStaticData();
    SymbolStaticData(const char * buf);
    int serialize(char * buf);
 };
}

///////////////////////////////////////////////////////////
// APIs to insert and access symbol static data

int
initializeSymbolStaticDataStore();

int
getSymbolStaticData(long symbolId, SG::SymbolStaticData* symbolData);

// API functions
// search data in API
int
getSymbolStaticData(const std::string& str,
                    SG::SymbolStaticData* symbolData);

// sanitize symbol coming from API
std::string sanitizeApiSymbol(const std::string& str);

///////////////////////////////////////////////////////////

#endif
