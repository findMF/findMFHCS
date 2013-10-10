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
  namespace base
  {
    namespace utils
    {
      /*!\brief read file into string
      */
      inline std::string read(const std::string & filename){
        std::ifstream in(filename.c_str() , std::ios::in | std::ios::binary);
        if( in )
          {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);
          }
        throw std::logic_error("connection not open!");
      }

      /*!\brief write lines to a connection */
      template<typename T>
      void writeLines
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

      }

      /*!\ Write lines to file */
      template<typename T>
      void writeLines
      (
          const std::vector<T> & source //!<[in] vector of lines to write
          ,const std::string & dest //!<[out] name of file
          )
      {
        std::fstream file(dest.c_str(), std::ios::out); //open connection
        writeLines(source,file);
        file.close();
      }



      /*! \brief Read some or all text lines from a file.
            */
      template<typename T>
      void readLines(
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

                for( std::string line; std::getline( file, line ); )
                  {
                    res.push_back(boost::lexical_cast<T>(line));
                  }


                file.close();
              }
            else // read until and of file or until line n
              {
                for( std::string line; std::getline( file, line ) && i < static_cast<unsigned int>(n); )
                  {
                    res.push_back(boost::lexical_cast<T>(line));
                    i++;
                  }


                file.close();
              }

            //if last line endl only than truncate result.
            if(line.size()==0)
              {
                res.resize(res.size()-1);
              }
            return;
          }//end if file.is_open()
        throw std::logic_error("connection not open!");
      }
    }//end namespace fileio
  }//end namespace ralab
}

#endif // READWRITELINES_H
