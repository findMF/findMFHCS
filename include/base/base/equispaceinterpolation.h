// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef EQUISPACEINTERPOLATION_H
#define EQUISPACEINTERPOLATION_H

#include <utility>
#include <limits>
#include <vector>
#include <assert.h>

#include "base/base/constants.h"
#include "base/base/utilities/interpolation.h"

/*!\file equispaceInterpolation.h

Interpolation on a equidistantly spaced grid.
*/

/*! EQUISPACEINTERPOL Interpolation on a equidistantly spaced grid.

\brief Interpolation on a equidistantly spaced grid. The Values y are located at a grid [0,1,..., len(y)].

For equidistantly spaced y at a grid [0,1,..., len(y)] the \f$ x_{i}\f$ and \f$x_{i+1}\f$ enclosing \f$x_{out}\f$
can be found efficiently by i = floor(xout). Furthermore for \f$ x_{i+1} - x_{i} = 1 \f$  the interpolation formulas are simple.
Therefore, w provided this specialized implementation of interpolation functions.

Each segment (bounded by two data points) can be interpolated independently.
The parameter mu defines where to estimate the value on the interpolated line, it is 0 at the first point and 1 and the second point.
For interpolated values between the two points mu ranges between 0 and 1. Values of mu outside this range result in extrapolation.
This convention is followed for all the subsequent methods below.


*/

namespace ralab
{
  namespace base
  {
    namespace base
    {
      /*!
                        \brief affine interpolation on equidistantly spaced y. The y's are located at 0,1,2....,len(y).

                        For x's < 0 or x's > len(y) y[0] or y[len(y) -1 ] is used.

                        */
      template <typename TReal>
      void interpolate_linear
      (
          const std::vector<TReal> & y, //!< y values equidistantly spaced. spacing is [0,1,2, .... ,len(y)]
          const std::vector<TReal> & xout, //!< points to interpolate at
          std::vector<TReal> & out, //!< interpolated values, same length as x.
          int start_index = 0, //!< if y values are placed on a grid with start_index != 0
          TReal epsilon = std::numeric_limits<TReal>::epsilon()
          )
      {
        utilities::LinearInterpolate<TReal> functor(epsilon);
        utilities::interpolate_LC_worker(y,xout,out,functor, start_index);
      }// end interpolate cubic


      template <typename YInputIterator,
                typename XInputIterator,
                typename OutputIterator>
      void interpolate_linear(
          YInputIterator begY, //!< y values equidistantly spaced. spacing is [0,1,2, .... ,len(y)]
          YInputIterator endY,
          XInputIterator begX, //!< points to interpolate at
          XInputIterator endX,
          OutputIterator out, //!< interpolated values, same length as x.
          int start_index = 0, //!< if y values are placed on a grid with start_index != 0
          typename std::iterator_traits<OutputIterator>::value_type epsilon =
          std::numeric_limits<typename std::iterator_traits<OutputIterator>::value_type>::epsilon()
          )
      {
        typedef typename std::iterator_traits<OutputIterator>::value_type TReal;
        utilities::LinearInterpolate<TReal> functor(epsilon);
        utilities::interpolate_LC_worker(begY , endY , begX , endX , out , functor , start_index);
      }// end interpolate cubic


      /*!
                        \brief cosine interpolation on equidistantly spaced y. The y's are located at 0,1,2....,len(y).

                        For x's < 0 or x's > len(y) y[0] or y[len(y) -1 ] is used.

                        */
      template <typename TReal>
      void interpolate_cosine(
          const std::vector<TReal> & y, //!< y values equidistantly spaced. spacing is [0,1,2, .... ,len(y)]
          const std::vector<TReal> & xout, //!< points to interpolate at
          std::vector<TReal> & out, //!< interpolated values, same length as x.
          int start_index = 0 //!< if y values are placed on a grid with start_index != 0
          )
      {
        utilities::CosineInterpolate<TReal> functor;
        utilities::interpolate_LC_worker(y,xout,out,functor,start_index);
      }// end interpolate cubic


      template <typename YInputIterator,
                typename XInputIterator,
                typename OutputIterator
                >
      void interpolate_cosine(
          YInputIterator begY, //!< y values equidistantly spaced. spacing is [0,1,2, .... ,len(y)]
          YInputIterator endY,
          XInputIterator begX, //!< points to interpolate at
          XInputIterator endX,
          OutputIterator out, //!< interpolated values, same length as x.
          int start_index = 0 //!< if y values are placed on a grid with start_index != 0
          )
      {
        typedef typename std::iterator_traits<OutputIterator>::value_type TReal;
        utilities::CosineInterpolate<TReal> functor;
        utilities::interpolate_LC_worker(begY,endY ,begX,endX, out,functor, start_index);
      }// end interpolate cubic

