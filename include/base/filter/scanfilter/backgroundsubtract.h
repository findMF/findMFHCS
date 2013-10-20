// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef BACKGROUNDSUBTRACT_H
#define BACKGROUNDSUBTRACT_H

#include "base/filter/scanfilter/iscanfilterfunctor.h"
#include "base/filter/morphology.h"

/*! \file BackgroundSubtract.h Background-subtraction using the top hat method.
*/

namespace ralab
{
  namespace base
  {
    namespace filter
    {
      namespace scanfilter
      {

        /*!\brief Background Subtraction for vectors.
      \tparam base type
    */
        template < typename TReal >
        struct BackgroundSubtract : IScanFilter< TReal >
        {
          uint32_t pwm_;
          OpenMS::MorphologicalFilter<TReal> morphfilter_;
          std::vector<TReal> worker_;
           BackgroundSubtract
          (
              uint32_t pwm = 5 //!<
              ):
            pwm_(pwm),morphfilter_()
          {}

         BackgroundSubtract
          (
              const BackgroundSubtract & rhs //!<
              ):
            pwm_(rhs.pwm_)
          ,morphfilter_()
          {}


          int32_t operator()
          (
              std::vector<TReal> & rt //!< data
              )
          {
            worker_.assign( rt.begin(),rt.end() );
            morphfilter_.tophat(worker_.begin(),worker_.end(),rt.begin(),pwm_);
            return 0;
          }//end operator

          int32_t operator()
          (
              const std::vector<TReal> & rt //!< data
              ,std::vector<TReal> & rtfiltered //!< filtered signal
              )
          {
            morphfilter_.tophat(rt.begin(),rt.end(),rtfiltered.begin(),pwm_);
            return 0;
          }//end operator
        };

      }
    }

  } // end namespace
} // end namespace ralab


#endif // BACKGROUNDSUBTRACT_H
