#ifndef DETERMINEBINWIDTHUTILITIES_H
#define DETERMINEBINWIDTHUTILITIES_H

#include <functional>
#include <stdint.h>
#include <algorithm>
namespace ralab
{
  namespace base
  {
    namespace resample
    {

      namespace utilities{

        template<class T>
        struct meanfunctor : std::binary_function<T,T,T>{
          T operator()(const T & x, const T& y){
            return (x+y)/2.;
          }
        };


        template <
            typename InputIterator,
            typename OutputIterator,
            typename TN = int32_t
            >
        OutputIterator summ
        (
            InputIterator begin, //!< [in] begin
            InputIterator end, //!< [in] end
            OutputIterator destBegin, //!< [out] dest begin
            TN lag = 1//!< [in] an integer indicating which lag to use.
            )
        {
          return( std::transform(begin + lag
                                 , end
                                 , begin
                                 , destBegin
                                 , meanfunctor<typename InputIterator::value_type>())
                  );
        }

        template<typename TRealI>
        double determine(TRealI begin, TRealI end,double maxj=5.){
          //BOOST_ASSERT(!boost::range::is_sorted(begin,end));
          double j = 1.;
          double average = *begin;
          double sum = average;
          int32_t i = 1;
          for(; begin != end ; ++begin, ++i){
              while(*begin > (j+0.5) *average){
                  ++j;
                }
              if(j > maxj){
                  break;
                }
              sum += *begin/j;
              average = sum/static_cast<double>(i);
            }
          return average;

        }
      }


    }
  }
}
#endif
