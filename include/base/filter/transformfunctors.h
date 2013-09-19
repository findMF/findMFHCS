// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef TRANSFORMFUNCTORS_H
#define TRANSFORMFUNCTORS_H
namespace ralab{
  namespace base{
    namespace filter{

      template<class TReal>
      struct Sqrt : std::unary_function<TReal,TReal>
      {
        TReal operator()(TReal x) const{
          return sqrt(x);
        }
      };

      template<class TReal>
      struct Pow : std::unary_function<TReal,TReal>
      {
        TReal operator()(TReal x) const {
          return x*x;
        }
      };
    }//filter
  }//base
}//ralab

#endif // TRANSFORMFUNCTORS_H
