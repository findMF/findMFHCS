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

#include "base/ms/peakpicker.h"

namespace ralab{
  /** This is used to write the filtered spectrum list*/
  struct QTOFPeakPickerFilter : pwiz::msdata::SpectrumListWrapper{
    double resolution_;
    double smoothwidth_;
    uint32_t integrationWidth_;
    bool area_;
    bool storestate_;

    QTOFPeakPickerFilter(
        const pwiz::msdata::SpectrumListPtr & inner, //!< spectrum list
        double resolution, //!< instrument resolution
        double smoothwidth = 2., //!< smoothwidth
        uint32_t integrationWidth = 4, //! integration width
        bool area = true, //!< do you want to store are or intensity
        bool storestate = false //!< do you want to serialize pp state for debugging.
        ):SpectrumListWrapper(inner),
      resolution_(resolution),
      smoothwidth_(smoothwidth),
      integrationWidth_(integrationWidth),
      area_(area),
      storestate_(storestate)
    {
      // add processing methods to the copy of the inner SpectrumList's data processing
      pwiz::msdata::ProcessingMethod method;
      method.order = dp_->processingMethods.size();
      method.userParams.push_back(pwiz::msdata::UserParam("qtof peak picker"));
      if (!dp_->processingMethods.empty())
        method.softwarePtr = dp_->processingMethods[0].softwarePtr;
      dp_->processingMethods.push_back(method);
    }

    pwiz::msdata::SpectrumPtr spectrum( std::size_t index,
                                                bool getBinaryData ) const
    {
      pwiz::msdata::SpectrumPtr specptr = inner_->spectrum(index, true);
      try
      {

        std::vector<double>& mzs = specptr->getMZArray()->data;
        std::vector<double>& intensities = specptr->getIntensityArray()->data;


        std::pair<double, double> range = std::make_pair(mzs.front(),mzs.back());
        ralab::base::ms::PeakPicker<double, ralab::base::ms::SimplePeakArea > pp(
              resolution_,
              range,
              smoothwidth_,
              integrationWidth_
              );

        // std::vector<double> pickedMZs, pickedIntensities, area;
        // this deletes all binary data.

        pp( mzs.begin(), mzs.end(), intensities.begin());


        if(storestate_){
            mzs.assign(pp.getResampledMZ().begin(),pp.getResampledMZ().end());
            if(area_){
                intensities.assign(pp.getResampledIntensity().begin(),
                                   pp.getResampledIntensity().end());
              }else{
                intensities.assign(pp.getSmoothedIntensity().begin(),pp.getSmoothedIntensity().end());
              }
          }else{
            mzs.assign(pp.getPeakMass().begin(),pp.getPeakMass().end());
            if(area_){
                intensities.assign(pp.getPeakArea().begin(),
                                   pp.getPeakArea().end());
              }else{
                intensities.assign( pp.getPeakIntensity().begin() , pp.getPeakIntensity().end() );
              }
          }

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
//if(false)
//  {
//    std::string mzsf = "mz";
//    mzsf += boost::lexical_cast<std::string>(index);
//    mzsf += ".bin";
//    std::string intf = "int";
//    intf += boost::lexical_cast<std::string>(index);
//    intf += ".bin";

//    ralab::base::utils::writeBin(mzs,mzsf);
//    ralab::base::utils::writeBin(intensities,intf);

//    std::string file = "x" ;
//    file += boost::lexical_cast<std::string>(index);
//    pp.write(file);
//  }
