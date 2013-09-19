// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

void MMap2::test(){
  using namespace vigra;
  MultiArray<2, float> labels(mp_.shape()), gradient(mp_.shape());

  vigra::FindMinMax<Map::value_type> minmax;   // functor to find range
  inspectImage(srcImageRange(mp_), minmax); // find original range
  std::cout << "image maximum" <<  minmax.max << std::endl;

  transformImage( srcImageRange(mp_) , destImage(gradient),
                  linearIntensityTransform(
                    -1., // scaling
                    -(minmax.max ) )
                  );

  /*
  transformImage( srcImageRange(labels) , destImage(mp_),
                        linearIntensityTransform(
                          -1., // scaling
                          -(minmax.max ) )
                  );*/

  ///inspectImage(srcImageRange(labels), minmax);
  ///std::cout << "image max" << minmax.min << " " << minmax.max << std::endl;


  /*
  gaussianGradientMagnitude(srcImageRange(mp_),
                            destImage(gradient),
                            scale); */
  //mp_ =gradient;

  generateWatershedSeeds(
        vigra::srcImageRange(gradient),
        destImage(labels), // seeds: minima below 2.0
        SeedOptions()
        );


  watershedsRegionGrowing(srcImageRange(gradient),
                          destImage(labels), //seeds will be overwritten
                          FourNeighborCode(),// use 4-neighborhood
                          WatershedOptions().keepContours()
                          );

  inspectImage(srcImageRange(labels), minmax);
  std::cout << "image max" << minmax.min << " " << minmax.max << std::endl;
  maxelem_ = minmax.max;
  mp_ = labels;

}

