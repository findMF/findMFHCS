// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT



#ifndef RT2SUM_H
#define RT2SUM_H

#include <stdint.h>

namespace ralab{

  //rt2sum
  class RT2sum{
    uint32_t rt2sum_;
  public:
    RT2sum(uint32_t rt2sum = 1):rt2sum_(rt2sum){}
    //get downsampled column;

    size_t getNrCols(size_t nrcols) const {
      size_t newnrcol = nrcols / rt2sum_;
      if(nrcols ==  newnrcol * rt2sum_){
          return newnrcol;
        }
      else{
          return ++newnrcol;
        }
    }

    //get column index
    size_t getCols(size_t nrcols) const {
      size_t newnrcol = nrcols/ rt2sum_;
      return newnrcol;
    }



  };
}
#endif // RT2SUM_H
