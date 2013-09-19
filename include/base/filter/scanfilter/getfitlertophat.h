// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef __FILTER
#define __FILTER


#include "base/filter/scanfilter/backgroundsubtract.h"

namespace ralab{
  namespace base{
    namespace filter{
      namespace scanfilter{

        /*Background subtraction filter utilities
        */
        inline IScanFilterFunctorFloatPtr getFilterTOPHAT(
            float withRT, //!< with of chromatographic peaks
            float factor = 2.5 //!< by how much to increase
            )
        {
          IScanFilterFunctorFloatPtr bbss(
                new ralab::base::filter::scanfilter::BackgroundSubtract<float>(static_cast<unsigned int>(withRT * factor)));
           return bbss;
        }

      }
    }
  }

 }
#endif
