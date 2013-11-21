// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SQLFEATURETABLESELECTS_H
#define SQLFEATURETABLESELECTS_H
#include <QtSql>
#include <boost/lexical_cast.hpp>
#include "findmf/interfaces/ifeaturewrite.h"

#include "base/base/cpplog.h"

namespace ralab{

  /**access feature table
  \todo
  */
  struct SQLFeatureTableSelects{
    QSqlDatabase db_;
    SQLFeatureTableSelects( QSqlDatabase db):db_(db){}
    //select all features with ID in ids
    //

    //\todo finish testing
    void getFeatures2(const std::vector<unsigned int> & ids)//, std::vector<Feature2D> & features )
    {

      QList<QVariant> qlist;
      for(std::size_t i = 0 ; i < ids.size() ; ++i){
          qlist.append(ids[i]);
        }

      QSqlQuery query(db_);

      QString queryString( "SELECT * from features WHERE id IN (:ids)");
      query.prepare(queryString);
      query.bindValue(":ids",qlist);
      if(!query.exec("SELECT * from features WHERE id IN (1,2,3)")){
          QSqlError err = query.lastError();
          std::cerr <<  err.text().toStdString() << std::endl;
        }
      while(query.next()){
          std::cerr << "print : " ;
          std::cerr << query.value(0).toUInt() << std::endl;
        }
    }

    //select all features with ID in ids
    //
    void getFeatures(const std::vector<unsigned int> & ids)//, std::vector<Feature2D> & features )
    {
      std::string x = "";
      for(std::size_t i = 0 ; i < ids.size()-1 ; ++i){
          x += boost::lexical_cast<std::string>(ids[i]);
          x += ",";
        }
      x+= boost::lexical_cast<std::string>(ids.back());
      //LOG_INFO << x << std::endl;

      QString b(x.c_str());
      //LOG_INFO << b.toStdString() << std::endl;

      QSqlQuery query(db_);

      QString queryString( "SELECT * from features WHERE id IN (:ids)");
      query.prepare(queryString);
      query.bindValue(":ids",b);
      if(!query.exec()){
          QSqlError err = query.lastError();
          std::cerr <<  "ERROR: " << err.text().toStdString() << std::endl;
        }
      while(query.next()){
          std::cerr << "print : " ;
          std::cerr << query.value(0).toUInt() << std::endl;
        }
    }


    //select all features with ID in ids
    //
    void getFeature(unsigned int id, ralab::IFeatureWrite & feature) //, std::vector<Feature2D> & features )
    {
      QString queryString( "SELECT id, idswath, max, count, volume, MZ, RT , mzProjection, rtProjection "
                          " from features WHERE id = :id");

      QSqlQuery query(db_);
      query.prepare(queryString);
      query.bindValue(":id",id);
      if(!query.exec()){
          QSqlError err = query.lastError();
          std::cerr <<  "ERROR: " << err.text().toStdString() << std::endl;
        }
      while(query.next()){

          //feature.setID( query.value(0).toUInt());
          feature.setID(query.value(0).toInt());
          feature.setSwathId(query.value(1).toInt());
          feature.setMaximum(query.value(2).toFloat());
          feature.setCount(query.value(3).toInt());
          feature.setVolume(query.value(4).toFloat());
          feature.setApexMZ(query.value(5).toFloat());
          feature.setApexRT(query.value(6).toFloat());
          QByteArray r = query.value(7).toByteArray();
          feature.setMZProjectionData(r.data(),r.size());
          r = query.value(8).toByteArray();
          feature.setRTProjectionData(r.data(),r.size());
        }
    }


  };

}

#endif // SQLFEATURETABLESELECTS_H
