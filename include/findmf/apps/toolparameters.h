// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef TOOLPARAMETERS_H
#define TOOLPARAMETERS_H

#include <string>
#include <stdint.h>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

namespace ralab
{
  namespace findmf
  {
    namespace apps
    {
      struct Params
      {
        std::string infile;
        std::string outdir;
        uint32_t nrthreads;
        double ppm; // with of mz bins in ppms
        double mzscale;
        double rtscale;
        uint32_t rtpixelwidth; // in pixel
        uint32_t mzpixelwidth; // in pixel
        double minmass; // minimal mass to consider
        double maxmass; // maximum mass to consider
        double minintensity;
        uint32_t i_; //?
        uint32_t rt2sum_; //how many spectra to sum (downsampling)

        std::string filestem_;
        boost::filesystem::path outdir_;

        Params():infile(),outdir(),ppm(0.),mzscale(1.),
          rtscale(1.), rtpixelwidth(), mzpixelwidth(), minmass(), maxmass(), minintensity(), i_(), rt2sum_(), filestem_(), outdir_(){}

        //stern...
        void prepareOutputFile(bool create_dir=false)
        {
          if( !boost::filesystem::exists(infile) )
            {
              std::cerr << "could not find specified file :" ;
              std::cerr << infile << std::endl;
              return;
            }
          filestem_ = boost::filesystem::path(infile).stem().stem().stem().string(); //createOutputs(p1.string() ,"fitered");
          outdir_ = boost::filesystem::path(outdir);
          outdir_ /= filestem_;
          if(create_dir){
              boost::filesystem3::create_directory(outdir_);
            }
          filestem_ += "_";
          filestem_ += boost::lexical_cast<std::string>(i_);
        }


      };//end struct param
    }
  }

}//namespace ralab end
#endif // TOOLPARAMETERS_H
