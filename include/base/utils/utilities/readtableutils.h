#ifndef READTABLEUTILS_H
#define READTABLEUTILS_H

#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdexcept>
#include <string>
#include <boost/lexical_cast.hpp>

namespace ralab
{
  namespace base
  {
    namespace utils
    {
      namespace utilities
      {
        /// transpose
        template<typename TReal>
        void transpose(
            const std::vector< std::vector<TReal> > & x, //!< [in] matrix in
            std::vector<std::vector<TReal> > &y //!< [out] transposed
            )
        {
          if(!x.empty())
            {
              size_t nrow  = x.size();
              size_t ncol = x[0].size();
              std::vector<TReal> tmp;
              for(unsigned long i = 0 ; i < ncol; ++i)
                {
                  tmp.clear();
                  for(unsigned long j = 0 ; j < nrow ; ++j)
                    {
                      tmp.push_back(x[j][i]);
                    }
                  y.push_back(tmp);
                }
            }
        }

        template<typename TReal>
        void read_table_byrow(
            const std::string & filename,
            std::vector< std::vector<TReal> > & vvData,
            char sep = '\t'
            )
        {
          if(filename.empty())
            {
              throw std::runtime_error("no filename specified");
            }
          std::ifstream file_stream;
          file_stream.open(filename.c_str());
          if ( file_stream.bad() || !(file_stream.is_open()) )
            {
              throw std::runtime_error("file not found:'" + filename + "'");
            }

          typedef std::vector<TReal> StringVec;
          /*typedef std::vector<StringVec> RowVec;*/
          vvData.reserve(10000);
          std::string line;
          while (std::getline(file_stream , line)) {
              std::string entry;
              std::istringstream linestrm(line);
              vvData.push_back(StringVec());
              while (std::getline(linestrm, entry, sep)) {
                  vvData.back().push_back(boost::lexical_cast<TReal>(entry));
                }
            }
        }


        /*! \brief read table by columns
        */

        template<typename TReal>
        void read_table_bycol(const std::string & filename,
                              std::vector< std::vector<TReal> > & vvData,
                              char sep = '\t')
        {
          vvData.clear();
          std::vector< std::vector<TReal> > tmp;
          read_table_byrow(filename,tmp,sep);
          utilities::transpose(tmp,vvData);
        }
      }//namespace UTILITIES
    }
  }
}

#endif
