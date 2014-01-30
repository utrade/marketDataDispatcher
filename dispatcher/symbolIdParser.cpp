#include <boost/unordered_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <fstream>
#include "symbolIdParser.h"
#include "symbolIdFdMap.h"
#include <commands.h>
#include <boost/lexical_cast.hpp>
//#include <client/foSymbol.h>
#include "fdDispatcher.h"

namespace MarketData 
{
  int SymbolIdParser::_exchangeId = 0;

  void SymbolIdParser::initializeSymbolIdNameMap()
  {
    if(MarketData::FdDispatcher::getMclUseDispatcher())
    {
      getMclSymbolsFromFile("./config/instrMap.csv"); 
      getMclSymbolsFromFile("./config/mclSymbols.csv"); 
      getMclSymbolsFromFile("./config/bse100List.txt");
      getMclSymbolsFromFile("./config/sensexCurrent.txt");
      getMclSymbolsFromFile("./config/sensexNear.txt");
    }

  
    //if(MarketData::FdDispatcher::getUseDispatcherCM())
    // getCmSymbolsFromFile("./config/tbt/client/cmClientSymbols.txt");
    
    //if(MarketData::FdDispatcher::getUseDispatcherFO())
    //  getFoSymbolsFromFile("./config/tbt/client/foClientSymbols.txt");

  }


