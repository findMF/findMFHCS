// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef READTABLE_H
#define READTABLE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdexcept>
#include <string>

#include <boost/lexical_cast.hpp>
#include "base/utils/utilities/readtableutils.h"
namespace ralab
{
  namespace base
  {
    namespace utils
    {

      template<typename TReal>
      void read_table(
          const std::string & filename, //!< [in] file to read
          std::vector< std::vector<TReal> > & vvData, //!< [out] vector of vectors
          bool  byrow = true, //!< default true by row
          char sep = '\t'
          )
      {
        if(byrow)
          {
            return utilities::read_table_byrow<TReal>(filename,vvData,sep);
          }
        else
          {
            return utilities::read_table_bycol<TReal>(filename,vvData,sep);
          }
      }

    }//end namespace fileio
  }//end namespace ralab
}

#endif // READTABLE_H
