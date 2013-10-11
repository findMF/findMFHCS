// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SQLFACADE_H
#define SQLFACADE_H
#include <string>
#include <tbb/tbb.h>

#include "findmf/fileio/sqlite2/featuresmapsqlwriter.h"

namespace ralab{
  namespace findmf{

    struct FeaturesMapSQLWriterFacade{
      typedef tbb::spin_mutex FreeListMutexType;
      FreeListMutexType freeListMutex_;
      std::string db_location_;
      std::string db_name_;

      FeaturesMapSQLWriterFacade(const std::string & folder,
                                 const std::string & stem):freeListMutex_(),
        db_location_(folder), db_name_(stem)
      {}

      void createDatabase(){
        FeaturesMapSQLWriter fw(db_location_,db_name_);
        fw.createTables();
      }

      void writeSQL2(datastruct::FeaturesMap & featuresMap){
        FreeListMutexType::scoped_lock lock(freeListMutex_);
        FeaturesMapSQLWriter fw(db_location_,db_name_);
        fw.write(featuresMap);
      }

    };
  }
}

#endif // SQLFACADE_H
