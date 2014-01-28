// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef WRITEIMAGE_H
#define WRITEIMAGE_H

#include "findmf/datastruct/lcmsimage.h"
#include <tbb/pipeline.h>

namespace ralab
{
  namespace findmf
  {

    // Write Images
    struct WriteImage : tbb::filter{
      std::string outdir_ ; //!< outdir
      WriteImage(const std::string  & outdir ) : tbb::filter(serial_in_order), outdir_(outdir)
      {}

      /// writes featues into outdir
      void * operator()(void * image)
      {
        ralab::findmf::datastruct::LCMSImage * mp = static_cast<ralab::findmf::datastruct::LCMSImage *>(image);
        mp->write(outdir_);
        delete mp;
        return NULL;
      }
    };

  }//findm
}//ralab


#endif // WRITEIMAGE_H
