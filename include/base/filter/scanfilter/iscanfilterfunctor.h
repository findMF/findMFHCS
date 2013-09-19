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
        struct IScanFilterFunctor
        {
          typedef TReal value_type;
          //virtual int32_t operator()(std::vector<TReal> & vector) = 0;
          virtual int32_t operator()
          (
              const std::vector<TReal> & vector
              ,std::vector<TReal> & out
              ) = 0;

          virtual int32_t operator()(std::vector<TReal> & in) = 0;

          virtual boost::shared_ptr<IScanFilterFunctor < TReal > > clone() = 0;
          virtual ~IScanFilterFunctor(){}
        };

        typedef boost::shared_ptr<IScanFilterFunctor<double> > IScanFilterFunctorDoublePtr;
        typedef boost::shared_ptr<IScanFilterFunctor<float> > IScanFilterFunctorFloatPtr;
      }
    }

  }//end namespace
}//end namespace ralab



#endif // ISCANFILTERFUNCTOR_H
