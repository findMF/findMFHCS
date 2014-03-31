/************************************************************************/
/*                                                                      */
/*               Copyright 1998-2002 by Ullrich Koethe                  */
/*                                                                      */
/*    This file is part of the VIGRA computer vision library.           */
/*    The VIGRA Website is                                              */
/*        http://hci.iwr.uni-heidelberg.de/vigra/                       */
/*    Please direct questions, bug reports, and contributions to        */
/*        ullrich.koethe@iwr.uni-heidelberg.de    or                    */
/*        vigra@informatik.uni-hamburg.de                               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */
/*                                                                      */
/************************************************************************/

#ifndef DIFFERENCEOFGAUSSIANS_H
#define DIFFERENCEOFGAUSSIANS_H

#include <functional>
#include "vigra/stdconvolution.hxx"
#include "vigra/separableconvolution.hxx"
#include "vigra/recursiveconvolution.hxx"
#include "vigra/nonlineardiffusion.hxx"
#include "vigra/combineimages.hxx"
#include "vigra/multi_shape.hxx"

namespace vigra {
  /********************************************************/
  /*                                                      */
  /*                 differenceOfGaussian                  */
  /*                                                      */
  /********************************************************/

  doxygen_overloaded_function(template <...> void laplacianOfGaussian)

  template <class SrcIterator, class SrcAccessor,
  class DestIterator, class DestAccessor>
  void differenceOfGaussian(SrcIterator supperleft,
                            SrcIterator slowerright, SrcAccessor sa,
                            DestIterator dupperleft, DestAccessor da,
                            double scale1, double scale2, double R12 = 1.6)
  {
    typedef typename
    NumericTraits<typename SrcAccessor::value_type>::RealPromote
        TmpType;
    BasicImage<TmpType> tmpBig(slowerright - supperleft, SkipInitialization),
        tmpSmall(slowerright - supperleft, SkipInitialization);

    Kernel1D<double> smooth11, smooth12, smooth21, smooth22;
    smooth11.initGaussian(scale1 * R12);
    smooth12.initGaussian(scale1);
    smooth21.initGaussian(scale2 * R12);
    smooth22.initGaussian(scale2);


    //smooth with larger kernel
    separableConvolveX(srcIterRange(supperleft, slowerright, sa),
                       destImage(tmpBig), kernel1d(smooth11));
    separableConvolveY(srcImageRange(tmpBig),
                       destImage(tmpBig), kernel1d(smooth21));

    //smooth with smaller kernel
    separableConvolveX(srcIterRange(supperleft, slowerright, sa),
                       destImage(tmpSmall), kernel1d(smooth12));
    separableConvolveY(srcImageRange(tmpSmall),
                       destImage(tmpSmall), kernel1d(smooth22));

    combineTwoImages(srcImageRange(tmpSmall), srcImage(tmpBig),
                     destIter(dupperleft, da), std::minus<TmpType>());
  }

  template <class SrcIterator, class SrcAccessor,
            class DestIterator, class DestAccessor>
  inline void
  differenceOfGaussian(triple<SrcIterator, SrcIterator, SrcAccessor> src,
                       pair<DestIterator, DestAccessor> dest,
                       double scale1, double scale2, double R12 = 1.6)
  {
    differenceOfGaussian(src.first, src.second, src.third,
                         dest.first, dest.second, scale1, scale2);
  }

  template <class T1, class S1,
            class T2, class S2>
  inline void
  differenceOfGaussian(MultiArrayView<2, T1, S1> const & src,
                       MultiArrayView<2, T2, S2> dest,
                       double scale1, double scale2 ,double R12 = 1.6)
  {
    vigra_precondition(src.shape() == dest.shape(),
                       "laplacianOfGaussian(): shape mismatch between input and output.");
    differenceOfGaussian(srcImageRange(src),
                         destImage(dest), scale1, scale2);
  }

}//vigra

#endif // DIFFERENCEOFGAUSSIANS_H
