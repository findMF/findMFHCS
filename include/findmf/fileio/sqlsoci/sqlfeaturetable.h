// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SQLFEATUREFILESOCI_H
#define SQLFEATUREFILESOCI_H

#include <stdexcept>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <soci/soci.h>
#include <glog/logging.h>

#include "findmf/interfaces/ifeatureaccess.h"
#include "findmf/fileio/sqlsoci/sqlfindmfstorage.h"

namespace ralab{
  // Table with most relevant feature statistics
  // \todo remove the table from the name.
  struct SQLFeatureTable{
    SQLFindMFStorage & db_;
    //QSqlQuery insertFeatureQuery_;

    SQLFeatureTable( SQLFindMFStorage & db):db_(db){}


    //drops the table first;
    void createTable(){
      QSqlQuery query(db_);
      try
      {
        db_.session() << "drop table if exists features";

      } catch(std::exception & e)
      {
        LOG(ERROR) << " features table drop failed :  " << __FILE__ << __LINE__ << x.toStdString() << std::endl;
        LOG(ERROR) << e.what();
      }

      try
      {
        db_.session() << "CREATE TABLE features ( "
                     "id integer primary key, "
                     "idmap int, " // id in the current map
                     "idswath int,"
                     "centerOfMassMZ real, "
                     "centerOfMassRT real, "
                     "MZ real, "
                     "RT real ,"
                     "MZSD real, "
                     "RTSD real, "
                     "MZSKEW real, "
                     "RTSKEW real, "
                     "MZKURT real, "
                     "RTKURT real, "
                     "Max real, "
                     "Count integer, "
                     "Volume real, "
                     "maximumLocationMZ real, "
                     "maximumLocationRT real, "
                     "minMZIndex real, "
                     "mzExtend real, "
                     "minRTIndex real, "
                     "rtExtend real, "
                     "mzProjection blob , "
                     "rtProjection blob"
                     ")";
      }catch(std::exception &e){
        LOG(ERROR) << " features table create failed:  " << __FILE__ << __LINE__ << x.toStdString() << std::endl;
        LOG(ERROR) << e.what();
      }
    }

    void prepareInsert()
    {
      std::string insert = "INSERT INTO features "
                           "(id, idmap, idswath,  max, count,volume,"
                           "MZ , RT , centerOfMassMZ , centerOfMassRT , maximumLocationMZ , maximumLocationRT, "
                           "MZSD , RTSD , MZSKEW , RTSKEW , MZKURT , RTKURT ,"
                           "minMZIndex,mzExtend,minRTIndex,rtExtend,mzProjection, rtProjection )"
                           " VALUES (:id, :idmap, :idswath,  :max, :count,:volume,"
                           ":mz,:rt,:commz,:commrt,:maxlocmz,:maxlocrt,"
                           " :mzsd, :rtsd, :mzskew, :rtskew, :mzkurt, :rtkurt,"
                           " :mzminidx, :extmz, :rtminidx, :extrt, :mzproj, :rtproj "
                           ")";
    }

    void insertFeature(ralab::IFeatureAccess & feature,//!
                       uint32_t id ,//!< optional id which you get from the rtree!
                       uint32_t mapid_
                       )
    {
      insertFeatureQuery_.bindValue(":id",id); // running id in the rtree table.
      insertFeatureQuery_.bindValue(":idmap",feature.getID()); // id within map.
      insertFeatureQuery_.bindValue(":idswath",mapid_); // id in the map table;
      insertFeatureQuery_.bindValue(":max" , feature.getMaximum());
      insertFeatureQuery_.bindValue(":count" , feature.getCount());
      insertFeatureQuery_.bindValue(":volume" , feature.getVolume());

      insertFeatureQuery_.bindValue(":mz",feature.getApexMZ());
      insertFeatureQuery_.bindValue(":rt",feature.getApexRT());
      insertFeatureQuery_.bindValue(":commz",feature.getCenterOfMassMZ());
      insertFeatureQuery_.bindValue(":comrt",feature.getCenterOfMassRT());
      insertFeatureQuery_.bindValue(":maxlocmz" , feature.getMaxLocationMZ());
      insertFeatureQuery_.bindValue(":maxlocrt" , feature.getMaxLocationRT());

      //distribution infromation
      insertFeatureQuery_.bindValue(":mzsd",feature.getSDMZ());
      insertFeatureQuery_.bindValue(":rtsd",feature.getSDRT());
      insertFeatureQuery_.bindValue(":mzskew",feature.getSkewnessMZ());
      insertFeatureQuery_.bindValue(":rtskew",feature.getSkewnessRT());
      insertFeatureQuery_.bindValue(":mzkurt",feature.getKurtosisMZ());
      insertFeatureQuery_.bindValue(":rtkurt",feature.getKurtosisRT());

      //bounding box and bounding box location
      insertFeatureQuery_.bindValue(":mzminidx",feature.getMinMZIdx());
      insertFeatureQuery_.bindValue(":extmz",feature.getMZExtend());
      insertFeatureQuery_.bindValue(":rtminidx",feature.getMinRTIdx());
      insertFeatureQuery_.bindValue(":extrt",feature.getRTExtend());


      //projections
      char * x;
      std::size_t size;
      x = feature.getMZProjectionData(size);

      QByteArray baa(x, size);
      insertFeatureQuery_.bindValue(":mzproj", baa );

      x = feature.getRTProjectionData(size);
      QByteArray bab(x, size);
      insertFeatureQuery_.bindValue(":rtproj", bab );

      if(!insertFeatureQuery_.exec()){
          QSqlError err = insertFeatureQuery_.lastError();
          LOG(ERROR) << " insert Feature: " << err.text().toStdString() ;
        }
    }
  };//end of class
  //   SELECT * FROM stuff WHERE id IN (SELECT id FROM feature_index WHERE value < :value)
}


#endif // SQLFEATUREFILE_H
