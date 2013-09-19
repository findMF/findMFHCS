// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef FILTERUTILS_H
#define FILTERUTILS_H
#include <math.h>
#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>
#include <limits>

#include <boost/iterator/reverse_iterator.hpp>
#include <boost/bind.hpp>
#include <boost/math/distributions/normal.hpp>



#include "base/filter/utilities/preparedata.h"


namespace ralab
{
  namespace base
  {
    namespace filter
    {

      template <typename TIterator, typename TFilterIterator, typename TOutputIterator>
      void filter_sequence(
          TIterator  dataBeg,   //!<[in] a univariate time series.
          TIterator  dataEnd,
          TFilterIterator filterBeg, //!<[in] a vector of filter coefficients in reverse time order (as for AR or MA coefficients). Lenght of filter must be odd.
          size_t fsize,
          TOutputIterator resBeg, //!<[out] result
          bool circular = false, //!<[in]  If TRUE, wrap the filter around the ends of the series, otherwise assume external values are missing (NA).
          uint32_t sides = 2  //!<[in] currently only sides 2 supported....
          )
      {
        typedef typename std::iterator_traits<TOutputIterator>::value_type TReal;
        if((fsize-1) % 2)
          {
            throw std::logic_error("filter size must be odd");
          }
        if(!circular)
          {
            //result.assign(data.size(), std::numeric_limits<TReal>::quiet_NaN() );

            size_t offset = static_cast<size_t>(fsize/2);
            for(std::size_t i = 0 ; i< offset; ++i, ++resBeg)
              {
                *resBeg = std::numeric_limits<TReal>::quiet_NaN();
              }

            for( ; dataBeg != dataEnd - (fsize -1) ; ++dataBeg, ++resBeg )
              {
                *resBeg = (std::inner_product(dataBeg , dataBeg + fsize, filterBeg ,0. ));
              }
          }
        else
          {

            std::vector<typename std::iterator_traits<TIterator>::value_type> tmp;
            typename std::vector<typename std::iterator_traits<TIterator>::value_type>::iterator it;
            it = utilities::prepareData( dataBeg, dataEnd, fsize , tmp );

            TIterator tbegin = tmp.begin();
            TIterator tend = it;

            for( ; tbegin != tend - (fsize-1 ) ; ++tbegin, ++resBeg )
              {
                *resBeg = std::inner_product(tbegin , tbegin + fsize, filterBeg ,0. );
              }
          }
      }// filter end



      /*! \brief Applies linear convolution (filtering) to a univariate time series

            The convolution filter is  \f$ y[i] = f[1]*x[i+o] + ... + f[p]*x[i+o-(p-1)] \f$
            where o is the offset: see sides for how it is determined.

            \param sides for convolution filters only.
                        If sides=1 the filter coefficients are for past values only;
                        if sides=2 they are centred around lag 0.
                        In this case the length of the filter should be odd,
                        but if it is even, more of the filter is forward in time than backward


            */
      template <typename TContainer>
      void filter(
          const TContainer & data,   //!<[in] a univariate time series.
          const TContainer & filter, //!<[in] a vector of filter coefficients in reverse time order (as for AR or MA coefficients). Lenght of filter must be odd.
          TContainer & result, //!<[out] result
          bool circular = false, //!<[in]  If TRUE, wrap the filter around the ends of the series, otherwise assume external values are missing (NA).
          uint32_t sides = 2  //!<[in] currently only sides 2 supported....
          )
      {
        result.resize(data.size());
        filter_sequence
            (
              data.begin(),
              data.end(),
              filter.begin(),
              filter.size(),
              result.begin(),
              circular,
              sides
              );
      }// filter end
    }//filter
  }//base
}//ralab





#endif


