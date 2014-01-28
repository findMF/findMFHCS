// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef _HELPERFUNCTIONS_
#define _HELPERFUNCTIONS_
#include <string>
#include <boost/filesystem.hpp>

namespace ralab{
  namespace findmf{
    ///add descript to filename stem (remove extension)
    inline boost::filesystem::path createOutputs(const std::string & name ,
                                                 const std::string & descript)
    {
      boost::filesystem::path p1;
      boost::filesystem::path p(name);

      p = boost::filesystem::absolute(p);
      p1 = p.parent_path();

      std::string x = p.stem().string();
      x += descript ;
      p1 /=  x;
      return p1;
    }

    ///remove extension from filename and add a new one
    inline boost::filesystem::path addextension(const std::string & filename,
                                                const std::string & extension
                                                )
    {
      boost::filesystem::path p(filename);
      std::string fnamex = p.stem().string();
      fnamex += "." + extension;
      boost::filesystem::path p1 = p.parent_path();
      p1 /= fnamex;
      return p1;
    }
  }
}

#endif

