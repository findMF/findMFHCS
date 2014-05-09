// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SQLFEATURERTREESELECT_H
#define SQLFEATURERTREESELECT_H

#include <QtSql>
#include "base/cpplog.h"

namespace ralab{

/** select statements on Rtrees */
struct SQLFeatureRTreeSelect{
    QSqlDatabase db_;

    SQLFeatureRTreeSelect( QSqlDatabase db):db_(db){}
    //
    void getMinRTS( std::vector<float> & prem){
        cpplog::FileLogger log( "log.txt", true );
        QString queryString3( "SELECT minRT FROM feature_index");
        QSqlQuery query(db_);
        if(!query.exec(queryString3)){
            QSqlError err = query.lastError();
            LOG_INFO(log) <<  "Cant select from feature_index : " << err.text().toStdString() <<std::endl;
        }
        while(query.next()){
            prem.push_back(query.value(0).toFloat());
        }
    }

    /// select ids of all features intersecting with rt range
    void getIDs(float rtStart, float rtEnd, std::vector<uint32_t> & prem){
        QString queryString3( "SELECT id FROM feature_index WHERE  maxRT >= :QMinRT AND minRT <= :QMaxRT");

        QSqlQuery query(db_);
        query.prepare(queryString3);
        query.bindValue(":QMinRT",rtStart);
        query.bindValue(":QMaxRT",rtEnd);

        if(!query.exec()){
            QSqlError err = query.lastError();
            std::cerr << "Cant select from feature_index : " << err.text().toStdString();
        }
        while(query.next()){
            prem.push_back(query.value(0).toUInt());
        }
    }

    /// select ids of all features intersecting with mz and rt range
    void getIDs(float mzStart, float mzEnd, float rtStart, float rtEnd, std::vector<uint32_t> & prem){
        QString queryString3( "SELECT id FROM feature_index WHERE"
                              " maxRT >= :QMinRT AND minRT <= :QMaxRT"
                              " AND maxMZ >= :QMinMZ AND minMZ <= :QMaxMZ"
                              );
        QSqlQuery query(db_);
        query.prepare(queryString3);

        query.bindValue(":QMinRT",rtStart);
        query.bindValue(":QMaxRT",rtEnd);

        query.bindValue(":QMinMZ",mzStart);
        query.bindValue(":QMaxMZ",mzEnd);

        if(!query.exec()){
            QSqlError err = query.lastError();
            std::cerr <<  "Cant select from feature_index : " << err.text().toStdString();
        }
        while(query.next()){
            prem.push_back(query.value(0).toUInt());
        }
    }

    // Select all feature interesecting with rt
    void getIDs(float rt, std::vector<uint32_t> & prem){
        QString queryString( "SELECT id FROM feature_index WHERE minRT <= :rtVal AND maxRT >= :rtVal2");

        QSqlQuery query(db_);
        query.prepare(queryString);
        query.bindValue(":rtVal",rt);
        query.bindValue(":rtVal2",rt);

        if(!query.exec()){
            QSqlError err = query.lastError();
            std::cerr <<  "Cant select from feature_index : " << err.text().toStdString();
        }
        while(query.next()){
            prem.push_back(query.value(0).toUInt());
        }
    }

};
}


#endif // SQLFEATURERTREESELECT_H
