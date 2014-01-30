    /*
    * =====================================================================================
    *
    *       Filename:  symbolIdFdMap.cpp
    *
    *    Description:  
    *
    *        Version:  1.0
    *        Created:  12/02/2013 09:11:28 AM
    *       Revision:  none
    *       Compiler:  gcc
    *
    *         Author:  Abhishek Anand (), abhishek.anand@utradesolutions.com
    *   Organization:  uTrade Solutions
    *
    * =====================================================================================
    */

    #include "symbolIdFdMap.h"

    namespace MarketData
    {
    void SymbolIdFdMap::initialize( int symbolId)
    {
        std::cout << "Symbol =====> " << symbolId <<" added in Map." << std::endl;
        std::deque<std::pair<int, int> > q;
        _idFdMap[symbolId] = q;
    }
    
    void SymbolIdFdMap::addFd(int symbolId, int feedType, int fd)
    {
        _idFdMap[symbolId].push_back(std::make_pair(feedType, fd));
        _fdIdMap[fd].push_back(symbolId);
    }
    
    //void SymbolIdFdMap::addFdInConsolidatedMap(int symbolId, int fd)
    //{
    //  _idFdMap[symbolId].push_back(std::make_pair(symbolId, fd));
    //  _fdIdMap[fd].push_back(symbolId);
    //}
    template<class T>
    int findIndex(T &vec, int id)
    {
        int idx =0;
        for(auto it = vec.begin(); it!=vec.end(); ++it, ++idx)
            if( *it == id)
                return idx;
        return -1;
    }

    void SymbolIdFdMap::deleteAllSymbolOfFd(int fd)
    {
        auto tempVec = _fdIdMap[fd];
        for(auto itr = tempVec.begin(); itr != tempVec.end(); ++itr)
        {
            int symbolId = *itr;
            _iter = _idFdMap.find(symbolId);
            if(_iter != _idFdMap.end())
            {
                //<feedType, symbolId>
                std::deque<std::pair<int, int> > q =  _iter->second;
                auto qItr = q.begin();
                while(qItr != q.end())
                {
                    if( std::get<1>(*qItr) == fd)
                    {
                        int idx = findIndex(_fdIdMap[fd], fd);
                        if(idx>=0) 
                            _fdIdMap[fd].erase(_fdIdMap[fd].begin()+idx);
                        q.erase(qItr);
                        break;
                    }  
                    ++qItr;
                }
                _iter->second = q;
            }
        }
        _fdIdMap.erase(fd);
    }

    void SymbolIdFdMap::deleteFd(int symbolId, int feedType, int fd)
    {
        _iter = _idFdMap.find(symbolId);
        if(_iter != _idFdMap.end())
        {
            //<feedType, symbolId>
            std::deque<std::pair<int, int> > q =  _iter->second;
            auto qItr = q.begin();
            while(qItr != q.end())
            {
                if(std::get<0>(*qItr) == feedType 
                    && std::get<1>(*qItr) == fd)
                {
                    int idx = findIndex(_fdIdMap[fd], fd);
                    if(idx>=0) 
                        _fdIdMap[fd].erase(_fdIdMap[fd].begin()+idx);
                    q.erase(qItr);
                    break;
                }  
                ++qItr;
            }
            //q.erase(std::find(q.begin(), q.end(), fd));//std::make_pair(fd, feedType)));
            _iter->second = q;
        }
    }
    
    std::deque<std::pair<int, int> >& SymbolIdFdMap::getFdForSymbol(int symbolId)
        throw (std::domain_error)
    {
        _iter = _idFdMap.find(symbolId);
        if(_iter == _idFdMap.end())
        {  
            std::cout <<" ======= Symbol Id is not found in the map. "<<std::endl;
            throw std::domain_error("Symbol Id is not found in the map.");
        }
        return _iter->second;
    }

    std::deque<int>& SymbolIdFdMap::getFdForSymbolConsolidated(int symbolId) 
        throw (std::domain_error)
    {
        auto iter = _consolidatedIdFdMap.find(symbolId);
        if(iter == _consolidatedIdFdMap.end())
        {  
            std::cout <<" ======= Symbol Id is not found in the map. "<<std::endl;
            throw std::domain_error("Symbol Id is not found in the map.");
        }
        return iter->second;
    }
    
    
    std::deque<int>& SymbolIdFdMap::getSymbolForFdConsolidated(int fd) 
        throw (std::domain_error)
    {
        auto iter = _consolidatedFdIdMap.find(fd);
        if(iter == _consolidatedFdIdMap.end())
        {  
            std::cout <<" ======= Fd is not found in the map. "<<std::endl;
            throw std::domain_error("Fd is not found in the map.");
        }
        return iter->second;
    }
    
    
    void SymbolIdFdMap::addFdInConsolidatedMap(int symbolId, int fd)
    {
        _consolidatedIdFdMap[symbolId].push_back( fd);
        _consolidatedFdIdMap[fd].push_back(symbolId);
    }
    
    void SymbolIdFdMap::deleteFdFromConsolidatedMap(int symbolId, int fd)
    {
        auto iter = _consolidatedIdFdMap.find(symbolId);
        if(iter != _consolidatedIdFdMap.end())
        {
            std::deque<int > q =  iter->second;
            auto qItr = q.begin();
            while(qItr != q.end())
            {
                if(  (*qItr) == fd )
                {
                    int idx = findIndex(_consolidatedFdIdMap[fd], fd);
                    if(idx>=0) 
                        _consolidatedFdIdMap[fd].erase(_consolidatedFdIdMap[fd].begin()+idx);
                    q.erase(qItr);
                    break;
                }  
                ++qItr;
            }
            //q.erase(std::find(q.begin(), q.end(), fd));//std::make_pair(fd, feedType)));
            iter->second = q;
            
        }
    }

    void SymbolIdFdMap::deleteAllSymbolOfConsolidatedFd(int fd)
    {
        auto tempVec = _consolidatedFdIdMap[fd];
        for(auto itr = tempVec.begin(); itr != tempVec.end(); ++itr)
        {
            int symbolId = *itr;
            auto tempIter = _consolidatedIdFdMap.find(symbolId);
            if(tempIter != _consolidatedIdFdMap.end())
            {
                std::deque<int > q =  tempIter->second;
                auto qItr = q.begin();
                while(qItr != q.end())
                {
                    if( (*qItr) == fd)
                    {
                        int idx = findIndex(_consolidatedFdIdMap[fd], fd);
                        if(idx>=0) 
                            _consolidatedFdIdMap[fd].erase(_consolidatedFdIdMap[fd].begin()+idx);
                        q.erase(qItr);
                        break;
                    }  
                    ++qItr;
                }
                tempIter->second = q;
            }
        }
        _consolidatedFdIdMap.erase(fd);
    }

    
    
    
    
    void SymbolNameIdMap::setId(std::string &name, int &id)
    {
        _symbolNameIdMap[name] = id;
    }
    
    int SymbolNameIdMap::getId(std::string &name)
    {
        auto itr =    _symbolNameIdMap.find(name);
        if(itr != _symbolNameIdMap.end())
        {
            return itr->second;
        }
        else
        {
            return 0;
        }
    }
    
    
    
    
}//namespace









