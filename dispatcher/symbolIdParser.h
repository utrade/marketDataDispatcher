#ifndef _SYMBOLIDPARSER_H_
#define _SYMBOLIDPARSER_H_

#include <boost/unordered_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <fstream>

namespace MarketData 
{
  class SymbolIdParser
  {
    public:
      void initializeSymbolIdNameMap();
      void getMclSymbolsFromFile(std::string file);
      //void getFoSymbolsFromFile(std::string fileName);
      //void getCmSymbolsFromFile(std::string fileName);
      static void setExchangeId(int val) { _exchangeId = val; }

    private:
      static int _exchangeId;
  };

} // namespaces
#endif
