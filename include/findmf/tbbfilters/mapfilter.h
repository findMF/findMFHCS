// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef MAPFILTER_H
#define MAPFILTER_H

#include <tbb/pipeline.h>
#include "findmf/algo/vigra/lcmsimagefilter.h"


namespace ralab{
namespace findmf{
  //second stage filtering...
  struct MapFilter:tbb::filter{
    uint32_t mzpixelwidth_;
    uint32_t rtpixelwidth_;
    double mzscale_;
    double rtscale_;

    MapFilter(uint32_t mzpixwidth, uint32_t rtpixwidth, double mzscale=1., double rtscale=1.):tbb::filter(parallel),
      mzpixelwidth_(mzpixwidth),rtpixelwidth_(rtpixwidth),mzscale_(mzscale), rtscale_(rtscale)
    {
    }

    void * operator()(void * queue)
    {
      ralab::findmf::datastruct::LCMSImage * mp = static_cast<ralab::findmf::datastruct::LCMSImage *>(queue);
      ralab::findmf::LCMSImageFilterGauss imgf;
      imgf.filter( mp->getImageMap().getMap() , mzpixelwidth_ , rtpixelwidth_ , mzscale_, rtscale_);
      mp->getImageMap().updateImageRange();
      //std::cerr << " Image Filtered in : " << time.elapsed() << " [s]";
      return mp;
    }
  };

}
}

#endif
