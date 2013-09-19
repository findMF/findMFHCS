// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SCANFILTERPIPELINE_H
#define SCANFILTERPIPELINE_H

#include "base/filter/scanfilter/iscanfilterfunctor.h"
#include <boost/shared_ptr.hpp>

/*! \file ScanFilterPipelineFunctor.h methods for filtering HDF5 profile datasets using Gaussian filters

\ingroup FILTERFUNCTORS
*/

namespace ralab
{
  namespace algo
  {

    /*!\brief Scan Filter Pipeline functor

                \tparam base type

                */
    template < typename TReal >
    struct ScanFilterPipelineFunctor : IScanFilterFunctor< TReal >
    {
      typedef std::vector< boost::shared_ptr<IScanFilterFunctor< TReal > > > Pipeline;
      Pipeline pipeline_  ;
      std::vector<TReal> rworker_;

      ScanFilterPipelineFunctor
      (
          ):pipeline_(0),rworker_()
      {}

      /*
                        !\brief constructor
                        */
      explicit ScanFilterPipelineFunctor
      (
          const Pipeline & pipeline //!<
          ):pipeline_(),rworker_()
      {
        for(unsigned int i = 0 ; i < pipeline.size(); ++i)
          {
            pipeline_.push_back(pipeline[i]);
          }
      }

      /*!\brief Copy constructor

                        */
      ScanFilterPipelineFunctor
      (
          const ScanFilterPipelineFunctor & scanpipeline //!< FWHM of Gaussian
          ):pipeline_(),rworker_()
      {
        for(unsigned int i = 0 ; i < scanpipeline.pipeline_.size(); ++i)
          {
            pipeline_.push_back(scanpipeline.pipeline_[i]->clone());
          }
      }

      boost::shared_ptr<IScanFilterFunctor<TReal > > clone()
      {
        return boost::shared_ptr<IScanFilterFunctor <TReal > >(
              new ScanFilterPipelineFunctor<TReal>(*this)
              );
      }
      /*!\brief in place filtering

                        \return if >= 0 than OK. Error otherwise.
                        */
      int32_t operator()
      (
          std::vector<TReal> & rt
          )
      {
        uint32_t err(0);
        for(uint32_t i = 0 ; i < pipeline_.size(); ++i )
          {
            //Probably performance could be improved by using
            //the 2 param version of the operator...
            if( 0>(err = pipeline_[i]->operator()(rt) ) )
              {
                return err;
              }
          }
        return err;
      }//end operator

      /*!\brief filter rt

                        \return if >= 0 than OK. Error otherwise.
                        */
      int32_t operator()
      (
          const std::vector<TReal> & rt
          ,std::vector<TReal> & rtfiltered
          )
      {
        uint32_t err(0);
        rtfiltered.assign(rt.begin(),rt.end());
        for(uint32_t i = 0 ; i < pipeline_.size(); ++i )
          {
            rworker_.assign( rtfiltered.begin() , rtfiltered.end() );
            if(0> (err = pipeline_[i]->operator()(rworker_ , rtfiltered) ) )
              {
                return err;
              }
          }
        return err;
      }//end operator
    };

  } // end namespace
} // end namespace ralab



#endif // SCANFILTERPIPELINE_H
