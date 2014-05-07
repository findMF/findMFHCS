// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

//#include <boost/test/included/unit_test.hpp>
#define BOOST_TEST_DYN_LINK
//#define BOOST_TEST_MODULE Hello
#define BOOST_TEST_MODULE Fixtures
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <QtSql>
#include <iostream>
#include <stdexcept>

// The fixture for testing class Foo.
class SQLFeatureStorageTestHelp {
protected:
    //std::vector<double> data , dcumsum , res , res2;

    // You can remove any or all of the following functions if its body
    // is empty.
    // std::string dir_;
    boost::filesystem::path dir;
    SQLFeatureStorageTestHelp():dir("./testdata"){
        if(boost::filesystem::create_directory(dir)) {
            std::cout << "Success" << std::endl;
        }
        // You can do set-up work for each test here.
    }
    ~SQLFeatureStorageTestHelp() {
        // You can do clean-up work that doesn't throw exceptions here.
        boost::filesystem::remove_all(dir);
    }
};
BOOST_FIXTURE_TEST_SUITE(testRTree , SQLFeatureStorageTestHelp)

BOOST_AUTO_TEST_CASE(testRtree){
  QSqlDatabase db_ = QSqlDatabase::addDatabase("QSQLITE");
  boost::filesystem::path dir2 =  dir /"testRTree.sql";
  QString dblocQ(dir2.string().c_str());
  db_.setDatabaseName(dblocQ);

  if (!db_.open())
    {
      throw std::runtime_error("can't open db");
    }

  QSqlQuery query(db_);
    if(!query.exec("drop table if exists feature_index"))
    {
      QSqlError err = query.lastError();
      QString x = err.text();
      std::cout << "Can' drop table feature_index : " << x.toStdString() << std::endl;
    }

  if(!query.exec("CREATE VIRTUAL TABLE feature_index USING rtree("
                 "id, "
                 "minMZsw, maxMZsw, "
                 "minRT, maxRT,"
                 "minMZ, maxMZ"
                 ");"
                 )){
      QSqlError err = query.lastError();
      std::cout << "can't create table feature_index ON rtree : " << err.text().toStdString() << std::endl;

    }//first attempt
}

BOOST_AUTO_TEST_SUITE_END()

