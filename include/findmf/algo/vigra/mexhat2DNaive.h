#ifndef MEXHAT2DNAIVE_H
#define MEXHAT2DNAIVE_H

#include <vigra/stdconvolution.hxx>


namespace vigra{


  template <class T1, class S1,
            class T2, class S2>


  inline void

  mexicanHat2D(MultiArrayView<2, T1, S1> const & src,
                  MultiArrayView<2, T2, S2> dest,
                  double scale_x, double scale_y){
    gaussianSmoothing(srcImageRange(src), destImage(dest), scale_x, scale_y);

    // define horizontal Sobel filter
    vigra::Kernel2D<float> laplace2d;
    // upper left and lower right

    laplace2d.initExplicitly(Diff2D(-1,-1), Diff2D(1,1)) =
        0.  ,  -1., 0.,
        -1. ,  4., -1.,
        0.  , -1., -0.;

    //convolveImage(src, dest, laplace2d);
  }
}

#endif // MEXHAT2DNAIVE_H
