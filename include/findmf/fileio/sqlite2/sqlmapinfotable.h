// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SQLMAPINFOTABLE_H
#define SQLMAPINFOTABLE_H

#include <QtSql>
#include "findmf/datastruct/maplcmsdescription.h"
#include "base/cpplog.h"

namespace ralab{
  namespace findmf{
    /** Table to store map information */
    struct SQLMapInfoTable{
      QSqlDatabase db_;
      QSqlQuery insertMapQuery_;
      SQLMapInfoTable( QSqlDatabase db):db_(db){}


      void setDB(QSqlDatabase db){
        db_ = db;
      }

      //drops the table first;
      void createTable(){
        QSqlQuery query(db_);
        if( !query.exec("drop table if exists mapinfo") )
          {
            QSqlError err = query.lastError();
            QString x = err.text();
            //LOG_INFO  << "SQLMapInfoTable" << x.toStdString() << std::endl;
          }
        if(!query.exec("CREATE TABLE mapinfo ( "
                       "id integer primary key, "
                       "mslevel int, "
                       "minMZsw float,"
                       "maxMZsw float,"
                       "minMZ float ,"
                       "maxMZ flaot ,"
                       "minRT float ,"
                       "maxRT float ,"
                       "rts blob,"
                       "mzs blob"
                       ")")){
            QSqlError err = query.lastError();
            std::cerr << err.text().toStdString() << std::endl;
          }
      }

      void prepareInsert(){
        QString queryString ("INSERT INTO mapinfo "
                             "( mslevel, minMZsw, maxMZsw, minMZ, maxMZ, minRT, maxRT, rts, mzs) "
                             " VALUES ( :mslevel, :minMZsw,:maxMZsw,:minMZ,:maxMZ,:minRT,:maxRT,:rts,:mzs);");

        /*
      QString queryString ("INSERT INTO mapinfo "
                           "( mslevel, minMZsw, maxMZsw, minMZ, maxMZ, minRT, maxRT) "
                           " VALUES ( :mslevel, :minMZsw,:maxMZsw,:minMZ,:maxMZ,:minRT,:maxRT);");

      */
        insertMapQuery_.prepare(queryString);
      }

      ///
      /// just for testing
      ///
      void insertFeature2(){
        insertMapQuery_.bindValue(":mslevel" , 1);

        insertMapQuery_.bindValue(":minMZsw" , 10.1);
        insertMapQuery_.bindValue(":maxMZsw" , 12.3);

        insertMapQuery_.bindValue(":minMZ" , 13.);
        insertMapQuery_.bindValue(":maxMZ", 15.);

        insertMapQuery_.bindValue(":minRT" , 100.);
        insertMapQuery_.bindValue(":maxRT" , 300.);
        if(!insertMapQuery_.exec()){
            QSqlError err = insertMapQuery_.lastError();
            //LOG_INFO << "insert Mapinfo: " << err.text().toStdString() ;
          }
      }

      void executeTestQuery(){
        QString testQuery("INSERT INTO mapinfo "
                          "( mslevel, minMZsw, maxMZsw, minMZ, maxMZ, minRT, maxRT) "
                          " VALUES ( 1 , 120.,145.,120.,1400.3,10,3600)");
        if(!insertMapQuery_.exec(testQuery)){
            QSqlError err = insertMapQuery_.lastError();
            //LOG_INFO << "insert Mapinfo: " << err.text().toStdString() ;
          }
      }

      //insert map description into table
      // return id of this insert
      uint32_t insert(datastruct::MapLCMSDescriptionPtr mapdescript){

        insertMapQuery_.bindValue(":mslevel" , mapdescript->getMSLevel());

        insertMapQuery_.bindValue(":minMZsw" , mapdescript->getMinMZsw());
        insertMapQuery_.bindValue(":maxMZsw" , mapdescript->getMaxMZsw());

        insertMapQuery_.bindValue(":minMZ" , mapdescript->getMinMZ());
        insertMapQuery_.bindValue(":maxMZ", mapdescript->getMaxMZ());

        insertMapQuery_.bindValue(":minRT" , mapdescript->getMinRT());
        insertMapQuery_.bindValue(":maxRT" , mapdescript->getMaxRT());

        //projections
        char * x;
        std::size_t size;
        x = mapdescript->getRTAxis(size);

        QByteArray baa(x, size);
        insertMapQuery_.bindValue(":rts", baa );

        x = mapdescript->getMZAxis(size);
        QByteArray bab(x, size);
        insertMapQuery_.bindValue(":mzs", bab );

        if(!insertMapQuery_.exec()){
            QSqlError err = insertMapQuery_.lastError();
            //LOG_INFO << "insert Mapinfo: " << err.text().toStdString() ;
          }
        return insertMapQuery_.lastInsertId().toUInt();
      }
    };//end map info
  }
}

#endif // SQLMAPINFOTABLE_H
