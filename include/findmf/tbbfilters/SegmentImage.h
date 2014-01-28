// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef SEGMENTIMAGE_H
#define SEGMENTIMAGE_H

#include <tbb/pipeline.h>

#include "findmf/algo/vigra/featurefinder.h"
#include "findmf/algo/vigra/ComputeFeatureStatistics.h"

namespace ralab{
  namespace findmf{
    /// filter performing image segmentation
    struct SegmentImage : tbb::filter{
      double minitensity_;

      SegmentImage(double minitensity):tbb::filter(parallel),minitensity_(minitensity)
      {}
      void * operator()(void * imagep)
      {
        ralab::findmf::datastruct::LCMSImage * image = static_cast<ralab::findmf::datastruct::LCMSImage *>(imagep);

        boost::timer time;
        time.restart();
        ralab::findmf::FeatureFinder ff;
        ralab::findmf::datastruct::FeaturesMap * map = new ralab::findmf::datastruct::FeaturesMap();
        map->setMapDescription(image->getMapDescription());

        ff.findFeature( image->getImageMap().getMap(), minitensity_ );

        ralab::findmf::ComputeFeatureStatistics cfs;
        cfs.extractFeatures(*map,image->getImageMap().getMap(),ff.getLabels());
        delete image;
        std::cerr << " Features Found in " << time.elapsed() << " [s]";
        return map;
      }
    };
  }
}

#endif
