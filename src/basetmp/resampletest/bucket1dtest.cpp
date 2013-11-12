// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "base/resample/bucket1d.h"
#include "base/resample/bin1d.h"

#include "gtest/gtest.h"

#include <algorithm>
#include <boost/timer.hpp>
#include "base/stats/uniform.h"
//#include "base/resample/histprototype.h"
#include "base/resample/breakspec.h"
#include "base/resample/masscomparefunctors.h"
//#include "base/resample/IndicatorHelper.h"

namespace {

  class Bucket1DTest: public ::testing::Test{
  protected:
    Bucket1DTest(){}
  };

  TEST_F(Bucket1DTest,testBin1D){
    double ref [] = {2., 2.1 , 5. , 5.1 , 7.1 , 7.15 , 10. , 10.1};
    std::vector<double> breaks;
    breaks.assign(ref , ref + sizeof(ref)/sizeof(double));

    ralab::base::resample::Bin1D bin(breaks);
    ASSERT_EQ(bin(2.-1e-8) , 0);
    ASSERT_EQ(bin(2.+1e-8) , 1);
    ASSERT_EQ(bin(2.1) ,1);
    ASSERT_EQ(bin(2.1 + 1e-9) , 2);
  }

  TEST_F(Bucket1DTest,testBin1D2){
    double ref [] = {2., 2.1 , 5. , 5.1 , 7.1 , 7.15 , 10. , 10.1};
    std::vector<double> breaks;
    breaks.assign(ref , ref + sizeof(ref)/sizeof(double));

    ralab::base::resample::Bin1D bin(breaks);
    std::vector<int32_t> idx;
    std::vector<double> dist;
    bin(2. -1e-8 , 2. + 2e-4,  idx, dist ) ;
    ASSERT_EQ(idx[0],-1);
    ASSERT_EQ(idx[1],0);
    ASSERT_FLOAT_EQ(dist[0],1e-8);
    ASSERT_FLOAT_EQ(dist[1],2e-4);

    bin(2. -1e-8 , 2.1 + 2e-4,  idx, dist ) ;
    ASSERT_EQ(idx[0],-1);
    ASSERT_EQ(idx[1],0);
    ASSERT_EQ(idx[2],1);
    ASSERT_FLOAT_EQ(dist[0], 1e-8 );
    ASSERT_FLOAT_EQ(dist[1], 0.1 );
    ASSERT_FLOAT_EQ(dist[2], 2e-4 );

    bin(2. -1e-8 , 5.1 + 2e-4,  idx, dist ) ;
    double resd [] = {1e-8, 0.1, 2.9, 0.1, 2e-4};
    int  idxr[] = {-1,0,1,2,3};
    bool x = std::equal(idxr , idxr + sizeof(idxr)/sizeof(int), idx.begin());
    ASSERT_TRUE(x);
    x = std::equal(resd , resd + sizeof(resd)/sizeof(double), dist.begin(), ralab::base::resample::DaCompFunctor<double>(1e-8));
    ASSERT_TRUE(x);

    bin(2. - 2e-4 , 2. - 1e-4,  idx, dist ) ;
    ASSERT_EQ(idx[0],-1);
    ASSERT_NEAR(dist[0],1e-4,1e-14);
    bin(2.1 - 2e-4 , 2.1 - 1e-4,  idx, dist ) ;
    ASSERT_EQ(idx[0],0);
    ASSERT_NEAR(dist[0],1e-4,1e-14);
    bin(2.1 - 1e-4 , 7.1 + 2e-4,  idx, dist ) ;


    //testing end span
    bin(10.1 - 1e-4 , 10.1 + 2e-4,  idx, dist ) ;
    std::cout << breaks[idx[0]] << std::endl;
    std::cout << breaks[idx[1]] << std::endl;

    bin(5.1 - 1e-4 , 10.1 + 2e-4,  idx, dist ) ;
    std::cout << idx[0] << std::endl;

    bin(10.1 + 1e-4 , 10.1 + 2e-4,  idx, dist ) ;
    std::cout << idx[0] << std::endl;


  }


  TEST_F(Bucket1DTest,testHist)
  {
    std::vector<double> breaks;
    std::vector<uint32_t> indicator;
    //                 0     1     2    3     4      5     6
    double ref [] = {2., 2.1 , 5. , 5.1 , 7.1 , 7.15 , 10. , 10.1};
    // We cover 0.1+0.1+0.05+0.1 = 0.35 //
    breaks.assign(ref , ref + sizeof(ref)/sizeof(double));

    /*!\brief length indic is length(ref) - 1
                                */
    //                   0   1   2   3   4   5   6
    uint32_t indic[] = { 1 , 0 , 1 , 0 , 1 , 0 , 1};

    indicator.assign(
          indic
          ,indic + sizeof(indic)/sizeof(uint32_t)
          );

    ralab::base::resample::Bucket1D b1d( breaks, indicator);
    std::vector<double> sample;
    std::pair< size_t , bool > rb;
    rb = b1d.operator()( 1. );
    rb = b1d.operator()( 2. );
    rb = b1d.operator()( 2.05 );
    rb = b1d.operator()( 2.1 );
    rb = b1d.operator()( 4. );
    rb = b1d.operator()( 5. );
    rb = b1d.operator()( 5.01 );
    rb = b1d.operator()( 5.1 );
    rb = b1d.operator()(6.);
    rb = b1d.operator()(7.);
    rb = b1d.operator()(7.12); // 4,true
    rb = b1d.operator()(8.);
    rb = b1d.operator()( 10.1 );
    rb = b1d.operator()( 13. );

    sample.push_back( 1. );//false
    sample.push_back( 2. );//false
    sample.push_back( 2.05 );//#2 -> 0
    sample.push_back( 2.1 );//#3 -> 0
    sample.push_back( 4. );//false
    sample.push_back( 5. );//false
    sample.push_back( 5.01 );//#6 -> 2
    sample.push_back( 5.1 );//#7 -> 2
    sample.push_back( 10.1 );//#8 -> 6
    sample.push_back( 13. );//false

    std::vector< std::pair< size_t , size_t > > res;
    b1d( sample.begin() , sample.end() , res );
    ASSERT_TRUE(res[0].first == 0 && res[0].second == 2);
    ASSERT_TRUE(res[1].first == 0 && res[1].second == 3);
  }


}//end namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}




