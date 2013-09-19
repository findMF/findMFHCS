// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

//#include <boost/test/included/unit_test.hpp>

#include <QtSql>
#include <iostream>
#include <stdexcept>


inline void testMapInfoTable(std::string dbloc){
  QSqlDatabase db_ = QSqlDatabase::addDatabase("QSQLITE");
  QString dblocQ(dbloc.c_str());
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



int main(int arg, char ** argv)
{
  std::string dbname = "./mytest.sqlite";
  testMapInfoTable(dbname);
  return 0;
}
