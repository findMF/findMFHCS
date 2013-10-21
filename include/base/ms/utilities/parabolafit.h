#ifndef PARABOLAFIT_H
#define PARABOLAFIT_H

#include <limits>
#include <math.h>

namespace ralab
{
  namespace base
  {
    namespace ms
    {
      namespace utilities
      {

        template<typename TReal>
        struct Parabolafit{
          TReal d_;
          TReal heigth_;
          TReal width_;
          TReal position_;

          Parabolafit(){}

          bool operator()(TReal l1, TReal l2, TReal l3){
            TReal d_ =  2 * l2 - l3 - l1;
            if( fabs(d_) < 3*std::numeric_limits<double>::epsilon() )
            {
              width_ = std::numeric_limits<double>::infinity();
              position_ = 0;
              heigth_ = 0;
              return false;
            }else{
              width_ = 2.355 * sqrt(1/d_);
              position_ = 0.5*(l3 - l1 )/d_;
              TReal l31 = (l3 - l1 );
              heigth_ = l2 + 0.125*l31*l31 /d_;
              return true;
            }
          }
        };

      }//utilities
    }//ms
  }//base
}//ralab







#endif



