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

      //returns ms1 map
      void getMS1Map( datastruct::LCMSImage & image ){
        //for ms1 maps we do not limit the mass range.
        getMap( 0 , 0., std::numeric_limits<double>::max(), image);
      }

      /// returns ms2 map for key
      void getMap(unsigned int key,double minMass, double maxMass, datastruct::LCMSImage & map_)
      {
        map_.setMapDescription(swathInfo_->getMapForKey(key));
        boost::timer time;
        mzRange_ = determineRange(
              msdataptr_,
              swathInfo_->getMapForKey(key)->getIndices()
              );
        if(mzRange_.first < minMass){
          mzRange_.first = minMass;
        }
        if(mzRange_.second > maxMass){
          mzRange_.second = maxMass;
        }
        map_.getMapDescription()->mzRange_= mzRange_;
        size_t nrcols = swathInfo_->getMapForKey(key)->getIndices().size();
        nrcols = rt2sum_.getNrCols(nrcols);

        time.restart();

        size_t rows = map_.defBreak(mzRange_, ppm_ );
        map_.getImageMap().resize(rows,nrcols);
        std::cerr << "rows : [" << rows << "]  nrcols : [" << nrcols << "]" << std::endl;

        fillLCMSImage(msdataptr_,swathInfo_->getMapForKey(key)->getIndices(),map_);
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
      void fillLCMSImage(pwiz::msdata::MSDataPtr msd,
                         const std::vector<std::size_t> & indices, //!< indices of spectra belonging to the map.
                         datastruct::LCMSImage & map_ //!< output map.
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
              map_.convert2dense(mz->data, intensity->data, idx);
            }
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
            if(!mz->data.empty()){
              if(min_ > mz->data.front()) min_ = mz->data.front();
              if(max_ < mz->data.back()) max_ = mz->data.back();
            }
          }
          return std::pair<double, double>(min_,max_);
        }
        return std::pair<double, double>(0.,0.);
      }

    };
  }
}//end namespace ralab


#endif
