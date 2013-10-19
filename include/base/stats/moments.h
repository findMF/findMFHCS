// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef MOMENTS_H
#define MOMENTS_H

#include "utilities/stats.h"

#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include "base/base/base.h"

namespace ralab
{
  namespace stats
  {
    /// variance \f[ mu_2 = 1/N \cdot \sum(x_i - \mu)^2 \f]
    template<class TReal, class Iter_T>
    TReal var(Iter_T first, Iter_T last, TReal mean) {
      return utilities::nthMoment<TReal, 2>(first, last, mean);
    }

    /// variance \f[ mu_2 = 1/N \cdot \sum{(x_i - \mu)^2} \f]
    template<class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type  var(Iter_T first, Iter_T last) {
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      TReal mean(ralab::base::base::mean(first, last));
      return utilities::nthMoment<TReal, 2>(first, last, mean);
    }

    /// unbiased variance \f[ 1/(n-1) \sum{(x - \mu)^2} \f]
    template<class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type varUnbiased(Iter_T first, Iter_T last)
    {
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      TReal mean( ralab::base::base::mean(first,last) );
      size_t cnt( distance(first, last) - 1 ); //unbiased...
      return std::accumulate(first, last, TReal( ), utilities::SumDiffNthPower<TReal, 2>(mean)) / cnt;
    }

    /// standard deviation \f[ \sqrt{\mu_2} \f], with \f$ \mu_2 \f$ - second moment \sa nthMoment.
    template<class TReal, class Iter_T>
    TReal sd(Iter_T first, Iter_T last, TReal mean) {
      return sqrt(computeVariance(first, last, mean));
    }


    /// standard deviation \f[ \sqrt{\mu_2} \f], with \f$ \mu_2 \f$ - second moment \sa nthMoment.
    template<class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type sd(Iter_T first, Iter_T last)
    {
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      TReal meanVal( ralab::base::base::mean(first, last));
      return sqrt(var(first, last, meanVal));
    }

    /// unbiased stdv \f[ \sqrt{1/(n-1) \sum(x - \mu)^2} \f]
    template< class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type sdUnbiased(Iter_T first, Iter_T last)
    {
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      return sqrt(varUnbiased(first, last));
    }

    /// Skew \f[ \mu_3 / (\mu_2 \cdot \sqrt{\mu_2}) \f], with \f$ \mu_2 \f$ - second moment.
    template< class TReal , class Iter_T>
    TReal skew(
        Iter_T begin, Iter_T end, TReal mean)
    {
      TReal m3 = utilities::nthMoment<TReal, 3>(begin, end, mean);
      TReal m2 = utilities::nthMoment<TReal, 2>(begin, end, mean);
      return m3 / (m2 * sqrt(m2));
    }

    /// Skew \f[ \mu_3 / (\mu_2 \cdot \sqrt{(\mu_2)}) \f], with \f$ \mu_2 \f$ - second moment.
    template<class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type skew(Iter_T begin, Iter_T end){
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      TReal meanVal( ralab::base::base::mean(begin,end));
      return (skew(begin,end,meanVal));
    }

    /// Kurtosis \f[ \mu_4 / (\mu_2 \cdot \mu_2) - 3 \f], with \f$ \mu_2 \f$ - second moment.
    template<class TReal, class Iter_T>
    TReal kurtosis(Iter_T begin, Iter_T end, TReal mean)
    {
      TReal m4 = utilities::nthMoment<TReal, 4>(begin, end, mean);
      TReal m2 = utilities::nthMoment<TReal, 2>(begin, end, mean);
      return m4 / (m2 * m2) - 3;
    }

    /// Kurtosis \f[ \mu_4 / (\mu_2 \cdot \mu_2) - 3 \f], with \f$ \mu_2 \f$ - second moment.
    template<class Iter_T>
    typename std::iterator_traits<Iter_T>::value_type kurtosis(Iter_T begin, Iter_T end){
      typedef typename std::iterator_traits<Iter_T>::value_type TReal;
      TReal meanVal(ralab::base::base::mean(begin,end));
      return (kurtosis(begin,end,meanVal));
    }

    /// computes sum, mean, var, std_dev, skew, kurt */
    template<class TReal, class Iter_T>
    void computeStats(
        Iter_T first, Iter_T last, TReal& sum, TReal& mean,
        TReal& var, TReal& std_dev, TReal& skew, TReal& kurt
        )
    {
      size_t cnt = distance(first, last);
      sum = std::accumulate(first, last, TReal( ));
      mean = sum / cnt;
      var = ralab::stats::var(first, last, mean);
      std_dev = sqrt(var);
      skew = ralab::stats::skew(first, last, mean);
      kurt = ralab::stats::kurtosis(first, last, mean);
    }
  }
}

#endif
