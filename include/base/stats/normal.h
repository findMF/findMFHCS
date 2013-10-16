// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef NORMAL_H
#define NORMAL_H

#include <complex>
#include <vector>
#include <time.h>
#include <boost/math/distributions/normal.hpp>
#include <boost/bind.hpp>
#include <math.h>


#include <boost/random/linear_congruential.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>

#include "base/base/timesince69.h"
#include "base/base/constants.h"

namespace ralab
{
  namespace base
  {
    namespace stats
    {


      typedef boost::minstd_rand base_generator_type;

      /**
                        Generates random numbers to fill data structures.
                        */


      template <typename TReal = double>
      struct Normal
      {
        typedef TReal value_type;
        typedef TReal result_type;
        base_generator_type generator;
        boost::normal_distribution<value_type> normal_dist;
        boost::variate_generator< base_generator_type& , boost::normal_distribution<value_type> > normal;
        // Define a uniform random number distribution which produces "double"
        // values between 0 and 1 (0 inclusive, 1 exclusive).
      public:
        Normal
        (
            value_type mean =0. ,
            value_type sigma = 1.,
            int seed = 0
            )
          : generator( 42u ),
            normal_dist(mean,sigma),
            normal(generator, normal_dist)
        {
          if(seed == 0)
            {
              generator.seed( ralab::base::secondsSince69() );
            }
          else
            {
              generator.seed(seed);
            }
        }

        value_type operator()()
        {
          return normal();
        }

        value_type getValue()
        {
          return normal();
        }
      private:
        Normal<value_type> & operator=(const Normal<value_type> & );
      };

      /** The Normal Distribution - runif generates random deviates.

                        returns seed;
                        */
      template<typename TReal >
      uint32_t rnorm
      (
          typename std::vector<TReal>::size_type n, //!< nr values to sample
          std::vector<TReal> &res, //!< [out]
          TReal mean = 0., //!< mean
          TReal sigma = 1., //!< sigma
          uint32_t seed = 0 //!< seed
          )
      {
        typedef typename std::vector<TReal>::size_type size_type;
        Normal<TReal> normal(mean,sigma,seed);
        res.clear();
        res.reserve(n);
        for(size_type i = 0; i < n; ++i)
          {
            res.push_back(normal());
          }
        return 0;
      }


      template<typename OutputIterator >
      OutputIterator rnorm
      (
          OutputIterator beg, //!< [out]
          OutputIterator end, //!< [out]
          typename std::iterator_traits<OutputIterator>::value_type mean = 0., //!< mean
          typename std::iterator_traits<OutputIterator>::value_type sigma = 1., //!< sigma
          uint32_t seed = 0 //!< seed
          )
      {
        typedef typename std::iterator_traits<OutputIterator>::value_type TReal;
        Normal<TReal> normal(mean,sigma,seed);
        for(; beg !=end ;  ++beg)
          {
            *beg = normal();
          }
        return beg;
      }

      template<typename TReal>
      struct Dnorm : std::unary_function<TReal, TReal>
      {
        boost::math::normal_distribution<TReal> nd_;
        Dnorm(TReal mean, TReal sd): nd_(mean,sd)
        {}

        TReal operator()(TReal x)
        {
          return boost::math::pdf( nd_ , x);
        }
      };

      /** Density, distribution function, quantile function and random generation for the normal distribution with mean
                        equal to mean and standard deviation equal to sd. */
      template<typename TInputIterator>
      TInputIterator dnorm(
          TInputIterator  beg,
          TInputIterator end,
          typename std::iterator_traits<TInputIterator>::value_type mean = 0.,
          typename std::iterator_traits<TInputIterator>::value_type sd = 1.,
          bool logb =false
          )
      {
        typedef typename std::iterator_traits<TInputIterator>::value_type TReal;
        Dnorm<TReal> dn(mean,sd);
        if(!logb)
          {
            end = std::transform(beg,end,beg,
                                 boost::bind(dn, _1)
                                 );
          }
        else
          {
            end = std::transform( beg , end , beg ,boost::bind<TReal>(log, boost::bind(dn, _1) ));
          }
        return end;
      }

      /** Density, distribution function, quantile function and random generation for the normal distribution with mean
                        equal to mean and standard deviation equal to sd. */
      template<typename TInputIterator, typename TOutputIterator >
      TOutputIterator dnorm(
          TInputIterator xbeg,
          TInputIterator xend,
          TOutputIterator ybeg,
          typename std::iterator_traits<TInputIterator>::value_type mean = 0.,
          typename std::iterator_traits<TInputIterator>::value_type sd = 1.,
          bool logb =false)
      {
        typedef typename std::iterator_traits<TInputIterator>::value_type TReal;
        Dnorm<TReal> dn(mean,sd);
        TOutputIterator yend;
        if(!logb)
          {
            yend = std::transform(xbeg,xend,ybeg,
                                  boost::bind(dn, _1) );
          }
        else
          {
            yend = std::transform( xbeg , xend , ybeg ,
                                   boost::bind<TReal>(log, boost::bind(dn, _1) ) );
          }
        return yend;
      }

      /** Density, distribution function, quantile function and random generation for the normal distribution with mean
                        equal to mean and standard deviation equal to sd. */
      template<typename TReal>
      TReal dnorm_1(TReal x, TReal mean = 0., TReal sd = 1., bool logb =false)
      {
        if(!logb)
          {
            return boost::math::pdf( boost::math::normal_distribution<TReal>(mean, sd), x);
          }
        else
          {
            TReal tmp = boost::math::pdf( boost::math::normal_distribution<TReal>(mean, sd), x);
            tmp = log(tmp);
            return tmp;
          }
      }

