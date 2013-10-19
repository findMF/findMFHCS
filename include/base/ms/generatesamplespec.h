// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef GENERATESAMPLESPEC
#define GENERATESAMPLESPEC

#include <algorithm>
#include <boost/bind.hpp>
#include <vector>
#include <boost/assign.hpp>
#include "base/filter/gaussfilter.h"
#include "base/stats/uniform.h"


namespace ralab{
  namespace base{
    namespace ms{
      template<typename TReal>
      int generateSampleSpec
      (
          size_t size, //!< size
          TReal fwhm, //!< full with of peaks
          size_t nr_peaks, //!< nr peaks to simulate
          std::vector<TReal> & res//!< [out]
          )
      {
        //Get Gauss Filter
        std::vector<TReal> gauss, gaussp;
        ralab::base::filter::getGaussianFilter( gauss ,fwhm);
        size_t gwith = static_cast<int>( gauss.size() / 2 );
        std::cout << gauss.size() << std::endl;
        res.assign(size,0.);
        ralab::base::stats::Uniform<TReal> uniform(0,size);
        ralab::base::stats::Uniform<TReal> uniform2(3000,4000, 100);

        for(unsigned int i = 0 ; i < nr_peaks ; ++i)
          {
            size_t location = static_cast<size_t> (uniform()); // sample peak location

            size_t start = 0;
            if(location > gwith)
              start = location - gwith;

            size_t end = location + gwith;
            end = std::min<size_t>( size , end );
            BOOST_ASSERT( start < end );
            gaussp.assign( gauss.begin() , gauss.end() );
            TReal x = uniform2();
            std::transform(gaussp.begin(),gaussp.end(),gaussp.begin(),boost::bind(std::multiplies<TReal>(), _1 , x));

            std::transform(
                  res.begin() + start ,
                  res.begin() + end,
                  gaussp.begin() ,
                  res.begin() + start ,
                  std::plus<TReal>() );
          }
        return 0;
      }

    }
  }
}

#endif
