// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef LCMSIMAGEREADER_H
#define LCMSIMAGEREADER_H

#include <tbb/pipeline.h>

#include "findmf/fileiopwiz/lcmsimagereader.h"

namespace ralab{
  namespace findmf{


    /// source - Read a single lcms map
    struct LCMSImageReaderFilter : tbb::filter{

      struct Params{
        std::string infile_; //!< location of input file
        double ppm_; //!< mass precsion of instrument in ppm
        uint32_t rt2sum_; //!< nr of spectra to sum
        double minmass_; //!< minmass
        double maxmass_; //!< maxmass
      };

      std::vector<std::size_t> keys; //!< keys to swath map
      ralab::findmf::datastruct::MSFileInfoPtr sip_; //!< pointer to swath maps informations
      Params anap_; //!< input parameters - TODO replace by parameter specific parameters
      uint32_t count_; //!< count the maps you have read

      LCMSImageReaderFilter(
          Params & ap, // the analysis parameter
          ralab::findmf::datastruct::MSFileInfoPtr sip // swath info pointer
          ) :tbb::filter(serial_in_order),sip_(sip), anap_(ap),count_(0)
      {
        //anap_.prepareOutputFile(false);
      }

      /// read the image from an file.
      /// @return pointer to LCMSImage
      void * operator()(void * )
      {
        boost::timer time;
        time.restart();
        std::vector< std::size_t > keys;
        sip_->getKeys(keys);
        if(count_ < keys.size())
        {

          ralab::findmf::datastruct::LCMSImage * lcmsimage = new ralab::findmf::datastruct::LCMSImage(sip_);
          {

            pwiz::msdata::MSDataPtr msdataptr = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(anap_.infile_));

            //TODO passing sip_ here seems redundant
            ralab::findmf::LCMSImageReader imagereader (msdataptr, sip_, anap_.ppm_, anap_.rt2sum_);


            std::cerr << "PROCESSING MAP : " << count_ << " with key : " << keys[count_] <<std::endl;

            imagereader.getMap( keys[count_] ,anap_.minmass_ , anap_.maxmass_, *lcmsimage);

            lcmsimage->getImageMap().updateImageMax();
            double max = lcmsimage->getImageMap().getImageMax();
            std::cerr << "max : " << max << std::endl;
            count_++;

          }
          return lcmsimage;
        }
        else
        {
          return NULL;
        }

      }
    };
    //end LCMSImageReader

  }
}

#endif
