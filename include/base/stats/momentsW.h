// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef MOMENTSW_H
#define MOMENTSW_H

#include "utilities/stats.h"

#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include "base/base/base.h"

namespace ralab
{
  namespace stats
  {

    /// Weighted average \f[ \sum(vValues \cdot vWeights) / \sum(vWeights) \f]

    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type meanW
    (
        ValIter_T valuesbeg, //!<[in] values
        ValIter_T valuesend, //
        WeightIter_T weigthsbeg//!<[in] weigths
        //WeightIter_T weightsend
        )
    {
      typedef typename std::iterator_traits<ValIter_T>::value_type TReal;
      std::ptrdiff_t x = std::distance(valuesbeg,valuesend);
      if( x <= 1 )
        {
          TReal res(1.0);
          return(res);
        }
      TReal dSumOfWeights( std::accumulate( weigthsbeg , weigthsbeg + x, TReal() ) );
      TReal dWeightedSum(std::inner_product(valuesbeg,valuesend,weigthsbeg,TReal()));
      return(dWeightedSum/dSumOfWeights);
    }

    /** Weighted Variance \f[ 1/\sum{w_i} \cdot \sum{ w_i ( x_i - \mu )^2 } \f]
      */
    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type
    varW
    (
        ValIter_T valuesbeg, //!<[in] values
        ValIter_T valuesend,
        WeightIter_T weightsbeg,
        typename std::iterator_traits<ValIter_T>::value_type dAverageMass //!<[in] precomputed weighted average
        )
    {
      typedef typename std::iterator_traits<ValIter_T>::value_type TReal;
      std::ptrdiff_t nr = std::distance(valuesbeg,valuesend);
      if (nr <= 1)
        {
          return std::numeric_limits<TReal>::quiet_NaN() ;
        }
      // sum of weights
      TReal dSumOfWeights( std::accumulate(weightsbeg, weightsbeg + nr, TReal()) );
      // computes Sum (w * (x- weightedaverage)^2)
      TReal var( std::inner_product(
                   valuesbeg,
                   valuesend,
                   weightsbeg,
                   TReal(),
                   std::plus<TReal>(),
                   utilities::DiffNthPowerWeight<TReal,2>(dAverageMass)
                   ));

      if (dSumOfWeights > 0.0)
        {
          var /= dSumOfWeights; // weighted sum of squared deviations
        }
      return var;
    }

    /// Weighted Variance \f[ 1/\sum{w_i} \cdot \sum{ w_i ( x_i - \mu )^2 } \f]
    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type
    varW
    (
        ValIter_T valuesbeg , //!<[in] Values
        ValIter_T valuesend,
        WeightIter_T weightsbeg //!<[in] Weight
        )
    {
      typedef typename std::iterator_traits<ValIter_T>::value_type TReal;
      TReal mean ( meanW( valuesbeg ,valuesend, weightsbeg ));
      return(
            varW( valuesbeg , valuesend, weightsbeg, mean ));
    }

    /// Weighted standard deviation \f[ \sqrt{var} \f]
    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type
    sdW
    (
        ValIter_T valuesbeg, //!<[in] Values
        ValIter_T valuesend, //!<[in] Weight
        WeightIter_T weightbeg
        )
    {
      return(sqrt(varW(valuesbeg , valuesend, weightbeg )));
    }

    /// Weighted standard deviation \f[ \sqrt{var} \f]
    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type
    sdW
    (
        ValIter_T valuesbeg, //!<[in] values
        ValIter_T valuesend,
        WeightIter_T weightsbeg,
        typename std::iterator_traits<ValIter_T>::value_type dAverageMass //!<[in] precomputed weighted average
        )
    {
      return(
            sqrt(varW( valuesbeg , valuesend,weightsbeg ,dAverageMass ))
            );
    }

    /// Skew
    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type
    skewW
    (
        ValIter_T valuesbeg, //!<[in] values
        ValIter_T valuesend,
        WeightIter_T weightsbeg,
        typename std::iterator_traits<ValIter_T>::value_type dAverage, //!<[in] mean
        typename std::iterator_traits<ValIter_T>::value_type dSD//!<[in] sd
        )
    {
      typedef typename std::iterator_traits<ValIter_T>::value_type TReal;

      std::ptrdiff_t nr = std::distance(valuesbeg, valuesend);
      TReal wm3( std::inner_product(
                   valuesbeg,
                   valuesend,
                   weightsbeg,
                   TReal(),
                   std::plus<TReal>(),
                   utilities::DiffNthPowerWeight<TReal,3>(dAverage)
                   ));
      TReal N( std::accumulate(weightsbeg,weightsbeg+nr,TReal() ));
      return (  wm3/( N * dSD * dSD * dSD ) );
    }

