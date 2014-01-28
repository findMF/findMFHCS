// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef WRITESQLFEATURES_H
#define WRITESQLFEATURES_H

#include <tbb/pipeline.h>

#include "findmf/fileio/sqlite2/featuresmapsqlwriterfacade.h"

namespace ralab{
  namespace findmf{

    struct WriteSQLFeatures : tbb::filter{
      ralab::findmf::FeaturesMapSQLWriterFacade facade_;
      bool writeprojections_;
      WriteSQLFeatures(const std::string & outdir,
                       const std::string & filestem,
                       bool writeprojections = true
          ) : tbb::filter(serial_in_order), facade_( outdir , filestem ),writeprojections_(writeprojections)
      {}

      void * operator()(void * map)
      {
        ralab::findmf::datastruct::FeaturesMap * fm =
            static_cast<ralab::findmf::datastruct::FeaturesMap *>( map );
        boost::timer time;
        //LOG(INFO) << " start writing SQL" ;
        facade_.writeSQL2(*fm, writeprojections_ );
        //LOG(INFO) << " Features Written in " << time.elapsed() << " [s]";
        delete fm;
        return NULL;
      }
    };
  }
}

#endif
