#ifndef SCALE_H
#define SCALE_H

#include <math.h>
#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>
#include <iterator>

#include "base/stats/stats.h"

/*! \file Scale.h
\brief statistic functions for package base: scale(x, center = TRUE, scale = TRUE)

\ingroup BASE
*/

namespace ralab
{
  namespace base
  {
    namespace stats
    {


      /*! \defgroup SCALE scale - centers and/or scales the columns of a numeric matrix.

                        -# The value of <b>center</b> determines how column centering is performed. If center is a numeric vector with length equal to the number of columns of x, then each column of x has the corresponding value from center subtracted from it. If center is TRUE then centering is done by subtracting the column means (omitting NAs) of x from their corresponding columns, and if center is FALSE, no centering is done.
                        -# The value of <b>scale</b> determines how column scaling is performed (after centering). If scale is a numeric vector with length equal to the number of columns of x, then each column of x is divided by the corresponding value from scale. If scale is TRUE then scaling is done by dividing the (centered) columns of x by their root-mean-square, and if scale is FALSE, no scaling is done.
                        -# The root-mean-square for a column is obtained by computing the square-root of the sum-of-squares of the non-missing values in the column divided by the number of non-missing values minus one.

                        \ingroup BASE
                        @{
                        */
      template<int N, typename TReal>
      struct NthPower : std::unary_function<TReal,TReal>
      {
        TReal operator()(const TReal & x)
        {
          TReal ret = x;
          for (int i=1; i < N; ++i) {
              ret *= x;
            }
          return ret;
        }
      };


      /*!\brief The root-mean-square for a column is obtained by computing the square-root of the sum-of-squares of the non-missing values in the column divided by the number of non-missing values minus one.  */
      template<typename InputIterator>
      typename std::iterator_traits<InputIterator>::value_type
      rootMeanSquare(
          const InputIterator begin, //!< [in] start iterator
          const InputIterator end //!< [in] end iterator
          )
      {
        typedef typename std::iterator_traits<InputIterator>::value_type TReal;
        std::vector<TReal> x(begin,end);

        std::transform( x.begin(), x.end(), x.begin(), NthPower<2,TReal>() ); //first sqaure all elements
        TReal sum = std::accumulate(x.begin(), x.end() , TReal(0.));
        sum = sum/static_cast<TReal>(x.size() - size_t(1));
        return(sqrt(sum));
      }


      /*! \brief scale centers and/or scales all values from begin in to end.  */
      template<typename InputIterator>
      void scale(
          InputIterator begin, //!< [in] start iterator
          InputIterator end, //!< [in] end iterator
          std::pair<typename std::iterator_traits<InputIterator>::value_type,typename std::iterator_traits<InputIterator>::value_type> & scaled, //!<[out] scaled.first = center, scaled.second = scale
          bool center = true,//!<[in] either a logical value or a numeric vector of length equal to the number of columns of x.
          bool scale = true //!<[in] 	either a logical value or a numeric vector of length equal to the number of columns of x.
          )
      {
        typedef typename std::iterator_traits<InputIterator>::value_type TReal;
        std::vector<TReal> tmp;

        if(center)
          {
            scaled.first = ralab::base::base::mean( begin , end);
            std::transform(begin, end, begin, std::bind2nd( std::minus<TReal>(), scaled.first));
          }
        else
          {
            scaled.first = std::numeric_limits<TReal>::quiet_NaN();
          }
        if(scale)
          {
            scaled.second = rootMeanSquare( begin , end );
            std::transform(begin, end, begin , std::bind2nd(std::divides<TReal>(), scaled.second) );
          }
        else
          {
            scaled.second = std::numeric_limits<TReal>::quiet_NaN();
          }
      }
      /*!@}*/

    }
  }
}
#endif


