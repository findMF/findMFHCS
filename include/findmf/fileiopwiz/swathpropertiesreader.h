// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SWATHPROPERTIESREADER_H
#define SWATHPROPERTIESREADER_H

#include <pwiz/data/msdata/MSDataFile.hpp>
#include <pwiz/data/msdata/SpectrumIterator.hpp>
#include <pwiz/data/msdata/SpectrumInfo.hpp>


#include "findmf/datastruct/msfileinfo.h"

namespace ralab{
  namespace findmf{

    //reads swath properties from an mzMLFIle
    struct SwathPropertiesReader{
    private:
      datastruct::MSFileInfoPtr swathinfo_;
      pwiz::msdata::MSDataPtr msdataptr_;

    public:
      SwathPropertiesReader(std::string filename)
        :swathinfo_(new datastruct::MSFileInfo(filename))
      {
        msdataptr_ = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(filename));
        readProperties(msdataptr_);
      }

      /// ms data ptr
      pwiz::msdata::MSDataPtr getMSData(){
        return msdataptr_;
      }

      /// if file contains many maps return keys of the maps.
      /// the keys are the minmass of the map extraction window.
      void getKeys(std::vector<std::size_t> & keys){
        swathinfo_->getKeys(keys);
      }

      ///
      datastruct::MSFileInfoPtr getSwathInfo(){
        return swathinfo_;
      }

    private:
      /// method to create swath file layout
      void readProperties(pwiz::msdata::MSDataPtr msdataptr_)
      {
        pwiz::msdata::SpectrumListPtr sl = msdataptr_->run.spectrumListPtr;
        if (sl.get())
          {
            std::cerr << "# of spectra: " << sl->size() << std::cout ;
            pwiz::msdata::SpectrumIterator::Config c;
            c.getBinaryData = false;
            pwiz::msdata::SpectrumIterator it(*sl,c);
            pwiz::msdata::SpectrumIterator end;
            int specIndex = 0;
            for(; it !=end ; ++it, ++specIndex){
                pwiz::msdata::SpectrumInfo specInf(*it);
                pwiz::msdata::Spectrum s = *it;


                if(specInf.msLevel == 1){
                    datastruct::MapLCMSDescriptionPtr map = swathinfo_->getMapDescriptionForKey(0);
                    map->getIndices().push_back(specIndex);
                    //map.rt_.push_back(specInf.retentionTime);
                    map->mslevel_ = 1;
                    map->rtRange_.first = std::min(map->rtRange_.first, specInf.retentionTime);
                    map->rtRange_.second = std::max(map->rtRange_.second, specInf.retentionTime);
                    map->mzRange_.first = std::min(map->mzRange_.first, specInf.mzLow);
                    map->mzRange_.second = std::max(map->mzRange_.second, specInf.mzHigh);

                  }
                else if(specInf.msLevel == 2){
                    if( specInf.precursors.size() > 0 )
                      {
                        double p1=0,p2=0;
                        pwiz::msdata::IsolationWindow isw = s.precursors[0].isolationWindow;
                        if(isw.cvParams.size() == 2){
                            p1 = boost::lexical_cast<double>(isw.cvParams[0].value);
                            p2 = boost::lexical_cast<double>(isw.cvParams[1].value);
                          }

                       datastruct::MapLCMSDescriptionPtr map = swathinfo_->getMapDescriptionForKey(specInf.precursors[0].mz);

                        map->mslevel_ = 2;
                        map->extractionWindowMZ_.first = specInf.precursors[0].mz - p1;
                        map->extractionWindowMZ_.second = specInf.precursors[0].mz + p2;

                        map->getIndices().push_back(specIndex);

                        // TODO - there is an issue with missing spectra in the AB Sciex API.
                        // map->getRT().push_back(specInf.retentionTime);

                        map->rtRange_.first = std::min(map->rtRange_.first, specInf.retentionTime);
                        map->rtRange_.second = std::max(map->rtRange_.second, specInf.retentionTime);
                        map->mzRange_.first = std::min(map->mzRange_.first, specInf.mzLow);
                        map->mzRange_.second = std::max(map->mzRange_.second, specInf.mzHigh);
                      }
                  }
                else
                  {
                    std::cerr << "unhandled ms level" << std::endl;
                  }

              }
            std::cout << specIndex << std::endl;

          }
      }
    };
  }
}


#endif // SWATHPROPERTIESREADER_H
