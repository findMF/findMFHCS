// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef FEATUREFIND_H
#define FEATUREFIND_H
#include <tbb/pipeline.h>

#include "findmf/algo/vigra/featurefinder.h"
#include "findmf/algo/vigra/ComputeFeatureStatistics.h"


namespace ralab{
  namespace findmf{
    /// find features in map
    struct FeatureFind : tbb::filter{

      double minitensity_;

      FeatureFind(double minitensity):tbb::filter(parallel),minitensity_(minitensity)
      {}

      void * operator()(void * image)
      {
        ralab::findmf::datastruct::LCMSImage * mp = static_cast<ralab::findmf::datastruct::LCMSImage *>(image);

        boost::timer time;
        time.restart();
        ralab::findmf::FeatureFinderLocalMax  ff;
        ralab::findmf::datastruct::FeaturesMap * map = new ralab::findmf::datastruct::FeaturesMap();
        map->setMapDescription(mp->getMapDescription());

        ff.findFeature( mp->getImageMap().getMap(), minitensity_ );

        ralab::findmf::ComputeFeatureStatistics cfs;
        cfs.extractFeatures(*map,mp->getImageMap().getMap(),ff.getLabels());
        delete mp;
        std::cerr << " Features Found in " << time.elapsed() << " [s]";
        return map;
      }
    };
  }
}

#endif

