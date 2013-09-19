// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef PRELIMINARYTESTS_H
#define PRELIMINARYTESTS_H

namespace ralab{
  class PreliminaryTests
  {
  public:
    PreliminaryTests();
    static void testImaging();
    static void testMap();
    static void merge();
    static void testSpectrast();
    static void testSwathTable();
  };
}

#endif // PRELIMINARYTESTS_H
