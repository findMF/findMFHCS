// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT



#ifndef VIGRATYPEDEFS_H
#define VIGRATYPEDEFS_H
#include "vigra/accumulator.hxx"


namespace ralab{
  namespace findmf{
    using namespace vigra::acc;

    typedef vigra::CoupledIteratorType<2, float, int>::type Iterator;
    typedef Iterator::value_type Handle;

    //typedef DivideByCount<Principal<PowerSum<2> > > PrincipalVariance;
    //typedef DivideByCount<Principal<Sum> > PrincipalMean;

    typedef AccumulatorChainArray<Handle,
    Select< DataArg<1>, LabelArg<2>, //WeightArg<3>,       //where to look in the Handle (coordinates are always arg 0)
    Mean, Variance,                    //per-region statistics over values
    Coord<Mean>,      //per-region statistics over coordinates
    Coord<Minimum>,
    Coord<Maximum>,
    RegionCenter,
    CenterOfMass,
    Maximum,
    //UnbiasedSkewness,
    //UnbiasedKurtosis,
    //StdDev,
    Coord<ArgMaxWeight>
    //RegionAxes,
    //Select<
    //RegionRadii, //
    //MomentsOfInertia, //
    //Weighted<RegionAxes>,
    //Global<Mean>, Global<Variance>
    //>
    > > //global statistics
    MyAccumulatorChain;

    //typedef LookupTag<RegionRadii, MyAccumulatorChain>::value_type RegionRadiiReturn;
    //typedef LookupTag<MomentsOfInertia, MyAccumulatorChain>::value_type MomentsOfInertiaReturn;
  }
}
#endif // VIGRATYPEDEFS_H
