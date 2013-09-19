// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef READWRITEBIN_H
#define READWRITEBIN_H

#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdexcept>


/*! \file readwritebin.h
Reading and writing binary data
*/


namespace ralab
{
  namespace base{
    namespace utils
    {

      /*! \brief write a Binary File
                        */
      template<typename T>
      bool writeBin(
          const std::vector<T> & vec,  //!< [in] data to write
          std::ofstream & out //!< [in] connection ofstream
          )
      {
        if(vec.empty()) //check if there is anything to write...
          {
            return false;
          }
        if ( out.is_open())
          {
            const char* pBuffer = reinterpret_cast<const char*>(&vec[0]);
            size_t writtenBytes( out.rdbuf()->sputn( pBuffer, vec.size()*sizeof(T) ) );
            assert( writtenBytes == vec.size()*sizeof(T) );
            return true;
          }
        return false;

      }

      /*! \brief write a Binary File

                        */
      template<typename T>
      bool writeBin(
          const std::vector<T> & vec, //!< [in] data to write
          const std::string & sScanFile //!< [in] File name
          )
      {
        //open connection for writing.
        std::ofstream out(sScanFile.c_str(), std::ios_base::out | std::ios_base::binary);
        bool status = writeBin(vec, out);
        out.close();
        return status;
      }
      /*! \brief read binary file


             */
      template<typename T>
      bool readBin(
          std::ifstream & file , //!<[in] connection
          std::vector<T> & vec //!<[out] storage
          )
      {
        if (file.is_open())
          {
            char * memblock;
            // get length of file:
            file.seekg (0, std::ios::end);
            std::size_t size = file.tellg();
            file.seekg (0, std::ios::beg);
            memblock = new char [size];
            file.read (memblock, size);
            file.close();
            T * tres = reinterpret_cast<T *>(memblock);
            vec.assign(tres, tres + (size * sizeof(char) / sizeof(T) ) );
            delete[] memblock;
            return(true);
          }
        return false;
      }


      /*! \brief read a binary file


            */
      template<typename T>
      bool readBin(
          const std::string & sScanFile, //!<[in] filename and path
          std::vector<T> & vec //!<[out] storage
          )
      {
        std::cerr << ("file :'" + sScanFile + "'") << std::endl;
        std::ifstream file ( sScanFile.c_str() , std::ios::in|std::ios::binary|std::ios::ate);

        if ( file.bad() || !(file.is_open()) )
          {
            throw std::runtime_error("file not found:'" + sScanFile + "'");
          }
        bool status = readBin(file,vec);
        file.close();
        return status;
      }

      /*! @} */
    }//end namespace fileio
  }//end namespace ralab
}


#endif // READWRITEBIN_H
