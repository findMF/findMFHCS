// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef UNIFORM_H
#define UNIFORM_H

#include <complex>
#include <vector>

#include <boost/generator_iterator.hpp>
#include <time.h>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include "base/base/constants.h"
#include "base/base/timesince69.h"

namespace ralab
{
  namespace base
  {
    namespace stats
    {

      typedef boost::minstd_rand base_generator_type;
      /*!
                        \brief Generates random numbers to fill data structures.
                        */
      template<typename TReal = double>
      struct Uniform
      {
        typedef TReal value_type;
        typedef TReal result_type;
        base_generator_type generator;
        boost::uniform_real<value_type> uni_dist;
        boost::variate_generator<base_generator_type&, boost::uniform_real<value_type> > uni;
        // Define a uniform random number distribution which produces "double"
        // values between 0 and 1 (0 inclusive, 1 exclusive).
      public:
        Uniform(
            double min, //!< lower limit of distribution
            double max, //!< upper limit of distribution
            uint32_t seed = 0 //!< seed
            )
          : generator(42u),
            uni_dist(min , max),
            uni(generator, uni_dist)
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
          return uni();
        }

        value_type getValue()
        {
          return uni();
        }
      private:
        Uniform<value_type> & operator=(const Uniform<value_type> & );
      };


      /*! \brief The Uniform Distribution - runif generates random deviates.
                        */
      template<typename TReal >
      //static void runif(uint32_t n, TReal min, TReal max, std::vector<TReal> &res)
      void runif(
          typename std::vector<TReal>::size_type n, //!< number of observations
          std::vector<TReal> &res,//!< output vector
          TReal min = 0.,//!< lower limit of distribution
          TReal max = 1. ,//!< upper limit of distribution
          uint32_t seed =0 //!< seed
          )

      {
        typedef typename std::vector<TReal>::size_type size_type;
        Uniform<TReal> uni(min,max,seed);
        res.clear();
        res.reserve(n);
        for(size_type i = 0; i < n; ++i)
          {
            res.push_back(uni());
          }
      }

    }//namespace stats
  }//namespace base
}//namespace ralab
#endif