      template<typename TReal>
      struct Pnorm : std::unary_function<TReal, TReal>
      {
        boost::math::normal_distribution<TReal> nd_;
        Pnorm(TReal mean, TReal sd): nd_(mean,sd)
        {}

        TReal operator()(TReal x)
        {
          return boost::math::cdf( nd_ , x);
        }
      };

      /** Density, distribution function, quantile function and random generation for the normal distribution with mean
                        equal to mean and standard deviation equal to sd. */
      template<typename TInputIterator>
      TInputIterator pnorm(
          TInputIterator beg
          , TInputIterator end
          , typename std::iterator_traits<TInputIterator>::value_type  mean = 0.
          , typename std::iterator_traits<TInputIterator>::value_type  sd = 1.
          , bool lowerTail = true
          , bool logP =false)
      {
        typedef typename std::iterator_traits<TInputIterator>::value_type TReal;
        Pnorm<TReal> pn(mean,sd);
        end = std::transform( beg , end , beg , boost::bind(pn, _1) );
        if(!lowerTail)
          end=std::transform(beg , end , beg ,boost::bind<TReal>(std::minus<TReal>(), 1 , _1  ) );
        if(logP)
          end = std::transform(beg, end , beg ,boost::bind<TReal>(log,_1 ) );
        return end;
      }


      /** Density, distribution function, quantile function and random generation for the normal distribution with mean
                        equal to mean and standard deviation equal to sd. */
      template<typename TInputIterator, typename TOutputIterator >
      TOutputIterator pnorm(
          TInputIterator xbeg,
          TInputIterator xend,
          TOutputIterator ybeg,
          typename std::iterator_traits<TInputIterator>::value_type mean = 0.,
          typename std::iterator_traits<TInputIterator>::value_type sd = 1.,
          bool lowerTail = true,
          bool logP =false)
      {
        typedef typename std::iterator_traits<TInputIterator>::value_type TReal;
        Pnorm<TReal> pn(mean,sd);
        TOutputIterator yend;

        yend = std::transform(xbeg,xend,ybeg,boost::bind(pn, _1) );
        if(!lowerTail)
          yend = std::transform( ybeg , yend , ybeg ,boost::bind<TReal>(std::minus<TReal>(), 1 , _1  ) );
        if(logP)
          yend = std::transform( ybeg , yend , ybeg ,boost::bind<TReal>(log,  _1  ) );
        return yend;
      }

      /** Density, distribution function, quantile function and random generation for the normal distribution with mean
                        equal to mean and standard deviation equal to sd. */
      template<typename TReal>
      TReal pnorm_1(TReal x, TReal mean = 0., TReal sd = 1.
          ,  bool lowerTail = true
          , bool logP =false)
      {
        TReal res =  boost::math::cdf( boost::math::normal_distribution<TReal>(mean, sd), x);

        if(!lowerTail)
          res = 1 - res;
        if(logP)
          res = log(res);
        return res;
      }


      template<typename TReal>
      struct Qnorm : std::unary_function<TReal, TReal>
      {
        boost::math::normal_distribution<TReal> nd_;
        Qnorm(TReal mean, TReal sd): nd_(mean,sd)
        {}

        TReal operator()(TReal x)
        {
          return boost::math::quantile( nd_ , x);
        }
      };


      /** Density, distribution function, quantile function and random generation for the normal distribution with mean
                        equal to mean and standard deviation equal to sd. */
      template<typename TInputIterator, typename TOutputIterator >
      TOutputIterator qnorm(
          TInputIterator xbeg,
          TInputIterator xend,
          TOutputIterator ybeg,
          typename std::iterator_traits<TInputIterator>::value_type mean = 0.,
          typename std::iterator_traits<TInputIterator>::value_type sd = 1.,
          bool lowerTail = true,
          bool logP =false)
      {
        typedef typename std::iterator_traits<TInputIterator>::value_type TReal;
        Qnorm<TReal> pn(mean,sd);
        TOutputIterator yend;
        std::vector<TReal> tmp(xbeg,xend);
        if(logP)
          {
            std::transform(tmp.begin() , tmp.end() , tmp.begin() , boost::bind<TReal>( exp , _1 ) );
          }

        if(!lowerTail)
          yend = std::transform( tmp.begin() ,
                                 tmp.end() ,
                                 tmp.begin() , boost::bind<TReal>(std::minus<TReal>(), 1 , _1  ) );

        yend = std::transform( tmp.begin() , tmp.end() , ybeg,boost::bind(pn, _1) );
        /*if(logP)
                                        yend = std::transform( ybeg , yend , ybeg ,boost::bind<TReal>(log,  _1  ) );*/
        return yend;
      }

      /** Density, distribution function, quantile function and random generation for the normal distribution with mean
                        equal to mean and standard deviation equal to sd. */
      template<typename TReal>
      TReal qnorm_1(TReal x, TReal mean = 0., TReal sd = 1.
          ,  bool lowerTail = true
          , bool logP =false)
      {
        TReal res =  boost::math::quantile( boost::math::normal_distribution<TReal>(mean, sd), x);

        if(!lowerTail)
          res = 1 - res;
        if(logP)
          res = log(res);
        return res;
      }



    }//namespace
  }//namespace
}//namespace ralab
#endif