      /*!
                        \brief cubic interpolation on equidistantly spaced y's. The y's are located at 0,1,2....,len(y).

                        For x's < 0 or x's > len(y) y[0] or y[len(y) -1 ] is used.

                        */
      template <typename TReal>
      void interpolate_cubic(
          const std::vector<TReal> & y, //!< y values equidistantly spaced. spacing is [0,1,2, .... ,len(y)]
          const std::vector<TReal> & xout, //!< points to interpolate at
          std::vector<TReal> & out, //!< interpolated values, same length as x.
          int start_index = 0, //!< if y values are placed on a grid with start_index != 0
          TReal epsilon = std::numeric_limits<TReal>::epsilon()
          )
      {
        utilities::CubicInterpolate<TReal> functor(epsilon);
        utilities::interpolate_CH_worker(y,xout,out,functor,start_index);
      }// end interpolate cubic


      template <typename YInputIterator,
                typename XInputIterator,
                typename OutputIterator>
      void interpolate_cubic(
          YInputIterator begY, //!< y values equidistantly spaced. spacing is [0,1,2, .... ,len(y)]
          YInputIterator endY,
          XInputIterator begX, //!< points to interpolate at
          XInputIterator endX,
          OutputIterator out, //!< interpolated values, same length as x.
          int start_index = 0, //!< if y values are placed on a grid with start_index != 0
          typename std::iterator_traits<OutputIterator>::value_type epsilon =
          std::numeric_limits<typename std::iterator_traits<OutputIterator>::value_type>::epsilon()
          )
      {
        typedef typename std::iterator_traits<OutputIterator>::value_type TReal;
        utilities::CubicInterpolate<TReal> functor(epsilon);
        utilities::interpolate_CH_worker(begY,endY ,begX,endX, out,functor, start_index);
      }// end interpolate cubic

      /*!
                        \brief Hermite interpolation on equidistantly spaced y's. The y's are located at 0,1,2....,len(y).

                        For x's < 0 or x's > len(y) y[0] or y[len(y) -1 ] is used.

                        */
      template <typename TReal>
      void interpolate_Hermite
      (
          const std::vector<TReal> & y, //!< y values equidistantly spaced. spacing is [0,1,2, .... ,len(y)]
          const std::vector<TReal> & xout, //!< points to interpolate at
          std::vector<TReal> & out, //!< interpolated values, same length as x.
          TReal tension = 0, //!< 1 is high, 0 normal, -1 is low
          TReal bias = 0, //!< 0 is even, positive is towards first segment, negative towards the other
          int start_index = 0, //!< if y values are placed on a grid with start_index != 0
          TReal epsilon = std::numeric_limits<TReal>::epsilon()
          )
      {
        utilities::HermiteInterpolate<TReal> functor(tension, bias, epsilon);
        utilities::interpolate_CH_worker(y,xout,out,functor,start_index);
      }// end interpolate cubic


      template <
          typename YInputIterator,
          typename XInputIterator,
          typename OutputIterator
          >
      void interpolate_Hermite(
          YInputIterator begY, //!< y values equidistantly spaced. spacing is [0,1,2, .... ,len(y)]
          YInputIterator endY,
          XInputIterator begX, //!< points to interpolate at
          XInputIterator endX,
          OutputIterator out, //!< interpolated values, same length as x.
          double tension = 0, //!< 1 is high, 0 normal, -1 is low
          double bias = 0, //!< 0 is even, positive is towards first segment, negative towards the other
          int start_index = 0, //!< if y values are placed on a grid with start_index != 0
          typename std::iterator_traits<OutputIterator>::value_type epsilon =
          std::numeric_limits<typename std::iterator_traits<OutputIterator>::value_type>::epsilon()
          )
      {
        typedef typename std::iterator_traits<OutputIterator>::value_type TReal;
        utilities::HermiteInterpolate<TReal> functor(tension, bias, epsilon);
        utilities::interpolate_CH_worker( begY , endY , begX , endX , out , functor , start_index );
      }// end interpolate cubic
    }//base
  }//namespace base
}//namespace ralab


#endif // EQUISPACEINTERPOLATION_H
