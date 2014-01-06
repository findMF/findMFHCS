// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef FEATURESQLPRINTER_H
#define FEATURESQLPRINTER_H

#include <ostream>
#include <iostream>
#include <boost/timer.hpp>
#include <boost/shared_ptr.hpp>


#include "findmf/fileio/sqlite2/sqlfeaturetable.h"
#include "findmf/fileio/sqlite2/sqlfeaturertree.h"
#include "findmf/fileio/sqlite2/sqlmapinfotable.h"
#include "findmf/fileio/sqlite2/sqlfeaturestorage.h"

#include "findmf/datastruct/featuresmap.h"


namespace ralab
{
  namespace findmf
  {
    class FeaturesMapSQLWriter //: public IFeatureVistor
    {
      SQLFeatureStorage sfstorage_;
      SQLFeatureTable sftable_;
      SQLFeatureRTree sftreetable_;
      SQLMapInfoTable smapinfotable_;
      uint32_t mapid_;

    public:
      FeaturesMapSQLWriter():sfstorage_(),
        sftable_( sfstorage_.getDatabase() ),
        sftreetable_(sfstorage_.getDatabase()),
        smapinfotable_(sfstorage_.getDatabase()),
        mapid_(0)
      {}

      FeaturesMapSQLWriter(const std::string & filename)
        :sfstorage_(filename),
          sftable_( sfstorage_.getDatabase() ),
          sftreetable_(sfstorage_.getDatabase()),
          smapinfotable_(sfstorage_.getDatabase()),
          mapid_(0)
      {}

      FeaturesMapSQLWriter(const std::string & ouddir, const std::string & outfileprefix)
        :sfstorage_(ouddir,outfileprefix),
          sftable_( sfstorage_.getDatabase() ),
          sftreetable_(sfstorage_.getDatabase()),
          smapinfotable_(sfstorage_.getDatabase()),
          mapid_(0)
      {}

      FeaturesMapSQLWriter(SQLFeatureStorage & sfs):sfstorage_(sfs),
        sftable_( sfstorage_.getDatabase() ),
        sftreetable_(sfstorage_.getDatabase()),
        smapinfotable_(sfstorage_.getDatabase()),
        mapid_(0)
      {}

      std::string & getDBLocation(){
        return sfstorage_.getDBLocation();
      }


      //create tables
      void createTables(){
        sftable_.createTable();
        sftreetable_.createTable();
        smapinfotable_.createTable();
      }

    private:

      //prepare the insert

      void prepareInserts(){
        sftreetable_.prepareInsertAuto();
        smapinfotable_.prepareInsert();
        sftable_.prepareInsert();
      }


      void commit(){
        sfstorage_.commit();
      }


      //call it once per LCMS map
      void storeTableInfo(datastruct::MapLCMSDescriptionPtr mlcmsd){
        mapid_ = smapinfotable_.insert(mlcmsd);
      }

    public:
      ///
      /// TODO think to make it simpler.
      /// This method was required to protect write acces into sql lite databse
      ///
      void write( datastruct::FeaturesMap & featuresMap, bool writeprojection = true )
      {
        boost::timer time;
        this->prepareInserts();
        this->storeTableInfo(featuresMap.getMapDescription());
        sfstorage_.transaction();

        datastruct::FeaturesMap::Features::iterator beg, end;
        beg = featuresMap.features().begin();
        end = featuresMap.features().end();
        for(int i = 0;beg != end; ++beg, ++i){
            datastruct::FeatureReadAdapterFM x(*beg, featuresMap.getMapDescription());
            uint32_t runningid_ = sftreetable_.insertFeature(x);
            sftable_.insertFeature(x,runningid_,mapid_,writeprojection);
          }
        sfstorage_.commit();
      }

      //check if storage open
      bool isOpen(){
        return sfstorage_.isOpen();
      }

      ~FeaturesMapSQLWriter(){
        //sfstorage_.closeDB();
      }
    };

    typedef boost::shared_ptr<FeaturesMapSQLWriter> FeaturesMapSQLWriterPtr;
  }
}
#endif // FEATUREPRINTER_H
