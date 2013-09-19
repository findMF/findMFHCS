// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//#include <boost/test/included/unit_test.hpp>

#include "findmf/application/featuresmapsqlwriterfacade.h"

#include <iostream>
#include <stdexcept>


inline void testMapInfoTable(){

  ralab::FeaturesMapSQLWriterFacade sql("test","bla");
  sql.createDatabase();
}



int main(int arg, char ** argv)
{

  testMapInfoTable();
  return 0;
}
