// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef TRANSFORMFILTER_H
#define TRANSFORMFILTER_H

#include "base/filter/scanfilter/iscanfilterfunctor.h"

namespace ralab{
  namespace base{
    namespace filter{
      namespace scanfilter{


        template <typename TReal, template <typename Telem> class Tfunctor>
        class TransformFilter: public IScanFilter<TReal>
        {
          std::string name_; //!< name of filter
        public:
          TransformFilter
          (
              ) : name_( "Transformfilter" )
          {}

          /*!\brief Clone */
          boost::shared_ptr< IScanFilter<TReal> > clone()
          {
            return boost::shared_ptr< IScanFilter<TReal> >(new TransformFilter<TReal, Tfunctor >(*this) );
          }


          /*!\brief Operator */
          int32_t operator()(
              std::vector<TReal> & rt //!< spectrum to filter
              )
          {
            std::transform(rt.begin(),rt.end(),rt.begin(),Tfunctor<TReal>());
            return 0;
          }

          /*!\brief General Filter
                        */
          template< typename InputIterator >
          inline int32_t Filter(
              InputIterator rtBeg, //!< [in] rt
              InputIterator rtEnd,
              std::vector<TReal> & rtfiltered //!< [out]
              )
          {
            size_t siz = std::distance(rtBeg,rtEnd);
            rtfiltered.resize(siz);
            std::transform(rtBeg,rtEnd,rtfiltered.begin(),Tfunctor<TReal>());
            return 0;
          }

          /*! \brief General Filter */
          int32_t operator()
          (
              const std::vector<TReal> & rt //!< [in] signal
              ,std::vector<TReal> & rtfiltered //!< [out] filtered signal
              )
          {
            int res = -1;
            res = Filter(&rt[0],&rt[0]+rt.size(),rtfiltered);
            return res;
          }
        };
      }
    }

  }
}


#endif // TRANSFORMFILTER_H
