// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef SIMPLEPICKER_H
#define SIMPLEPICKER_H

#include <iterator>
#include <vector>
#include <stdexcept>

namespace ralab{
  namespace base{
    namespace ms{
      /*! computes first derivative of a sequence, looks for zero crossings
       */
      template<class TReal>
      struct SimplePicker{
        std::vector<TReal> worker_;
        double epsilon_;

        SimplePicker(TReal epsilon = 1e-3):epsilon_(epsilon){}


        /*!
         *returns number of zero crossings found
         */
        template<typename Tit, typename Outit>
        size_t operator()(Tit beg, Tit end ,
                          Outit zerocrossings, //! picked peaks
                          std::ptrdiff_t lag = 2 //must be even (leave odd out)
            )
        {
          if((lag % 2 ) == 1){
              return -1;
            }

          worker_.resize(std::distance(beg,end) - lag);
          TReal * pworkerBeg = &worker_[0];
          TReal * pworkerEnd = &worker_[0] + worker_.size();

          Tit tbegin = beg;
          Tit tbeginm1 = tbegin + ( lag);
          for(;tbeginm1 != end ; ++tbeginm1, ++tbegin, ++pworkerBeg  )
            {
              *pworkerBeg = (*tbeginm1 - *tbegin);
            }

          //reset worker
          pworkerBeg = &worker_[0];
          std::size_t crosscount = 0;

          for( int i = 0 ; pworkerBeg != pworkerEnd-1 ; ++pworkerBeg , ++i )
            {

              TReal v1 = (*pworkerBeg);
              TReal v2 = *(pworkerBeg + 1);
              //peak detected ... detect a zero crossing
              if((v1 > 0 && v2 < 0) && ((v1 - v2) > epsilon_))
                {
                  //determine zero crossing....
                  double frac = v1 / ( v1 - v2 );
                  double idx = static_cast<float>(i + lag/2) + frac;
                  *zerocrossings = ( idx );
                  ++zerocrossings;
                  ++crosscount;
                }else if( v1 > 0 && v2  == 0 ){
                  TReal v3 = *(pworkerBeg + 2);
                  if((v3 < 0) && ((v1 - v3) > epsilon_)){
                      *zerocrossings = (i + lag/2 + 1.);
                    }
                }else{
                  //just continue, nothing to handle
                }
            }
          return crosscount;
        }

      };
    }//ms
  }//base
}//ralab

#endif // SIMPLEPICKER_H
