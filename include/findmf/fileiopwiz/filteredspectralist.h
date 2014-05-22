// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef FILTEREDSPECTRALIST_H
#define FILTEREDSPECTRALIST_H


#include <iostream>
#include <iterator>

#include "pwiz/data/msdata/MSDataFile.hpp"
#include "pwiz/data/msdata/SpectrumIterator.hpp"
#include "pwiz/data/msdata/SpectrumInfo.hpp"
#include "pwiz/data/msdata/SpectrumListWrapper.hpp"

#include "findmf/datastruct/lcmsimage.h"
#include "findmf/application/RT2sum.h"


namespace ralab{
  namespace findmf{


    /** This is used to write the filtered spectrum list*/
    struct FilteredSpectrumList : pwiz::msdata::SpectrumListWrapper{
      mutable datastruct::LCMSImage mmap_;
      RT2sum rt2sum_;
      mutable size_t ms1counter_; //!< count how many ms1 spectra you took

      FilteredSpectrumList(
          const pwiz::msdata::SpectrumListPtr& inner,
          datastruct::LCMSImage & mmap,
          const RT2sum & rt2sum = 1
          ):SpectrumListWrapper(inner),mmap_(mmap),rt2sum_(rt2sum),ms1counter_(0)
      {
        // add processing methods to the copy of the inner SpectrumList's data processing
        pwiz::msdata::ProcessingMethod method;
        method.order = dp_->processingMethods.size();
        method.userParams.push_back(pwiz::msdata::UserParam("filtered binary data"));
        if (!dp_->processingMethods.empty())
          method.softwarePtr = dp_->processingMethods[0].softwarePtr;
        dp_->processingMethods.push_back(method);
      }

      virtual pwiz::msdata::SpectrumPtr spectrum(std::size_t index, bool getBinaryData) const
      {
        pwiz::msdata::SpectrumPtr s = inner_->spectrum(index, true);

        size_t specidx = mmap_.getMapDescription()->specidx(index);
        size_t nrSpec = mmap_.getRTsize();
        if(specidx < nrSpec){
          try
          {
            std::vector<double>& mzs = s->getMZArray()->data;
            std::vector<double>& intensities = s->getIntensityArray()->data;
            std::vector<double> filteredMZs, filteredIntensities;
            //this deletes all binary data.
            size_t idx = rt2sum_.getCols(index);
            mmap_.getSpectrum( ms1counter_ , filteredMZs , filteredIntensities );
            mzs.swap(filteredMZs);
            intensities.swap(filteredIntensities);
            s->defaultArrayLength = mzs.size();

            //this is actually a hack
            ++ms1counter_;
          }
          catch(std::exception& e)
          {
            throw std::runtime_error(std::string("[SpectrumList_ZeroSamplesFilter] Error filtering intensity data: ") + e.what());
          }
        }
        s->dataProcessingPtr = dp_;
        return s;
      }
    };
  }
}

#endif // FILTEREDSPECTRALIST_H
