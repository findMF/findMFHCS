// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef ISCANFILTERFUNCTOR_H
#define ISCANFILTERFUNCTOR_H

#include <vector>
#include <boost/shared_ptr.hpp>


namespace ralab
{
  namespace base
  {
    namespace filter
    {
      namespace scanfilter
      {

        /*!	\brief	Scan filter functor.
    */
        template <typename TReal>
        struct IScanFilter
        {
          typedef TReal value_type;
          //virtual int32_t operator()(std::vector<TReal> & vector) = 0;
          virtual int32_t operator()
          (
              const std::vector<TReal> & vector
              ,std::vector<TReal> & out
              ) = 0;

          virtual int32_t operator()(std::vector<TReal> & in) = 0;
          virtual ~IScanFilter(){}
        };

        typedef boost::shared_ptr<IScanFilter<double> > IScanFilterDoublePtr;
        typedef boost::shared_ptr<IScanFilter<float> > IScanFilterFloatPtr;
      }
    }

  }//end namespace
}//end namespace ralab



#endif // ISCANFILTERFUNCTOR_H
