// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef QUANTILES_H
#define QUANTILES_H

//#include "utilities/stats.h"

#include "base/base/base.h"

namespace ralab
{
  namespace stats
  {
    /// median Value
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

    /** Sample Quantiles

                    sample quantiles
                    corresponding to the given probabilities. The smallest
                    observation corresponds to a probability of 0 and the
                    largest to a probability of 1.

                    Same as R's qunatile with type=2
                    */

    template<typename TReal>
    void quantile(
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

    /** Tukey Five-Number Summaries
                    Returns Tukey's five number
summary (minimum, lower-hinge, median, upper-hinge, maximum) for the input data.
                    */
    template<typename TReal>
    void fivenum(
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
      quantile(values, probs, fivenum);
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

  }//namespace algo
}//namespace ralab ends here



#endif // MOMENTSW_H
