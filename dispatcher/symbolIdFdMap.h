#ifndef _SYMBOLID_FD_MAP_H_
#define _SYMBOLID_FD_MAP_H_

#include <iostream>
#include <map>
#include <deque>
#include <vector>
#include <utility>   
#include <unordered_map>
#include <stdexcept>

namespace MarketData
{
  typedef std::unordered_map<int, std::deque<std::pair<int, int> > > IdFdMap;
  typedef std::unordered_map<int, std::deque<std::pair<int, int> > >::iterator IdFdMapItr;
  
  typedef std::unordered_map<int, std::vector<int> > FdIdMap;

  typedef std::unordered_map<int, std::deque<int> > ConsolidatedIdFdMap;

  enum FeedType
  {
    TBT = 0,
    MCL = 1
  };

  /**
   * @brief SymbolIdFdMap class
   * This is the data structure which is used by tbt to send 
   * event on each subscriber for each symbol id.
   *
   */
  class SymbolIdFdMap
  {
    public:


      /**
       * @brief Inizlize symbol id map.
       * This function will be called when tbt will start.
       *
       * @param symbolId
       */
      void initialize(int symbolId);

      /**
       * @brief SymbolIdFdMap is is a Singleton class, which will 
       * have only one instance. This instance can be accessed using
       * the getInstance method.
       *
       * @return SymbolIdFdMap*
       */
      static SymbolIdFdMap* getInstance()
      {
        static SymbolIdFdMap s;
        return &s;
      }

      /**
       * @brief Add member in the subscribed list.
       *
       * @param symbolId
       * @param fd
       */
      void addFd(int symbolId, int feedType, int fd);     
 
      /**
       * @brief Add member in the subscribed list.
       *
       * @param symbolId
       * @param fd
       */
      void addFdInConsolidatedMap(int symbolId, int fd);     
      
      /**
       * @brief Delete member from subscribed list.
       *
       * @param symbolId
       * @param fd
       */
      void deleteFd(int symbolId, int fd, int feedType);

      /**
       * @brief Delete member from subscribed list.
       *
       * @param symbolId
       * @param fd
       */
      void deleteFdFromConsolidatedMap(int symbolId, int fd);
      
      void deleteAllSymbolOfConsolidatedFd(int fd);
      
      void deleteAllSymbolOfFd(int fd);

      /**
       * @brief Get Fds associated to symbol id.
       *
       * @param symbolId
       *
       * @return queue having fds associated to specific symbolId.
       */
      std::deque<std::pair<int, int> >& getFdForSymbol(int symbolId) throw (std::domain_error);
      
      
      std::deque<int>& getFdForSymbolConsolidated(int symbolId) throw (std::domain_error);
      
      std::deque<int>& getSymbolForFdConsolidated(int fd) throw (std::domain_error);
      
    private:
      IdFdMap               _idFdMap;
      IdFdMapItr            _iter;
      FdIdMap               _fdIdMap;
      ConsolidatedIdFdMap  _consolidatedIdFdMap;
      ConsolidatedIdFdMap  _consolidatedFdIdMap;
  };

  class SymbolNameIdMap
  {
    public:
      static SymbolNameIdMap* getInstance()
      {
        static SymbolNameIdMap s;
        return &s;
      }
      void  setId(std::string &name, int &id);
      int   getId(std::string &name);
    private:
      SymbolNameIdMap() {};
      std::map<std::string, int> _symbolNameIdMap;

  };

}//namespace 

#endif