// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef READWRITELINES_H
#define READWRITELINES_H

#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>


/*! \file ReadWriteLine.h
\brief Read from Write in Lines File
*/

namespace ralab
{
  namespace base{
    namespace utils
    {

      /*!\ Write lines to file */
      template<typename T> bool writeLines
      (
          const std::vector<T> & source //!<[in] vector of lines to write
          ,const std::string & dest //!<[out] name of file
          )
      {
        std::fstream file(dest.c_str(), std::ios::out); //open connection
        writeLines(source,file);
        file.close();
        return true;
      }

      /*!\brief write lines to a connection */
      template<typename T> bool writeLines
      (
          const std::vector<T> & source //!< [in] vector of lines to write
          ,std::fstream & dest //!<[out] file stream
          )
      {
        if (dest.is_open())
          {
            for(unsigned int i = 0 ; i < source.size(); ++i )
              {
                dest << source[i] << std::endl;
              }
          }
        return true;
      }



      /*! \brief Read some or all text lines from a file.
            */
      template<typename T>
      bool readLines(
          const std::string & sScanFile //!<[in] file name
          , std::vector<T> & res //!<[out] the lines
          , int n=-1 //!< Integer. The (maximal) number of lines to read. Negative values indicate that one should read up to the end of the connection.
          )
      {
        std::fstream file(sScanFile.c_str(), std::ios::in); //open connection
        if (file.is_open())
          {
            unsigned int i = 0;
            std::string line;

            if(n <= 0) // read until end of file
              {
                while(!file.eof())
                  {
                    file >> line;
                    res.push_back(boost::lexical_cast<T>(line));
                    line.clear();
                  }
                file.close();
              }
            else // read until and of file or until line n
              {
                while(!file.eof() && i < static_cast<unsigned int>(n))
                  {
                    file >> line;
                    res.push_back( boost::lexical_cast<T>(line) );
                    line.clear();
                    i++;
                  }
                file.close();
              }

            //if last line endl only than truncate result.
            if(line.size()==0)
              {
                res.resize(res.size()-1);
              }
            return true;
          }//end if file.is_open()
        return false;
      }
      /*! \brief Read some or all text lines from a connection.
            */

      /*!@}*/
    }//end namespace fileio
  }//end namespace ralab
}

#endif // READWRITELINES_H
