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
/*                 laplacianOfGaussian                  */
/*                                                      */
/********************************************************/

doxygen_overloaded_function(template <...> void laplacianOfGaussian)





template <class SrcIterator, class SrcAccessor,
          class DestIterator, class DestAccessor>
void laplacianOfGaussian(SrcIterator supperleft,
                        SrcIterator slowerright, SrcAccessor sa,
                        DestIterator dupperleft, DestAccessor da,
                        double scale1, double scale2 = 1.6*)
{
    typedef typename
        NumericTraits<typename SrcAccessor::value_type>::RealPromote
        TmpType;
    BasicImage<TmpType> tmp(slowerright - supperleft, SkipInitialization),
                        tmpx(slowerright - supperleft, SkipInitialization),
                        tmpy(slowerright - supperleft, SkipInitialization);

    Kernel1D<double> smooth, deriv;
    smooth.initGaussian(scale);
    deriv.initGaussianDerivative(scale, 2);

    separableConvolveX(srcIterRange(supperleft, slowerright, sa),
                       destImage(tmp), kernel1d(deriv));
    separableConvolveY(srcImageRange(tmp),
                       destImage(tmpx), kernel1d(smooth));
    separableConvolveX(srcIterRange(supperleft, slowerright, sa),
                       destImage(tmp), kernel1d(smooth));
    separableConvolveY(srcImageRange(tmp),
                       destImage(tmpy), kernel1d(deriv));
    combineTwoImages(srcImageRange(tmpx), srcImage(tmpy),
                       destIter(dupperleft, da), std::plus<TmpType>());
}

template <class SrcIterator, class SrcAccessor,
          class DestIterator, class DestAccessor>
inline void
laplacianOfGaussian(triple<SrcIterator, SrcIterator, SrcAccessor> src,
                    pair<DestIterator, DestAccessor> dest,
                    double scale)
{
    laplacianOfGaussian(src.first, src.second, src.third,
                        dest.first, dest.second, scale);
}

template <class T1, class S1,
          class T2, class S2>
inline void
laplacianOfGaussian(MultiArrayView<2, T1, S1> const & src,
                    MultiArrayView<2, T2, S2> dest,
                    double scale)
{
    vigra_precondition(src.shape() == dest.shape(),
        "laplacianOfGaussian(): shape mismatch between input and output.");
    laplacianOfGaussian(srcImageRange(src),
                        destImage(dest), scale);
}


#endif // DIFFERENCEOFGAUSSIANS_H
