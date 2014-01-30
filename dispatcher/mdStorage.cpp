/*
* =====================================================================================
*
*       Filename:  mdStorage.cpp
*
*    Description:  
*
*        Version:  1.0
*        Created:  01/17/2014 02:25:44 PM
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  Abhishek Anand (), abhishek.anand@utradesolutions.com
*   Organization:  uTrade Solutions
*
* =====================================================================================
*/

#include "mdStorage.h"

namespace MarketData
{
    void MDStorage::save( const long symbolId, const CMD::MDQuote* quote)
    {
        std::cout << "\n======== Storing market data quote ========= \n";
        quote->dump();
        _symbolQuoteMap[symbolId] = (CMD::MDQuote*) quote;
    }

    CMD::MDQuote MDStorage::get(const long symbolId)
    {
        CMD::MDQuote mdq;
        if(_symbolQuoteMap.find(symbolId) == _symbolQuoteMap.end())
            std::cout   << "Error: SymbolId " << symbolId 
                             << " not found in _symbolQuoteMap." << std::endl;
        else
        {
            return *_symbolQuoteMap[symbolId];
        }
        return mdq;
    }
}
