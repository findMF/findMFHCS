// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SPECTRAMAP_H
#define SPECTRAMAP_H

#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/timer.hpp>
#include <algorithm>

#include <pwiz/data/msdata/MSDataFile.hpp>
#include <glog/logging.h>

#include "findmf/datastruct/lcmsimage.h"
#include "findmf/datastruct/swathinfo.h"

#include "findmf/fileiopwiz/swathpropertiesreader.h"
#include "findmf/fileiopwiz/filteredspectralist.h"

#include "findmf/application/RT2sum.h"

namespace ralab{
  namespace findmf{

    /**
   \brief reads map from MSData

   default initialization is performed with an file name.
  */
    struct LCMSImageReader{
    private:
      pwiz::msdata::MSDataPtr msdataptr_;
      std::pair<double, double> mzRange_;
      double ppm_; // determined from instrument resolution using for resampling mass axis
      RT2sum rt2sum_; // how many spectra to sum
      typedef boost::shared_ptr<datastruct::LCMSImage> LCMSImagePtr;
      datastruct::SwathInfoPtr swathInfo_; // holds the swath information
      std::vector<int> idx_;
      std::vector<double> weight_;

    public:

      LCMSImageReader(std::string & filename,
                      double ppm = 100,
                      uint32_t rt2sum=1
          ):
        msdataptr_(),ppm_(ppm),rt2sum_(rt2sum)
      {
        msdataptr_ = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(filename));
        SwathPropertiesReader x(msdataptr_);
        swathInfo_ = x.getSwathInfo();
      }

      ///use this constructor when you have an swathInfo object already
      LCMSImageReader(pwiz::msdata::MSDataPtr data,
                      datastruct::SwathInfoPtr swathInfo,
                      double ppm = 100,
                      uint32_t rt2sum = 1
          ): msdataptr_(data), ppm_(ppm), rt2sum_(rt2sum), swathInfo_(swathInfo) {
      }

      void getKeys(std::vector<std::size_t> & keys){
        swathInfo_->getKeys(keys);
      }

      //returns ms1 map
      void getMS1Map( datastruct::LCMSImage & image ){
        //for ms1 maps we do not limit the mass range.
        getMap( 0 , 0., std::numeric_limits<double>::max(), image);
      }

      //returns ms2 map for key
      void getMap(unsigned int key,double minMass, double maxMass, datastruct::LCMSImage & map_)
      {
        map_.setMapDescription(swathInfo_->getMapForKey(key));
        boost::timer time;
        mzRange_ = determineRange(
              msdataptr_,
              swathInfo_->getMapForKey(key)->indices_
              );
        if(mzRange_.first < minMass){
            mzRange_.first = minMass;
          }
        if(mzRange_.second > maxMass){
            mzRange_.second = maxMass;
          }
        map_.getMapDescription()->mzRange_= mzRange_;
        size_t nrcols = swathInfo_->getMapForKey(key)->indices_.size();
        nrcols = rt2sum_.getNrCols(nrcols);

        LOG(INFO) << "key : [" << key << "] map properties read : [" << time.elapsed() << "]"<< std::endl;
        time.restart();

        size_t rows = map_.setBreak(mzRange_, ppm_ );
        map_.resize(rows,nrcols);

        fillLCMSImage(msdataptr_,swathInfo_->getMapForKey(key)->indices_,map_);
        LOG(INFO) << "key : [" << key << "]  map created : [" << time.elapsed() << "]"<< std::endl;
      }

      /// Store file on disk
      void write(const std::string & filenameOut, datastruct::LCMSImage & mmap){
        pwiz::msdata::SpectrumListPtr sl = msdataptr_->run.spectrumListPtr;
        pwiz::msdata::SpectrumListPtr mp( new FilteredSpectrumList(sl,mmap,rt2sum_) );
        msdataptr_->run.spectrumListPtr = mp;
        std::string fileOut = filenameOut;
        fileOut += ".mzML";
        pwiz::msdata::MSDataFile::write(*msdataptr_,
                                        fileOut,
                                        pwiz::msdata::MSDataFile::Format_mzML
                                        ); // uncomment if you want mzXML
      }

    private :
      /// Converts a sparse spec to a dense spec
      void convert2dense(std::vector<double> & mz,
                         std::vector<double> & intens,
                         std::size_t specnr,
                         datastruct::LCMSImage & map_
                         ){
        std::size_t l = mz.size();
        for(std::size_t i = 0 ; i < (l-1) ; ++i){

            std::size_t bin;
            double mass1 = mz[i];
            double mass2 = mz[i+1];
            if(map_.inRange(mass1))
              {
                map_.getBin(mass1,mass2,idx_,weight_);
                double intensd = static_cast<double>(intens[i]);
                double nr = mass2-mass1;
                for(std::size_t i = 0 ; i < idx_.size();++i){
                    map_.put(idx_[i], specnr, intensd * weight_[i]/nr );
                  }

              }
          }
      }

      /* fills the map  */
      void fillLCMSImage(pwiz::msdata::MSDataPtr msd,
                         const std::vector<std::size_t> & indices, // spectrum indices
                         datastruct::LCMSImage & map_ // output map.
                         ){
        pwiz::msdata::SpectrumListPtr sl = msd->run.spectrumListPtr;
        if (sl.get())
          {
            std::size_t nrcols_ = indices.size() ;
            for(std::size_t i = 0 ; i < nrcols_ ; ++i)
              {
                pwiz::msdata::SpectrumPtr sp = sl->spectrum(indices[i],true);
                pwiz::msdata::BinaryDataArrayPtr mz,intensity;
                pwiz::msdata::SpectrumInfo specInfo(*sp);
                double rt = specInfo.retentionTime;

                map_.getRT().push_back(rt);
                mz = sp->getMZArray();

                intensity = sp->getIntensityArray();
                size_t idx = rt2sum_.getCols(i);
                convert2dense(mz->data, intensity->data, idx , map_);
                //std::cout << idx << " " << i << std::endl;
              }
          }
      }

      /* determines range of dataset by iterating
       through all spectra in indices */
      std::pair<double, double> determineRange( pwiz::msdata::MSDataPtr msd,
                                                const std::vector<std::size_t> & indices )
      {
        double min_( std::numeric_limits<double>::max()) , max_( std::numeric_limits<double>::min());
        pwiz::msdata::SpectrumListPtr sl = msd->run.spectrumListPtr;
        if (sl.get())
          {
            std::size_t nrcols_ = indices.size();
            for(std::size_t i = 0 ; i < nrcols_ ; ++i)
              {
                pwiz::msdata::SpectrumPtr sp = sl->spectrum(indices[i],true);
                pwiz::msdata::BinaryDataArrayPtr mz;
                mz = sp->getMZArray();
                if(min_ > mz->data.front()) min_ = mz->data.front();
                if(max_ < mz->data.back()) max_ = mz->data.back();
              }
            return std::pair<double, double>(min_,max_);
          }
        return std::pair<double, double>(0.,0.);
      }

    };
  }
}//end namespace ralab


#endif
