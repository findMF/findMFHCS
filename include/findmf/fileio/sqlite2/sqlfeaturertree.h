// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SQLFEATURERTREE_H
#define SQLFEATURERTREE_H

#include <QtSql>
#include "base/cpplog.h"

#include "findmf/interfaces/ifeaturerangeaccess.h"



namespace ralab{

  struct SQLFeatureRTree{
    QSqlDatabase db_;
    QSqlQuery insertRTreeQuery_;

    SQLFeatureRTree(QSqlDatabase db):db_(db){}

    void setDB(QSqlDatabase db){
      db_ = db;
    }

    void createTable(){

      QSqlQuery query(db_);
      if(!query.exec("drop table if exists feature_index"))
      {
        QSqlError err = query.lastError();
        QString x = err.text();
        std::cerr << "Can' drop table feature_index : " << x.toStdString() << std::endl;
      }

      /*
      if(!query.exec("CREATE VIRTUAL TABLE feature_index USING rtree("
                     "id, "
                     "minMZsw, maxMZsw, "
                     "minRT, maxRT,"
                     "minMZ, maxMZ"
                     ");"
                     )){
        QSqlError err = query.lastError();
        std::cerr << "can't create table feature_index ON rtree : " << err.text().toStdString() << std::endl;
      */

      if (!query.exec("CREATE TABLE feature_index("
                      "id integer primary key, "
                      "minMZsw real, " // id in the current map
                      "maxMZsw real,"
                      "minRT real, "
                      "maxRT real, "
                      "minMZ real,"
                      "maxMZ real"
                      ");"
                      )){

        QSqlError err = query.lastError();
        std::cerr << "can't create table feature_index NO rtree : " << err.text().toStdString();
      }//second attempt
      //}//first attempt
    }

    ////
    unsigned int getLastInsertedID(){
      QString queryString ("SELECT last_insert_rowid() from feature_index");
      QSqlQuery query(db_);
      query.exec(queryString);
      if(query.next()){
        unsigned int id = query.value(0).toInt();
        return id;
      }
      else{
        return -1;
      }
    }

    ////
    void prepareInsertAuto(){
      QString queryString ("INSERT INTO feature_index  "
                           " ( minMZsw, maxMZsw, minRT, maxRT, minMZ, maxMZ) "
                           " VALUES(  :minMZsw, :maxMZsw, :minRT, :maxRT, :minMZ, :maxMZ)");
      QSqlQuery query(db_);
      query.prepare(queryString);
      insertRTreeQuery_ = query;
    }


    //@return
    uint32_t insertFeature(ralab::IFeatureRangeAccess & feature)
    {
      float minmzsw = feature.getMinMZsw();
      float maxmzsw = feature.getMaxMZsw();
      float minrt = feature.getMinRT();
      float maxrt = feature.getMaxRT();
      float minmz = feature.getMinMZ();
      float maxmz = feature.getMaxMZ();
      insertRTreeQuery_.bindValue(":minMZsw", minmzsw );
      insertRTreeQuery_.bindValue(":maxMZsw", maxmzsw );
      insertRTreeQuery_.bindValue(":minRT", minrt );
      insertRTreeQuery_.bindValue(":maxRT", maxrt );
      insertRTreeQuery_.bindValue(":minMZ", minmz );
      insertRTreeQuery_.bindValue(":maxMZ", maxmz );

      if(!insertRTreeQuery_.exec()){
        QSqlError err = insertRTreeQuery_.lastError();
        std::cerr <<  "can't insert into RTREE: " << err.text().toStdString() <<std::endl;
      }
      return insertRTreeQuery_.lastInsertId().toUInt();

    }

    //@return
    void prepareInsert(){
      QString queryString ("INSERT INTO feature_index  "
                           " (id, minMZsw, maxMZsw, minRT, maxRT, minMZ, maxMZ) "
                           " VALUES(:id,  :minMZsw, :maxMZsw, :minRT, :maxRT, :minMZ, :maxMZ)");
      QSqlQuery query(db_);
      query.prepare(queryString);
      insertRTreeQuery_ = query;
    }

    //@return
    void insertFeature(ralab::IFeatureRangeAccess & feature, uint32_t runid)
    {
      float minmzsw = feature.getMinMZsw();
      float maxmzsw = feature.getMaxMZsw();
      float minrt = feature.getMinRT();
      float maxrt = feature.getMaxRT();
      float minmz = feature.getMinMZ();
      float maxmz = feature.getMaxMZ();

      insertRTreeQuery_.bindValue(":id", runid );
      insertRTreeQuery_.bindValue(":minMZsw", minmzsw );
      insertRTreeQuery_.bindValue(":maxMZsw", maxmzsw );
      insertRTreeQuery_.bindValue(":minRT", minrt );
      insertRTreeQuery_.bindValue(":maxRT", maxrt );
      insertRTreeQuery_.bindValue(":minMZ", minmz );
      insertRTreeQuery_.bindValue(":maxMZ", maxmz );

      if(!insertRTreeQuery_.exec()){
        QSqlError err = insertRTreeQuery_.lastError();
        std::cerr <<  "can't insert into RTREE with id : " << err.text().toStdString() << std::endl;
      }

    }
  };// end of class

}//end ralab

#endif // SQLFEATURERTREE_H
