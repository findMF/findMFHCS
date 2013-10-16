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

    /** Weighted average \f[ \sum(vValues \cdot vWeights) / \sum(vWeights) \f] */
    template< typename TReal >
    const TReal
    meanW
    (
        const std::vector<TReal> & vValues , //!<[in] values
        const std::vector<TReal> & vWeights //!<[in] weights
        )
    {
      if( vValues.size() <= 1 )
        {
          TReal res(1.0);
          return(res);
        }
      TReal dSumOfWeights( std::accumulate( vWeights.begin() , vWeights.end(), TReal() ) );
      TReal dWeightedSum(std::inner_product(vValues.begin(),vValues.end(),vWeights.begin(),TReal()));
      return(dWeightedSum/dSumOfWeights);
    }

    /** Weighted Variance \f[ 1/\sum{w_i} \cdot \sum{ w_i ( x_i - \mu )^2 } \f]
      */
    template< typename TReal >
    const TReal
    varW
    (
        const std::vector<TReal> & vValues , //!<[in] values
        const std::vector<TReal> & vWeights, //!<[in] weights
        const TReal dAverageMass //!<[in] precomputed weighted average
        )
    {
      if (vValues.size() <= 1)
        {
          return std::numeric_limits<TReal>::quiet_NaN() ;
        }
      // sum of weights
      TReal dSumOfWeights( std::accumulate(vWeights.begin(), vWeights.end(), TReal()) );

      // computes Sum (w * (x- weightedaverage)^2)
      TReal var( std::inner_product(
                   vValues.begin(),
                   vValues.end(),
                   vWeights.begin(),
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

    /** Weighted Variance \f[ 1/\sum{w_i} \cdot \sum{ w_i ( x_i - \mu )^2 } \f] */
    template<typename TReal>
    const TReal
    varW
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights //!<[in] Weight
        )
    {
      TReal mean ( meanW<TReal>( vValues , vWeights ));
      return(
            varW<TReal>( vValues , vWeights, mean ));
    }
    /** Weighted standard deviation \f[ \sqrt{var} \f] */
    template<typename TReal>
    const TReal
    sdW
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights //!<[in] Weight
        )
    {
      return(
            sqrt(varW(vValues , vWeights ))
            );
    }

    /** Weighted standard deviation \f[ \sqrt{var} \f] */
    template<typename TReal>
    const TReal
    sdW
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights , //!<[in] Weight
        const TReal mean //!<[in]
        )
    {
      return(
            sqrt(varW( vValues , vWeights ,mean ))
            );
    }

    /** Skew */
    template<typename TReal>
    const TReal skewW
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights , //!<[in] Weight
        const TReal dAverage, //!<[in] mean
        const TReal dSD//!<[in] sd
        )
    {

      TReal wm3( std::inner_product(
                   vValues.begin(),
                   vValues.end(),
                   vWeights.begin(),
                   TReal(),
                   std::plus<TReal>(),
                   utilities::DiffNthPowerWeight<TReal,3>(dAverage)
                   ));
      TReal N( std::accumulate(vWeights.begin(),vWeights.end(),TReal() ));
      return (  wm3/( N * dSD * dSD * dSD ) );
    }

    /** Skew */
    template<typename TReal>
    const TReal skewW
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights  //!<[in] Weight
        )
    {
      TReal dAverage( meanW<TReal>( vValues , vWeights ) );
      TReal dSD(sdW(vValues,vWeights,dAverage));
      return skewW(vValues,vWeights,dAverage, dSD);
    }

    /** brief Kurtosis   */
    template<typename TReal>
    const TReal kurtW
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights , //!<[in] Weight
        const TReal dAverage, //!<[in] mean
        const TReal dSD//!<[in] sd
        )
    {
      // computes \Sum(w * (x- weightedaverage)^4)
      TReal wm4( std::inner_product(
                   vValues.begin(),
                   vValues.end(),
                   vWeights.begin(),
                   TReal(),
                   std::plus<TReal>(),
                   utilities::DiffNthPowerWeight<TReal,4>(dAverage)
                   ));
      TReal N( std::accumulate( vWeights.begin() , vWeights.end() , TReal() ));
      return (  wm4 /( N * dSD * dSD * dSD * dSD ) - 3 );
    }

    /*\brief Skewness (Kruemmung/Schiefe) (Sachs "Angewandte Statistic") */

    template<typename TReal>
    const TReal kurtW
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights  //!<[in] Weight
        )
    {
      TReal dAverage( meanW<TReal>( vValues , vWeights ) );
      TReal dSD(sdW(vValues,vWeights,dAverage));
      return kurtW(vValues , vWeights , dAverage , dSD);
    }

    /** Weighted variance unbiased
     *
                        \f[ ( sum(wx^2) sum(w) - sum(wx)^2 ) / ( sum(w)^2 - sum(w^2) )  \f]
                        */
    template<typename TReal>
    const TReal
    varWUnbiased
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights  //!<[in] Weight
        )
    {
      //sum(w)
      TReal sumW( std::accumulate(vWeights.begin() , vWeights.end(), TReal()) );
      //sum(w^2)
      TReal sumW2( std::inner_product( vWeights.begin() , vWeights.end(), vWeights.begin(),TReal()
                                       , std::plus<TReal>(),std::multiplies<TReal>() ) ) ;
      //sum(w*x)
      TReal sumWX( std::inner_product( vValues.begin() , vValues.end(), vWeights.begin(),TReal()
                                       , std::plus<TReal>(),std::multiplies<TReal>() ) ) ;
      //sum(w*x*x)
      TReal sumXXW( std::inner_product( vValues.begin() , vValues.end(), vWeights.begin(),TReal()
                                        , std::plus<TReal>() , utilities::XXW<TReal>()  ) );
      TReal var = ( sumXXW * sumW - sumWX*sumWX ) / ( sumW * sumW - sumW2 );
      return ( var );
    }

    /** Weighted standard deviation unbiased (see GSL documentation)

                        \f[ \sqrt{VarUnbiasedWeight} \f]
                        */
    template<typename TReal>
    const TReal
    sdWUnbiased
    (
        const std::vector<TReal> & values, //!<[in] Values
        const std::vector<TReal> & weights  //!<[in] Weight
        )
    {
      return(sqrt(varWUnbiased( values , weights )));
    }



    /** Median Value

                        Compute the sample median.
                        */
    template<typename RanIt >
    typename std::iterator_traits<RanIt>::value_type median
    (
        RanIt begin, //!< begin iterator
        RanIt end //!< end iterator
        )
    {
      typedef typename std::iterator_traits<RanIt>::value_type TReal;
      typedef typename std::iterator_traits<RanIt>::difference_type TSize;
      TReal median1, median2;
      TSize count = std::distance(begin, end);
      // even case
      if (count % 2 == 0)
        {
          if (0 == count)
            {
              return 0.0;
            }
          std::nth_element(begin,begin +((count-1)/2), end );
          median1 = *(begin + ( count/2 - 1));
          std::nth_element(begin,begin +(count/2), end );
          median2 = *(begin + ( count/2  ));
          median1 = (median1 + median2) / 2.0;
        }
      else
        {
          if ( 1 == count )
            {
              return *begin;
            }
          std::nth_element(begin,begin +((count-1)/2), end );
          median1 = *(begin + ((count-1)/2));
          // middle value is median
        }
      return median1;
    }


  }//namespace algo
}//namespace ralab ends here



#endif // MOMENTSW_H
