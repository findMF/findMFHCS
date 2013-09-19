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

//#include "interfaces/ifeatureallaccess.h"

#include "findmf/fileio/sqlite/sqlfeaturetable.h"
#include "findmf/fileio/sqlite/sqlfeaturertree.h"
#include "findmf/fileio/sqlite/sqlmapinfotable.h"
#include "findmf/fileio/sqlite/sqlfeaturestorage.h"

#include "findmf/datastruct/featuresmap.h"


namespace ralab
{
  /** Concrete visitor */
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

    //prepare the insert
    void prepareInserts(){
      sftreetable_.prepareInsertAuto();
      smapinfotable_.prepareInsert();
      sftable_.prepareInsert();
    }

    void transaction(){
      sfstorage_.transaction();
    }

    void commit(){
      sfstorage_.commit();
    }

    //call it once per LCMS map
    void storeTableInfo(MapLCMSDescriptionPtr mlcmsd){
      mapid_ = smapinfotable_.insert(mlcmsd);
    }

    //visitor method which writes the features
    void visit(ralab::IFeatureAllAccess & feature) override {
      uint32_t runningid_ = sftreetable_.insertFeature(feature);
      sftable_.insertFeature(feature,runningid_,mapid_);
    }

    bool isValid(){
      return sfstorage_.db_.isValid();
    }

    ///
    ///\todo think to make it simpler.
    /// This method was required to protect write acces into sql lite databse
    ///
    void write2SQL( FeaturesMap & featuresMap )
    {
      boost::timer time;
      this->prepareInserts();
      this->storeTableInfo(featuresMap.getMapDescription());
      sfstorage_.transaction();

      FeaturesMap::Features::iterator beg, end;
      beg = featuresMap.features().begin();
      end = featuresMap.features().end();
      for(int i = 0;beg != end; ++beg, ++i){
          FeatureReadAdapterFM x(*beg, featuresMap.getMapDescription());
          uint32_t runningid_ = sftreetable_.insertFeature(x);
          sftable_.insertFeature(x,runningid_,mapid_);
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
#endif // FEATUREPRINTER_H
