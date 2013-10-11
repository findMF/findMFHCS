// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef STATS_H
#define STATS_H

#include <math.h>
#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include "base/base/base.h"

/*! \file Statistics.h
\brief Descriptive statistics: stdv, var, mean, max, min */
namespace ralab
{
  namespace stats
  {
    namespace utils
    {
      //This code is taken from C++ Cookbook... 11-9 page 404
      /*!
                                \brief Descriptive Statistics
                                Descriptive statistics are used to describe the basic features of the data
                                in a study. They provide simple summaries about the sample and the measures.
                                Together with simple graphics analysis, they form the basis of virtually
                                every quantitative analysis of data.


                                */


      /*! @brief nthPower */
      template<int N, typename TReal>
      TReal nthPower(TReal x) {
        TReal ret = x;
        for (int i=1; i < N; ++i) {
            ret *= x;
          }
        return ret;
      }


      /*! \brief Functor \f[ sum + (current - mu)\f] */
      template<typename TReal, uint32_t N>
      struct SumDiffNthPower : std::binary_function<TReal, TReal, TReal>
      {
        SumDiffNthPower(TReal mean) : mean_(mean) { };
        TReal operator( )(TReal sum, TReal current) {
          return (sum + nthPower<N>(current - mean_));
        }
        TReal mean_;
      };

      /*! \brief nth central moment \$[ mu_n = \sum((x_i - mean)^n) \$] */
      template<class TReal, int N, class Iter_T>
      TReal nthMoment(Iter_T first, Iter_T last, TReal mean)  {
        size_t cnt = distance(first, last);
        return std::accumulate(first, last, TReal( ), SumDiffNthPower<TReal, N>(mean)) / cnt;
      }

      /*! \brief \f[ X * X * W \f] */
      template<typename TReal>
      struct XXW : std::binary_function<TReal, TReal, TReal>
      {
        TReal operator( )(TReal X, TReal W) {
          return ( X * X * W );
        }
        TReal mean_;
      };

      /*! \brief Functor for computing Nth Power differences and to multiply them with a weight.

                                computes \f[ weight \cdot (value - m_{AverageMass})^N \f]
                                */
      template<typename TReal, uint32_t N>
      struct DiffNthPowerWeight : std::binary_function< TReal , TReal , TReal>
      {
        DiffNthPowerWeight(TReal x): m_AverageMass(x){};
        TReal operator()(TReal value , TReal weight)
        {
          TReal diffPow (nthPower<N>(value - m_AverageMass));
          return(diffPow * weight);
        }
        TReal m_AverageMass;
      };

    }//end utils

    /*! \name Moments - Mean, Standard Deviation, Variance
                        */




    /*! \brief variance \f[ mu_2 = 1/N \cdot \sum(x_i - \mu)^2 \f] */
    template<class TReal, class Iter_T>
    TReal computeVariance(Iter_T first, Iter_T last, TReal mean) {
      return utils::nthMoment<TReal, 2>(first, last, mean);
    }

    /*! \brief variance \f[ mu_2 = 1/N \cdot \sum{(x_i - \mu)^2} \f] */
    template<class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type  computeVariance(Iter_T first, Iter_T last) {
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      TReal mean(ralab::base::base::mean(first, last));
      return utils::nthMoment<TReal, 2>(first, last, mean);
    }

    /*! \brief unbiased variance \f[ 1/(n-1) \sum{(x - \mu)^2} \f] */
    template<class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type computeVarianceUnbiased(Iter_T first, Iter_T last)
    {
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      TReal mean( ralab::base::base::mean(first,last) );
      size_t cnt( distance(first, last) - 1 ); //unbiased...
      return std::accumulate(first, last, TReal( ), utils::SumDiffNthPower<TReal, 2>(mean)) / cnt;
    }

    /*! \brief standard deviation \f[ \sqrt{\mu_2} \f], with \f$ \mu_2 \f$ - second moment \sa nthMoment.  */
    template<class TReal, class Iter_T>
    TReal computeStdDev(Iter_T first, Iter_T last, TReal mean) {
      return sqrt(computeVariance(first, last, mean));
    }


    /*! \brief standard deviation \f[ \sqrt{\mu_2} \f], with \f$ \mu_2 \f$ - second moment \sa nthMoment. */
    template<class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type computeStdDev(Iter_T first, Iter_T last)
    {
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      TReal meanVal( ralab::base::base::mean(first, last));
      return sqrt(computeVariance(first, last, meanVal));
    }