    /// Skew
    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type
    skewW
    (
        ValIter_T valuesbeg, //!<[in] values
        ValIter_T valuesend,
        WeightIter_T weightsbeg
        )
    {
      typedef typename std::iterator_traits<ValIter_T>::value_type TReal;
      TReal dAverage( meanW( valuesbeg,valuesend , weightsbeg ) );
      TReal dSD(sdW(valuesbeg,valuesend , weightsbeg,dAverage));
      return skewW(valuesbeg,valuesend , weightsbeg,dAverage, dSD);
    }

    /// Kurtosis
    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type
    kurtW
    (
        ValIter_T valuesbeg, //!<[in] values
        ValIter_T valuesend,
        WeightIter_T weightsbeg,
        typename std::iterator_traits<ValIter_T>::value_type dAverage, //!<[in] mean
        typename std::iterator_traits<ValIter_T>::value_type dSD//!<[in] sd
        )
    {
      typedef typename std::iterator_traits<ValIter_T>::value_type TReal;
      std::ptrdiff_t nr = std::distance(valuesbeg,valuesend);
      // computes \Sum(w * (x- weightedaverage)^4)
      TReal wm4( std::inner_product(
                   valuesbeg,
                   valuesend,
                   weightsbeg,
                   TReal(),
                   std::plus<TReal>(),
                   utilities::DiffNthPowerWeight<TReal,4>(dAverage)
                   ));
      TReal N( std::accumulate( weightsbeg , weightsbeg + nr , TReal() ));
      return (  wm4 /( N * dSD * dSD * dSD * dSD ) - 3 );
    }

    /*\brief Skewness (Kruemmung/Schiefe) (Sachs "Angewandte Statistic") */

    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type
    kurtW
    (
        ValIter_T valuesbeg, //!<[in] values
        ValIter_T valuesend,
        WeightIter_T weightsbeg
        )
    {
      typedef typename std::iterator_traits<ValIter_T>::value_type TReal;
      TReal dAverage( meanW( valuesbeg , valuesend , weightsbeg ) );
      TReal dSD( sdW( valuesbeg, valuesend , weightsbeg ,dAverage));
      return kurtW( valuesbeg, valuesend , weightsbeg , dAverage , dSD);
    }

    /// Weighted variance unbiased
    /// \f[ ( sum(wx^2) sum(w) - sum(wx)^2 ) / ( sum(w)^2 - sum(w^2) )  \f]
    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type
    varWUnbiased
    (
        ValIter_T valuesbeg, //!<[in] values
        ValIter_T valuesend,
        WeightIter_T weightsbeg
        )
    {
      typedef typename std::iterator_traits<ValIter_T>::value_type TReal;
      std::ptrdiff_t nr = std::distance(valuesbeg,valuesend);
      //sum(w)
      TReal sumW( std::accumulate(weightsbeg , weightsbeg+nr, TReal()) );
      //sum(w^2)
      TReal sumW2( std::inner_product( weightsbeg , weightsbeg+nr, weightsbeg,TReal()
                                       , std::plus<TReal>(),std::multiplies<TReal>() ) ) ;
      //sum(w*x)
      TReal sumWX( std::inner_product( valuesbeg , valuesend, weightsbeg,TReal()
                                       , std::plus<TReal>(),std::multiplies<TReal>() ) ) ;
      //sum(w*x*x)
      TReal sumXXW( std::inner_product(  valuesbeg , valuesend, weightsbeg,TReal()
                                        , std::plus<TReal>() , utilities::XXW<TReal>()  ) );
      TReal var = ( sumXXW * sumW - sumWX*sumWX ) / ( sumW * sumW - sumW2 );
      return ( var );
    }

    /** Weighted standard deviation unbiased (see GSL documentation)

                        \f[ \sqrt{VarUnbiasedWeight} \f]
                        */
    template<class ValIter_T, class WeightIter_T>
    typename std::iterator_traits<ValIter_T>::value_type
    sdWUnbiased
    (
        ValIter_T valuesbeg, //!<[in] values
        ValIter_T valuesend,
        WeightIter_T weightsbeg
        )
    {
      return(sqrt(varWUnbiased( valuesbeg , valuesend, weightsbeg )));
    }





  }//namespace algo
}//namespace ralab ends here



#endif // MOMENTSW_H
