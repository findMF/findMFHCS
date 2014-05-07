// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Fixtures
#include <boost/test/unit_test.hpp>

//#include <boost/test/included/unit_test.hpp>
#include <boost/timer.hpp>
#include <boost/filesystem.hpp>

#include "findmf/datastruct/feature2d.h"
#include "findmf/datastruct/rangefeature.h"
#include "findmf/datastruct/featuresmap.h"
#include "findmf/datastruct/featurewriteadapter.h"

#include "findmf/fileio/spectrastreader.h"
#include "findmf/fileio/sqlite2/sqlfeaturetableselects.h"
#include "findmf/fileio/sqlite2/sqlfeaturertreeselect.h"

#include "findmf/fileio/sqlite2/sqlfeaturetable.h"
#include "findmf/fileio/sqlite2/sqlfeaturertree.h"
#include "findmf/fileio/sqlite2/featuresmapsqlwriter.h"


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
            std::cout << "Success" << "\n";
        }
        // You can do set-up work for each test here.
    }
    ~SQLFeatureStorageTestHelp() {
        // You can do clean-up work that doesn't throw exceptions here.
        boost::filesystem::remove_all(dir);
    }
};

BOOST_FIXTURE_TEST_SUITE(SQLFeatureStorageTest,SQLFeatureStorageTestHelp)


BOOST_AUTO_TEST_CASE( testFeatureTable2 ){
    std::cout << "testFeatureTable2" << std::endl;
    ralab::findmf::datastruct::Feature2D f2D;
    std::vector<float> rtproj,mzproj;

    mzproj.push_back(0.);
    mzproj.push_back(0.);
    mzproj.push_back(1.);
    mzproj.push_back(1.);
    //test
    rtproj.push_back(1.);
    rtproj.push_back(2.);
    rtproj.push_back(3.);
    rtproj.push_back(4.);

    ralab::findmf::datastruct::MapLCMSDescriptionPtr x =
            ralab::findmf::datastruct::MapLCMSDescriptionPtr(
                new ralab::findmf::datastruct::MapLCMSDescription()
                );
    ralab::findmf::datastruct::FeaturesMap fm(x);

    for(int i = 0 ; i < 1000; ++i)
    {
        // std::cout << i << std::endl;
        f2D.setID(i);
        //float n = static_cast<float>(i);
        f2D.setCount(i);
        f2D.setMaximum(i*2);
        mzproj[3] = i;
        rtproj[3] = i*2;
        f2D.setProjectionMZ(mzproj);
        f2D.setProjectionRT(rtproj);
        fm.features().push_back(f2D);
    }

    std::string dbname("./testdata/test2.sqlite");
    ralab::findmf::FeaturesMapSQLWriter fsqlw(dbname);
}

// Feature table
BOOST_AUTO_TEST_CASE( testFeatureTable )
{
    std::cout << "testFeatureTable" << std::endl;
    ralab::SQLFeatureStorage sfs("./testdata/test.sqlite");
    ralab::SQLFeatureTable sfrt( sfs.getDatabase() );

    sfrt.createTable();
    //sfrt.testInsert();
    sfrt.prepareInsert();
    QTime myTimer;
    myTimer.start();
    // do something..
    sfs.transaction();
    ralab::findmf::datastruct::Feature2D f2D;

    std::vector<float> rtproj,mzproj;
    mzproj.push_back(0.);
    mzproj.push_back(0.);
    mzproj.push_back(1.);
    mzproj.push_back(1.);

    rtproj.push_back(1.);
    rtproj.push_back(2.);
    rtproj.push_back(3.);
    rtproj.push_back(4.);

    boost::timer t;

    for(int i = 0 ; i < 1000; ++i)
    {
        // std::cout << i << std::endl;
        f2D.setID(i);
        f2D.setSwathId(i % 1000);
        //float n = static_cast<float>(i);
        f2D.setCount(i);
        f2D.setMaximum(i*2);

        mzproj[3] = i;
        rtproj[3] = i*2;
        f2D.setProjectionMZ(mzproj);
        f2D.setProjectionRT(rtproj);
        ralab::FeatureReadAdapter x(f2D);
        sfrt.insertFeature(x,i,0);
    }
    sfs.commit();

    std::cout << "time elapsed : " << t.elapsed() << std::endl;

    ralab::FeatureDTO dto;
    ralab::FeatureWriteAdapter fwa(dto);
    t.restart();
    std::cout << "started to write into sqlite " << std::endl;

    ralab::SQLFeatureTableSelects sfrs( sfs.getDatabase() );

    for(uint32_t i = 0 ; i <1000; ++i){
        sfrs.getFeature(i,fwa);
        if(!(dto.id_ == i))
            std::cout << "id : " << dto.id_ << std::endl;

        if(!(dto.count_ == i))
            std::cout << "cout : " << dto.count_ << std::endl;

        if(!(dto.maximum_ == i*2))
            std::cout << "maximum : " << dto.maximum_ << std::endl;

        if(!(dto.mzproj_[3]== i))
            std::cout << "mzproj last" << dto.mzproj_[3] << std::endl;

        if(!(dto.rtproj_[3]== i*2))
            std::cout << "mzproj last" << dto.rtproj_[3] << std::endl;
    }
    std::cout << "time elapsed : " << t.elapsed() << std::endl;
}

/* TODO fix test Intertions fail */
BOOST_AUTO_TEST_CASE( testRDTree ){
    std::cout << "testRDTree" << std::endl;
    ralab::SQLFeatureStorage sfs("./testdata/testrdtree.sqlite");
    ralab::SQLFeatureRTree sfrt( sfs.getDatabase() );
    sfrt.createTable();
    // sfrt.testInsert();
    sfrt.prepareInsert();

    QTime myTimer;
    myTimer.start();
    // do something..
    sfs.transaction();
    ralab::RangeFeature ff;

    for(int i = 0 ; i < 100; ++i){
        std::cout << i << std::endl;
        ff.setID(i);
        float n = static_cast<float>(i);
        ff.setMinMZ(n/100.+10);
        ff.setMaxMZ(n/100.+11);
        ff.setMinRT(n/10.+20);
        ff.setMaxRT(n/10.+21);
        sfrt.insertFeature(ff,i);
    }
    sfs.commit();
    int nMilliseconds = myTimer.elapsed();
    std::cout << "milli seconds: " << nMilliseconds  << std::endl;

    ralab::SQLFeatureRTreeSelect sfrteesel( sfs.getDatabase() );

    std::vector<uint32_t> tmp;
    sfrteesel.getIDs(20.5,25.,tmp);
    std::cout << tmp.size() << std::endl;
    tmp.clear();
    sfrteesel.getIDs(20.5,tmp);
    std::cout << tmp.size() << std::endl;

    tmp.clear();

    sfrteesel.getIDs(10.3,10.3,22.5,25.,tmp);
    std::cout << tmp.size() << std::endl;

    std::vector<float> rts;
    sfrteesel.getMinRTS(rts);
    sfs.closeDB();
}


BOOST_AUTO_TEST_SUITE_END()
