// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SQLFEATURESTORAGE_H
#define SQLFEATURESTORAGE_H

#include <string>
#include <boost/filesystem.hpp>
#include <glog/logging.h>

namespace ralab{

  template<typename Session>
  struct SQLFeatureStorage{

    Session db_;
    std::string dblocation_;

    //open resources
    SQLFeatureStorage(const std::string & dblocation):dblocation_(dblocation){
      openDB();
    }

    SQLFeatureStorage(const Session & rh):db_(rh.db_),dblocation_(rh.dblocation_)
    {}

    SQLFeatureStorage():db_(),dblocation_(){}


    //open resource (alternative constructor)
    SQLFeatureStorage(const std::string & output_directory,
                      const std::string & outfileprefix)
    {
      boost::filesystem::path p(output_directory);
      std::string outfile1 = outfileprefix;
      outfile1 +=".sqlite";
      boost::filesystem::path p1 = p;
      p1 /= outfile1;
      dblocation_ = p1.string();
      openDB();
    }

    //close resources RAII
    virtual ~SQLFeatureStorage(){
      LOG(INFO) << "db closing !" ;
    }

    virtual bool isOpen() = 0;

    virtual void openDB() = 0;

    Session getDatabase(){
      return db_;
    }

    std::string & getDBLocation(){
      return dblocation_;
    }

    // close db
    void closeDB(){
      db_.close();
    }

    //opens transaction
    void transaction(){
      db_.transaction();
    }


    void commit(){
      LOG(INFO) << "db committed";
      db_.commit();
    }

  };
}


#endif // SQLFEATURESTORAGE_H