    /*! \brief unbiased stdv \f[ \sqrt{1/(n-1) \sum(x - \mu)^2} \f] */
    template< class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type computeStdDevUnbiased(Iter_T first, Iter_T last)
    {
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      return sqrt(computeVarianceUnbiased(first, last));
    }

    /*! \brief Skew \f[ \mu_3 / (\mu_2 \cdot \sqrt{\mu_2}) \f], with \f$ \mu_2 \f$ - second moment. */
    template< class TReal , class Iter_T>
    TReal computeSkew(
        Iter_T begin, Iter_T end, TReal mean)
    {
      TReal m3 = utils::nthMoment<TReal, 3>(begin, end, mean);
      TReal m2 = utils::nthMoment<TReal, 2>(begin, end, mean);
      return m3 / (m2 * sqrt(m2));
    }

    /*! \brief Skew \f[ \mu_3 / (\mu_2 \cdot \sqrt{(\mu_2)}) \f], with \f$ \mu_2 \f$ - second moment.  */
    template<class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type computeSkew(Iter_T begin, Iter_T end){
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      TReal meanVal( ralab::base::base::mean(begin,end));
      return (computeSkew(begin,end,meanVal));
    }

    /*! \brief Kurtosis \f[ \mu_4 / (\mu_2 \cdot \mu_2) - 3 \f], with \f$ \mu_2 \f$ - second moment.  */
    template<class TReal, class Iter_T>
    TReal computeKurtosisExcess(Iter_T begin, Iter_T end, TReal mean)
    {
      TReal m4 = utils::nthMoment<TReal, 4>(begin, end, mean);
      TReal m2 = utils::nthMoment<TReal, 2>(begin, end, mean);
      return m4 / (m2 * m2) - 3;
    }

    /*! \brief Kurtosis \f[ \mu_4 / (\mu_2 \cdot \mu_2) - 3 \f], with \f$ \mu_2 \f$ - second moment.  */
    template<class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type computeKurtosisExcess(Iter_T begin, Iter_T end){
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      TReal meanVal(ralab::base::base::mean(begin,end));
      return (computeKurtosisExcess(begin,end,meanVal));
    }

    /*! \brief computes sum, mean, var, std_dev, skew, kurt */
    template<class TReal, class Iter_T>
    void computeStats(
        Iter_T first, Iter_T last, TReal& sum, TReal& mean,
        TReal& var, TReal& std_dev, TReal& skew, TReal& kurt
        )
    {
      size_t cnt = distance(first, last);
      sum = std::accumulate(first, last, TReal( ));
      mean = sum / cnt;
      var = computeVariance(first, last, mean);
      std_dev = sqrt(var);
      skew = computeSkew(first, last, mean);
      kurt = computeKurtosisExcess(first, last, mean);
    }


    /*! \brief Weighted average \f[ \sum(vValues \cdot vWeights) / \sum(vWeights) \f] */
    template< typename TReal >
    const TReal
    WeightedAverage
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

