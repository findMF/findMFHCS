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
      /// tool parameters
      /// TODO maybee compose of several objects?
      struct Params
      {
        // general options
        std::string infile; //!< input file location
        std::string outdir; //!< out directory location
        uint32_t nrthreads; //!< nr of threads to use
        uint32_t rt2sum_; //!< how many spectra to sum (downsampling)

        /// image genration options
        double ppm; //!< with of mz bins in ppms
        double minmass; //!< minimal mass to consider
        double maxmass; //!< maximum mass to consider

        /// filtering options
        bool dofilter;
        double mzscale; //!< peak extension in mz
        double rtscale; //!< peak extension in RT
        uint32_t rtpixelwidth; //!< peak rt base with in pixel
        uint32_t mzpixelwidth; //!< peak mz base with in pixel


        /// segementations options
        double minintensity; //!< minimal intensity
        bool writeprojections_; //!< should projections be written

        std::string filestem_; //!< stem of input file
        boost::filesystem::path outdir_; //!< the out directory prepended with the filestem

        Params():infile(),outdir(),rt2sum_(),ppm(0.),minmass(), maxmass(), dofilter(true), mzscale(1.),
          rtscale(1.), rtpixelwidth(), mzpixelwidth(),  minintensity(), writeprojections_(false),  filestem_(), outdir_(){}

        //stern
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
        }


      };//end struct param
    }
  }

}//namespace ralab end
#endif // TOOLPARAMETERS_H
