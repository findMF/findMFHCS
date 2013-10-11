// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//#include <boost/test/included/unit_test.hpp>
#include <boost/timer.hpp>
//#include <boost/test/unit_test.hpp>
//using namespace boost::unit_test;

#include <iostream>
#include "findmf/datastruct/maplcmsdescription.h"
#include "findmf/fileio/sqlite2/sqlfeaturestorage.h"
#include "findmf/fileio/sqlite2/sqlmapinfotable.h"



inline void testMapInfoTable(){
  std::cout << "testFeatureTable2" << std::endl;

  std::vector<double> rtproj,mzproj;

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
      ralab::findmf::datastruct::MapLCMSDescriptionPtr(new ralab::findmf::datastruct::MapLCMSDescription());

  x->mslevel_ = 1;
  x->extractionWindowMZ_.first = 100.;
  x->extractionWindowMZ_.second  = 125.;
  x->mass_ = mzproj;
  x->retentiontime_ = rtproj;
  x->rtRange_.first = 1000;
  x->rtRange_.second = 1400;

  x->mzRange_.first = 120.03;
  x->mzRange_.second = 1444.03;

  std::string dbname("../data/fmap.sqlite");
  ralab::SQLFeatureStorage sfs(dbname);
  ralab::findmf::SQLMapInfoTable fsqlw(sfs.getDatabase() );
  fsqlw.createTable();
  fsqlw.executeTestQuery();
  fsqlw.prepareInsert();
  fsqlw.insert(x);
}


int main(int arg, char ** argv)
{
  testMapInfoTable();
  return 0;
}
