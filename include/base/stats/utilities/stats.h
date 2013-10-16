// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef UTILITIESSTATS_H
#define UTILITIESSTATS_H

#include <math.h>
#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>

namespace ralab
{
  namespace stats
  {
    namespace utilities
    {
      //This code is taken from C++ Cookbook... 11-9 page 404
      /**
        nthPower
      */
      template<int N, typename TReal>
      TReal nthPower(TReal x) {
        TReal ret = x;
        for (int i=1; i < N; ++i) {
            ret *= x;
          }
        return ret;
      }
      /**
       * Functor \f[ sum + (current - mu)\f] */
      template<typename TReal, uint32_t N>
      struct SumDiffNthPower : std::binary_function<TReal, TReal, TReal>
      {
        SumDiffNthPower(TReal mean) : mean_(mean) { };
        TReal operator( )(TReal sum, TReal current) {
          return (sum + nthPower<N>(current - mean_));
        }
        TReal mean_;
      };

      /** nth central moment \$[ mu_n = \sum((x_i - mean)^n) \$] */
      template<class TReal, int N, class Iter_T>
      TReal nthMoment(Iter_T first, Iter_T last, TReal mean)  {
        size_t cnt = distance(first, last);
        return std::accumulate(first, last, TReal( ), SumDiffNthPower<TReal, N>(mean)) / cnt;
      }

      /** \f[ X * X * W \f] */
      template<typename TReal>
      struct XXW : std::binary_function<TReal, TReal, TReal>
      {
        TReal operator( )(TReal X, TReal W) {
          return ( X * X * W );
        }
        TReal mean_;
      };

      /** Functor for computing Nth Power differences and to multiply them with a weight.

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
  }
}

#endif
