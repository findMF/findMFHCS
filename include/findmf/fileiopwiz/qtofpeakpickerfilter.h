// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef QTOFPEAKPICKERFILTER_H
#define QTOFPEAKPICKERFILTER_H

#include <iostream>
#include <iterator>

#include "pwiz/data/msdata/MSDataFile.hpp"
#include "pwiz/data/msdata/SpectrumIterator.hpp"
#include "pwiz/data/msdata/SpectrumInfo.hpp"
#include "pwiz/data/msdata/SpectrumListWrapper.hpp"
#include "base/ms/peakpickerqtof.h"

namespace ralab{
  /** This is used to write the filtered spectrum list*/
  struct QTOFPeakPickerFilter : pwiz::msdata::SpectrumListWrapper{
    double resolution_;
    double smoothwidth_;
    double integrationWidth_;
    double intensityThreshold_;
    bool area_; // should area or intensities be determined
    uint32_t maxnumberofpeaks_;
    bool notCheckProfile_;

    QTOFPeakPickerFilter(
        const pwiz::msdata::SpectrumListPtr & inner, //!< spectrum list
        double resolution, //!< instrument resolution
        double smoothwidth = 2., //!< smoothwidth
        double integrationWidth = 4, //! integration width
        double intensityThreshold = 10.,
        bool area = true, //!< do you want to store are or intensity
        uint32_t maxnumberofpeaks = 0 //!< max number of peaks returned by picker
        ):SpectrumListWrapper(inner),
      resolution_(resolution),
      smoothwidth_(smoothwidth),
      integrationWidth_(integrationWidth),
      intensityThreshold_(intensityThreshold),
      area_(area),maxnumberofpeaks_(maxnumberofpeaks),notCheckProfile_(false)
    {
      // add processing methods to the copy of the inner SpectrumList's data processing
      pwiz::msdata::ProcessingMethod method;
      method.order = dp_->processingMethods.size();
      method.userParams.push_back(pwiz::msdata::UserParam("qtof peak picker"));
      if (!dp_->processingMethods.empty())
        method.softwarePtr = dp_->processingMethods[0].softwarePtr;
      dp_->processingMethods.push_back(method);
    }

    pwiz::msdata::SpectrumPtr spectrum(std::size_t index, bool getBinaryData ) const
    {
      pwiz::msdata::SpectrumPtr specptr = inner_->spectrum(index, true);
      //if (!pwiz::msdata::msLevelsToPeakPick_.contains(s->cvParam(MS_ms_level).valueAs<int>()))
      //  return specptr;
      std::vector<pwiz::msdata::CVParam>& cvParams = specptr->cvParams;
      std::vector<pwiz::msdata::CVParam>::iterator itr = std::find(cvParams.begin(), cvParams.end(), pwiz::msdata::MS_profile_spectrum);

      // return non-profile spectra as-is
      // (could have been acquired as centroid, or vendor may have done the centroiding)
      if (notCheckProfile_ || itr == cvParams.end() )
        return specptr;

      // make sure the spectrum has binary data
      if (!specptr->getMZArray().get() || !specptr->getIntensityArray().get())
        specptr = inner_->spectrum(index, true);

      // replace profile term with centroid term
      std::vector<pwiz::msdata::CVParam>& cvParams2 = specptr->cvParams;
      *(std::find(cvParams2.begin(), cvParams2.end(), pwiz::msdata::MS_profile_spectrum)) = pwiz::msdata::MS_centroid_spectrum;

      try
      {
        std::vector<double>& mzs = specptr->getMZArray()->data;
        std::vector<double>& intensities = specptr->getIntensityArray()->data;

        /// if empty spectrum return withouth processing
        if(mzs.size()==0){
          return specptr;
        }
        std::pair<double, double> range = std::make_pair(mzs.front() , mzs.back());
        //construct peak picker
        ralab::base::ms::PeakPicker<double, ralab::base::ms::SimplePeakArea > pp(
              resolution_,
              range,
              smoothwidth_,
              integrationWidth_,
              intensityThreshold_,
              area_,
              maxnumberofpeaks_
              );

        pp( mzs.begin(), mzs.end(), intensities.begin());

        mzs.assign(pp.getPeakMass().begin(),pp.getPeakMass().end());
        intensities.assign(pp.getPeakArea().begin(),
                           pp.getPeakArea().end());
        specptr->defaultArrayLength = mzs.size();

      }
      catch(std::exception& e)
      {
        throw std::runtime_error(std::string("[SpectrumList_ZeroSamplesFilter] Error filtering intensity data: ") + e.what());
      }
      specptr->dataProcessingPtr = dp_;
      return specptr;
    }
  };
}


#endif // QTOFPEAKPICKERFILTER_H

