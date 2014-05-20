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

#include "findmf/datastruct/lcmsimage.h"
#include "findmf/datastruct/msfileinfo.h"

#include "findmf/fileiopwiz/swathpropertiesreader.h"
#include "findmf/fileiopwiz/filteredspectralist.h"

#include "findmf/application/RT2sum.h"

#include "base/resample/determinebinwidth.h"
#include "base/stats/quantiles.h"


namespace ralab{
  namespace findmf{

    /// \brief reads map from MSData
    /// default initialization is performed with an file name.
    struct LCMSImageReader{
    private:
      pwiz::msdata::MSDataPtr msdataptr_; //!< pointer to file
      std::pair<double, double> mzRange_; //!< mz range to read
      double ppm_; //!< determined from instrument resolution using for resampling mass axis
      RT2sum rt2sum_; //!< how many spectra to sum
      typedef boost::shared_ptr<datastruct::LCMSImage> LCMSImagePtr;
      datastruct::MSFileInfoPtr swathInfo_; //!< holds the information about the images

      std::vector<int> idx_; //!< worker variable used for binning
      std::vector<double> weight_; //!< worker variable used for binning

    public:

      /// use this one if your file only contains one lcms map or if you are opening it for the first time
      LCMSImageReader(std::string & filename, //!< filename
                      double ppm = 100, //!< mass precision
                      uint32_t rt2sum=1 //!< nr of spectra to sum
          ):
        msdataptr_(),ppm_(ppm),rt2sum_(rt2sum)
      {
        SwathPropertiesReader spr(filename);
        msdataptr_ = spr.getMSData();
        swathInfo_ = spr.getSwathInfo();
      }

      /// use this constructor when you have an swathInfo object already
      LCMSImageReader(pwiz::msdata::MSDataPtr data, //!< msdata file
                      datastruct::MSFileInfoPtr swathInfo, //!< file info
                      double ppm = 100, //!< mass precision
                      uint32_t rt2sum = 1 //!< nr of spectra to sum
          ): msdataptr_(data), ppm_(ppm), rt2sum_(rt2sum), swathInfo_(swathInfo) {
      }

      ///
      void getKeys(std::vector<std::size_t> & keys){
        swathInfo_->getKeys(keys);
      }

      /// returns ms1 map
      void getMS1Map( datastruct::LCMSImage & image ){
        getMap( 0 , 0., std::numeric_limits<double>::max(), image);
      }

      /// returns ms2 map for key (getKeys)
      void getMap(unsigned int key,double minMass, double maxMass, datastruct::LCMSImage & map_)
      {
        map_.setMapDescription(swathInfo_->getMapDescriptionForKey(key));

        mzRange_ = determineRange(
              msdataptr_,
              swathInfo_->getMapDescriptionForKey(key)->getIndices()
              );
        if(mzRange_.first < minMass){
          mzRange_.first = minMass;
        }
        if(mzRange_.second > maxMass){
          mzRange_.second = maxMass;
        }
        // determine vendor sampling with
        double am = determineAm(msdataptr_, swathInfo_->getMapDescriptionForKey(key)->getIndices());
        swathInfo_->getMapDescriptionForKey(key)->setAM(am);

        map_.getMapDescription()->mzRange()= mzRange_;
        size_t nrcols = swathInfo_->getMapDescriptionForKey(key)->getIndices().size();
        nrcols = rt2sum_.getNrCols(nrcols);

        size_t rows = map_.defBreak(mzRange_, ppm_ );
        map_.getImageMap().resize(rows,nrcols);
        std::cerr << "rows : [" << rows << "]  nrcols : [" << nrcols << "]" << std::endl;
        fillLCMSImage(msdataptr_,swathInfo_->getMapDescriptionForKey(key)->getIndices(), map_, am);
      }

      /// Store file on disk
      void write(const std::string & filenameOut, datastruct::LCMSImage & mmap){
        pwiz::msdata::SpectrumListPtr sl = msdataptr_->run.spectrumListPtr;

        //the filtered spectrum list ptr is injected into the msdataptr and will provide
        //access to the filtered data.
        pwiz::msdata::SpectrumListPtr mp( new FilteredSpectrumList(sl,mmap,rt2sum_) );
        msdataptr_->run.spectrumListPtr = mp;
        std::string fileOut = filenameOut;
        fileOut += "_filtered.mzML";
        pwiz::msdata::MSDataFile::write(*msdataptr_,
                                        fileOut,
                                        pwiz::msdata::MSDataFile::Format_mzML
                                        ); // uncomment if you want mzXML
      }

    private :
      void fillLCMSImage(pwiz::msdata::MSDataPtr msd,
                         const std::vector<std::size_t> & indices, //!< indices of spectra belonging to the map.
                         datastruct::LCMSImage & map_, //!< output map
                         double am //!< vendor sampling parameter
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
            if(!mz->data.empty()){
              map_.convert2dense(mz->data, intensity->data, idx, am);
            }
          }
        }
      }

      /// determines range of dataset by iterating
      /// through all spectra in indices
      /// TODO add code to compute the sampling with.
      std::pair<double, double> determineRange( pwiz::msdata::MSDataPtr msd,
                                                const std::vector<std::size_t> & indices )
      {
        double min_( std::numeric_limits<double>::max()) , max_( std::numeric_limits<double>::min());
        pwiz::msdata::SpectrumListPtr sl = msd->run.spectrumListPtr;
        if (sl.get())
        {
          std::size_t nrcols_ = indices.size();
          for( std::size_t i = 0 ; i < nrcols_ ; ++i )
          {
            pwiz::msdata::SpectrumPtr sp = sl->spectrum(indices[i],true);
            pwiz::msdata::BinaryDataArrayPtr mz;
            mz = sp->getMZArray();


            if(!mz->data.empty()){
              if(min_ > mz->data.front()) min_ = mz->data.front();
              if(max_ < mz->data.back()) max_ = mz->data.back();
            }
          }
          return std::pair<double, double>(min_,max_);
        }
        return std::pair<double, double>(0.,0.);
      }

      /// determine vendor sampling with
      double determineAm( pwiz::msdata::MSDataPtr msd,
                          const std::vector<std::size_t> & indices
                          )
      {
        pwiz::msdata::SpectrumListPtr sl = msd->run.spectrumListPtr;
        std::vector<double> am;
        if (sl.get())
        {
          base::resample::SamplingWith sw;
          std::size_t nrcols_ = indices.size();
          for( std::size_t i = 0 ; i < nrcols_ ; ++i )
          {
            pwiz::msdata::SpectrumPtr sp = sl->spectrum(indices[i],true);
            pwiz::msdata::BinaryDataArrayPtr mz;
            mz = sp->getMZArray();
            if(!mz->data.empty())
            {
              am.push_back( sw(mz->data.begin(),mz->data.end()) );
            }
          }
        }
        // determine 2% quantile (many spectra might be empty therefore go for 2% quantile).
        //std::sort(am.begin(), am.end());
        double res = *(std::min_element(am.begin(),am.end()));
        //double res = stats::quantile( am.begin(), am.end(), 0.01);
        return res;
      }// end determineAm

    };
  }
}//end namespace ralab


#endif
