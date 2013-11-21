// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SQLFEATURESTORAGE_H
#define SQLFEATURESTORAGE_H

#include <string>
#include <QtSql>
#include <boost/filesystem.hpp>
#include "base/base/cpplog.h"
namespace ralab{

  struct SQLFeatureStorage{

    QSqlDatabase db_;
    std::string dblocation_;

    //open resources
    SQLFeatureStorage(const std::string & dblocation):dblocation_(dblocation){
      openDB();
    }

    SQLFeatureStorage(const SQLFeatureStorage & rh):db_(rh.db_),dblocation_(rh.dblocation_)
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
    ~SQLFeatureStorage(){
      db_.close();
    }

    bool isOpen(){
      return db_.open();
    }

    void openDB(){
      db_ = QSqlDatabase::addDatabase("QSQLITE");
      QString dbloc(dblocation_.c_str());
      db_.setDatabaseName(dbloc);
      if (!db_.open())
        {
          throw std::runtime_error("can't open db");
        }
    }

    QSqlDatabase getDatabase(){
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
      db_.commit();
    }

  };
}


#endif // SQLFEATURESTORAGE_H