  void SymbolIdParser::getMclSymbolsFromFile(std::string file)
  {
    std::cout << "\n\nReading mcl symbols from file : " << file << std::endl;
    using namespace std;
    string line;
    ifstream myfile (file.c_str());
    if (myfile.is_open())
    {
      while ( myfile.good() )
      {
        getline (myfile,line);
        std::vector<std::string> symbols;

        typedef boost::tokenizer<boost::char_separator<char> > 
          tokenizer;
        boost::char_separator<char> sep(",|");
        tokenizer tokens(line, sep);

        for ( tokenizer::iterator tok_iter = tokens.begin();
              tok_iter != tokens.end(); 
              ++tok_iter )
        {
          symbols.push_back(*tok_iter);
        }

        BOOST_FOREACH( std::string symbol, symbols )
        {
          unsigned int found = symbol.find(":");
          if(found != std::string::npos)
          {
            std::string exch = symbol.substr(0,found);
            int securityId = 
              atoi(symbol.substr(found+1, symbol.length() - 1 - found ).c_str());

            int exchangeId = 0;
            using namespace CMD;

            if(exch == std::string("NSE"))
              exchangeId = ExchangeId_NSE;
            else if(exch == std::string("BSE"))
              exchangeId = ExchangeId_BSE;
            else if(exch == std::string("ESMNSE"))
              exchangeId = ExchangeId_ESMNSE;
            else 
              continue;

            // get unique symbol id independent of exchange 
            long symbolId = GET_SYMBOL_ID( securityId, exchangeId); 
            //Adding token id into SymbolIdFdMap
            MarketData::SymbolIdFdMap::getInstance()->initialize(symbolId);
            std::cout << "Symbol Id ===> " << symbolId
                      << " added in the map." << std::endl;
          }
        }

      }
      myfile.close();
    }
    else 
    {  
      cout << "Unable to open file "<<file <<std::endl; 
    }

  }

#if 0
  void SymbolIdParser::getCmSymbolsFromFile(std::string fileName)
  {
    FILE* fpCmClientSymbols = fopen(fileName.c_str(), "r");

    if(!fpCmClientSymbols)
    {
      fclose(fpCmClientSymbols);
      std::cout << "Error in opening " << fileName << std::endl;
    }

    char Symbol[20];
    int closePrice;
    int tickSize;
    char titleString[50];
    int tokenId = 1;
    fgets(titleString,50,fpCmClientSymbols);

    char eof = 0;

    while(eof != EOF)
    {
      eof = fscanf(fpCmClientSymbols, "%d", &tokenId);
      if(eof == EOF)
        continue;
      eof = fgetc(fpCmClientSymbols);
      eof = fscanf(fpCmClientSymbols, "%s", Symbol);
      eof = fgetc(fpCmClientSymbols);
      eof = fscanf(fpCmClientSymbols, "%d", &closePrice);
      eof = fgetc(fpCmClientSymbols);
      eof = fscanf(fpCmClientSymbols, "%d", &tickSize);
      eof = fgetc(fpCmClientSymbols);
      //eof = fgetc(fpCmClientSymbols);
      std::string symbol(Symbol, strlen(Symbol));
      symbol.resize(10, ' ');

      // get unique symbol id independent of exchange 
      int symbolId = GET_SYMBOL_ID( tokenId, _exchangeId); 
      std::cout << "CM: Adding " << symbolId <<" in the map."
                << std::endl ;

      MarketData::SymbolNameIdMap::getInstance()->setId(symbol, symbolId);
      
      //Adding token id into SymbolIdFdMap
      MarketData::SymbolIdFdMap::getInstance()->initialize(symbolId);

    }

    fclose(fpCmClientSymbols);
    std::cout<<"\nCM : client list initialized"<<std::endl;
  }

  
  void SymbolIdParser::getFoSymbolsFromFile(std::string fileName)
  {
    std::cout<<"\nFO : initiallizing client tokens list";
    FILE* fpFoClientSymbols = fopen(fileName.c_str(), "r");

    if(!fpFoClientSymbols)
    {
      std::cout << " SYMBOLS_FILE_NOT_FOUND " << std::endl;
      fclose(fpFoClientSymbols);
    }

    int tokenId;
    int closePrice;
    int tickSize;
    char line[200];


    fgets(line,200,fpFoClientSymbols);
    if(feof(fpFoClientSymbols))
    {
      std::cout << " EMPTY_SYMBOLS_LIST " << std::endl;
      return ;
    }

    typedef boost::tokenizer< boost::char_separator<char> >
      tokenizer;

    boost::char_separator<char> sep(",\n\r");


    while(! feof(fpFoClientSymbols))
    {
      memset(line, 0, 200);
      fgets(line, 200, fpFoClientSymbols);

      std::string str(line);

      tokenizer tokenFields(str, sep);

      std::vector<std::string> vecTokenFields;
      for (tokenizer::iterator iter = tokenFields.begin();
          iter != tokenFields.end();
          iter++ )
        vecTokenFields.push_back( *iter );

      if (vecTokenFields.size() == 0)
        continue;

      tokenId = boost::lexical_cast<int>(vecTokenFields[0]);
      closePrice = boost::lexical_cast<int>(vecTokenFields[1]);
      tickSize = boost::lexical_cast<int>(vecTokenFields[2]);

      std::string symbol;
      int32_t expiryDate, strikePrice;
      char optionType;

      symbol = vecTokenFields[3];
      expiryDate = boost::lexical_cast<int>(vecTokenFields[4]);
      strikePrice = boost::lexical_cast<int>(vecTokenFields[5]);
      optionType = *(vecTokenFields[6].data());

      // get unique symbol id independent of exchange 
      int symbolId = GET_SYMBOL_ID( tokenId, _exchangeId); 

      std::cout << "Input parameters in file: "
                << "Symbol: " << symbol
                << " Expiry: " << expiryDate 
                << " Strike: " << strikePrice
                << " Option: " << optionType 
                << " TokenId: " << tokenId
                << std::endl;

      tbt::FOSymbol foSymbol;
      foSymbol.setSymbol( symbol );
      foSymbol.setExpiryDate( expiryDate );
      foSymbol.setStrikePrice( strikePrice );
      foSymbol.setOptionType( optionType );

      std::string foSymbolStr = std::string(foSymbol);
      MarketData::SymbolNameIdMap::getInstance()->setId(foSymbolStr, symbolId);

      //Adding token id into SymbolIdFdMap
      MarketData::SymbolIdFdMap::getInstance()->initialize(symbolId);

    }

    fclose(fpFoClientSymbols);
    std::cout<<"\nFO: Symbol map initialized\n";
  }
#endif
  void inline getCurrentLocalTimeStamp()
  {
    struct timeval ttime;
    gettimeofday(&ttime, NULL);

    time_t curtime;
    curtime=ttime.tv_sec;
    char buffer[30];
    strftime(buffer,30,"%T",localtime(&curtime));

    std::cout << buffer << "." << ttime.tv_usec << " |  ";

  }

} // namespaces
