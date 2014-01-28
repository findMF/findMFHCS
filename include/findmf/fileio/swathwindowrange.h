// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SWATHWINDOWRANGE_H
#define SWATHWINDOWRANGE_H

#include "base/utils/readtable.h"

namespace ralab{
  namespace findmf{

    ///
    /// Swath Window parent mass ranges. Helper class for reading swath information
    ///

    struct SwathWindowRange{
      //private:
      typedef std::map<uint32_t , std::pair<double,double> > MCont;
      MCont swathBoundaries_; //parent mass boundaries
    public:

      void put(uint32_t swathidx, double massStart, double massEnd){
        swathBoundaries_.insert(std::make_pair(swathidx,std::make_pair(massStart, massEnd)));
      }
      std::pair<double, double> get(uint32_t swathidx){
        MCont::iterator it = swathBoundaries_.find(swathidx);
        if(it != swathBoundaries_.end())
          return it->second;
        else
          return std::make_pair(-1.,-1.);
      }

      void getKeys(std::vector<uint32_t> & keys){
        MCont::iterator beg = swathBoundaries_.begin();
        MCont::iterator end = swathBoundaries_.end();
        for(;beg!=end; ++beg)
        {
          keys.push_back(beg->first);
        }
      }

      void read(const std::string & filename){
        typedef std::vector<std::vector<double> > Table;
        Table tableData;
        ralab::base::utils::utilities::read_table_byrow<double>(
              filename,
              tableData,
              '\t'
              );
        Table::iterator beg = tableData.begin();
        Table::iterator end = tableData.end();
        for(;beg!=end;++beg)
        {
          if(beg->size() == 3)
            this->put(static_cast<uint32_t>((*beg)[2]),(*beg)[0],(*beg)[1]);
        }
      }//read table
    };
  }
}
#endif // SWATHWINDOWRANGE_H