    /*! \brief Weighted Variance \f[ 1/\sum{w_i} \cdot \sum{ w_i ( x_i - \mu )^2 } \f]
      */
    template< typename TReal >
    const TReal
    VarWeight
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
                   utils::DiffNthPowerWeight<TReal,2>(dAverageMass)
                   ));

      if (dSumOfWeights > 0.0)
        {
          var /= dSumOfWeights; // weighted sum of squared deviations
        }
      return var;
    }

    /*! \brief Weighted Variance \f[ 1/\sum{w_i} \cdot \sum{ w_i ( x_i - \mu )^2 } \f] */
    template<typename TReal>
    const TReal
    VarWeight
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights //!<[in] Weight
        )
    {
      TReal dAverageMass ( WeightedAverage<TReal>( vValues , vWeights ));
      return(
            VarWeight<TReal>( vValues , vWeights, dAverageMass ));
    }
    /*! \brief Weighted standard deviation \f[ \sqrt{var} \f] */
    template<typename TReal>
    const TReal
    SdWeight
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights //!<[in] Weight
        )
    {
      return(
            sqrt(VarWeight(vValues , vWeights ))
            );
    }

    /*! \brief Weighted standard deviation \f[ \sqrt{var} \f] */
    template<typename TReal>
    const TReal
    SdWeight
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights , //!<[in] Weight
        const TReal dAverageMass //!<[in]
        )
    {
      return(
            sqrt(VarWeight( vValues , vWeights ,dAverageMass ))
            );
    }

    /*\brief Skewness (Kruemmung/Schiefe) (Sachs "Angewandte Statistic")

                        (method calls based on GSL)
                        Ist v > 0, so ist die Verteilung rechtsschief, ist v<0, ist die Verteilung linksschief (auch genannt rechtssteil).
                        Bei rechtsschiefen (oder linkssteilen) Verteilungen sind Werte, die kleiner sind als der Mittelwert,
                        hufiger zu beobachten, so dass sich der Gipfel (Modus) links vom Mittelwert befindet;
                        der rechte Teil des Graphs ist flacher als der linke.

                        */
    template<typename TReal>
    const TReal WeightedSkewness
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights , //!<[in] Weight
        const TReal dAverage, //!<[in] mean
        const TReal dSD//!<[in] sd
        )
    {

      // computes \Sum(w * (x- weightedaverage)^3)
      //std::vector<TReal> tmp;
      //std::transform(vValues.begin(),vValues.end(), vWeights.begin(), std::back_inserter(tmp) , DiffNthPowerWeight<TReal,3>(dAverage) );

      TReal wm3( std::inner_product(
                   vValues.begin(),
                   vValues.end(),
                   vWeights.begin(),
                   TReal(),
                   std::plus<TReal>(),
                   utils::DiffNthPowerWeight<TReal,3>(dAverage)
                   ));
      TReal N( std::accumulate(vWeights.begin(),vWeights.end(),TReal() ));
      return (  wm3/( N * dSD * dSD * dSD ) );
    }

    /*\brief Skewness (Kruemmung/Schiefe) (Sachs "Angewandte Statistic") */

    template<typename TReal>
    const TReal WeightedSkewness
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights  //!<[in] Weight
        )
    {
      TReal dAverage( WeightedAverage<TReal>( vValues , vWeights ) );
      TReal dSD(SdWeight(vValues,vWeights,dAverage));
      return WeightedSkewness(vValues,vWeights,dAverage, dSD);
    }

    /*!\brief Kurtosis (Woelbung)
                        (method calls based on GSL)

                        <ul>
                        <li> k = 0: normalgipflig oder mesokurtisch. Die Normalverteilung hat die Kurtosis  = 3 und entsprechend den Exzess ?2 = 0.
                        <li> k > 0: steilgipflig, supergaufrmig oder leptokurtisch. Es handelt sich hierbei um im Vergleich zur Normalverteilung spitzere Verteilungen, d.h. Verteilungen mit starken Peaks.
                        <li> k < 0: flachgipflig, subgaufrmig oder platykurtisch. Man spricht von einer im Vergleich zur Normalverteilung abgeflachten Verteilung.
                        </ul>
                        */
    template<typename TReal>
    const TReal WeightedKurtosis
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
                   utils::DiffNthPowerWeight<TReal,4>(dAverage)
                   ));
      TReal N( std::accumulate( vWeights.begin() , vWeights.end() , TReal() ));
      return (  wm4 /( N * dSD * dSD * dSD * dSD ) - 3 );
    }

    /*\brief Skewness (Kruemmung/Schiefe) (Sachs "Angewandte Statistic") */

    template<typename TReal>
    const TReal WeightedKurtosis
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights  //!<[in] Weight
        )
    {
      TReal dAverage( WeightedAverage<TReal>( vValues , vWeights ) );
      TReal dSD(SdWeight(vValues,vWeights,dAverage));
      return WeightedKurtosis(vValues , vWeights , dAverage , dSD);
    }



    /*!\name unbiased versions of weighted variance and standard deviation */


    /*! \brief Weighted variance unbiased (see GSL documentation)

                        \f[ ( sum(wx^2) sum(w) - sum(wx)^2 ) / ( sum(w)^2 - sum(w^2) )  \f]
                        */
    template<typename TReal>
    const TReal
    VarUnbiasedWeight
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
                                        , std::plus<TReal>() , utils::XXW<TReal>()  ) );
      TReal var = ( sumXXW * sumW - sumWX*sumWX ) / ( sumW * sumW - sumW2 );
      return ( var );
    }

    /*! \brief Weighted standard deviation unbiased (see GSL documentation)

                        \f[ \sqrt{VarUnbiasedWeight} \f]
                        */
    template<typename TReal>
    const TReal
    SdUnbiasedWeight
    (
        const std::vector<TReal> & vValues , //!<[in] Values
        const std::vector<TReal> & vWeights  //!<[in] Weight
        )
    {
      return(sqrt(VarUnbiasedWeight( vValues , vWeights )));
    }

    /*@}*/

    /*!  QUANTILE Quantile statistics

                        \brief Includes such methods as: median, quantile, range. fivenum


                        */

    /*!\brief Median Value

                        Compute the sample median.
                        */
    template<typename RanIt >
    typename std::iterator_traits<RanIt>::value_type Median
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
          //TSize endIndex =  count/2;
          //endIndex;
          //std::partial_sort(begin, begin + endIndex + 1, end );
          // 4/2 -1 = 1, 4/2 = 2 ; 6/2 - 1 = 2 , 6/2 = 3
          // 2/2 -1 = 0, 2/2 = 1
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
          //(5-1)/2 = 2 ; (3-1)/2 = 1
          std::nth_element(begin,begin +((count-1)/2), end );
          median1 = *(begin + ((count-1)/2));
          // middle value is median
        }
      return median1;
    }


    template<typename TReal>
    TReal Median(  const std::vector<TReal>  & x )
    {
      std::vector<TReal> tmp(x); //create a copys
      return Median( tmp.begin() , tmp.end() );
    }

    /*! \brief Sample Quantiles

                        The generic function quantile produces sample quantiles
                        corresponding to the given probabilities. The smallest
                        observation corresponds to a probability of 0 and the
                        largest to a probability of 1.

                        Same as R's qunatile with type=2
                        */

    template<typename TReal>
    void Quantile(
        std::vector<TReal> values, //!<[in] values - requires to be sorted.
        const std::vector<TReal> & probs, //!<[in] numeric vector of probabilities with values in [0,1].
        std::vector<TReal> & result //!<[out] Inverse of empirical distribution function.
        )
    {
      typedef typename std::vector<TReal>::size_type size_type;
      //check if range of probabilities
      std::pair<TReal,TReal> xx;
      ralab::base::base::Range(probs, xx);
      bool rcheck = ((xx.first >= 0.0) && (xx.second <= 1.0));
      if(! rcheck )
        {
          throw( std::logic_error("probs - numeric vector of probabilities with values in [0,1]"));
        }

      if(values.size() == 0)
        {
          result.resize(probs.size());
          result.insert(result.begin(),probs.size(), std::numeric_limits<TReal>::quiet_NaN());
          return;
        }
      result.clear();
      std::sort(values.begin(),values.end());


      //determine indices of quantiles
      size_type size = values.size();
      TReal dsize = static_cast<TReal>(size);
      std::vector<TReal> pos(probs);
      std::transform( pos.begin(), pos.end(), pos.begin(),  std::bind2nd( std::multiplies<TReal>() , (dsize-1.) ) );
      std::transform( pos.begin(), pos.end(), pos.begin(),  std::bind2nd( std::plus<TReal>() , 1. ));

      // iterate through the indices and retrieve the quantiles...

      for( typename std::vector<TReal>::iterator i = pos.begin() ; i != pos.end() ; ++i )
        {
          size_type ind = static_cast<size_type>(*i);
          TReal tmp = ( *i ) -  static_cast<TReal>(ind);
          if( fabs(tmp - 0.) < 1e-10 )
            {
              result.push_back(values.at(ind-1)); // -1 because of pointer arithmetics.
            }
          else
            {
              result.push_back((values.at(ind-1) + values.at(ind)) / 2 );
            }
        }
    }

    /*! \brief Tukey Five-Number Summaries
                        Returns Tukey's five numb
//  Archive locater summary (minimum, lower-hinge, median, upper-hinge, maximum) for the input data.
                        */
    template<typename TReal>
    void Fivenum(
        const std::vector<TReal> & values, //!<[in] values
        std::vector<TReal> & fivenum //!<[out] fivenum
        )
    {
      std::vector<TReal> probs;
      probs.push_back(0);
      probs.push_back(.25);
      probs.push_back(.5);
      probs.push_back(.75);
      probs.push_back(1);
      Quantile(values, probs, fivenum);
    }


    template<typename TReal>
    std::ostream & Print_Fivenum(std::ostream & ostream,  std::vector<TReal> & values, bool header = false )
    {
      if(header)
        {
          ostream << "Min.\t 1st Qu.\t Median\t Mean\t 3rd Qu.\t Max." << std::endl;
        }
      else
        {
          ostream << values.at(0) << "\t";
          ostream << values.at(1) << "\t";
          ostream << values.at(2) << "\t";
          ostream << values.at(3) << "\t";
          ostream << values.at(4) << std::endl;
        }
      return ostream;
    }
    /*! @} */
  }//namespace algo
}//namespace ralab ends here



#endif // STATS_H
