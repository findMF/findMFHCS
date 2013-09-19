// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef PIPELINETEST_CPP
#define PIPELINETEST_CPP
/*
    Copyright 2005-2013 Intel Corporation.  All Rights Reserved.

    This file is part of Threading Building Blocks.

    Threading Building Blocks is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    version 2 as published by the Free Software Foundation.

    Threading Building Blocks is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Threading Building Blocks; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    As a special exception, you may use this file as part of a free software
    library without restriction.  Specifically, if other files instantiate
    templates or use macros or inline functions from this file, or you compile
    this file and link it with other files to produce an executable, this
    file does not by itself cause the resulting executable to be covered by
    the GNU General Public License.  This exception does not however
    invalidate any other reasons why the executable file might be covered by
    the GNU General Public License.
*/

//
// Example program that reads a file of decimal integers in text format
// and changes each to its square.
//
#include "tbb/pipeline.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/tbb_allocator.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <tbb/concurrent_queue.h>
//#include "tbbutils/utility.h"
//#include "../../common/utility/utility.h"

using namespace std;

//! Holds a slice of text.
/** Instances *must* be allocated/freed using methods herein, because the C++ declaration
    represents only the header of a much larger object in memory. */
class MyMatrix {
public:
  std::vector<std::vector<double> > tx;

  MyMatrix():tx(){}
  MyMatrix(int i,int j){
    tx.resize(i);
    for(int x = 0; x < i; ++x){
        tx[x]=std::vector<double>(j,x);
      }
  }
};

typedef  tbb::concurrent_bounded_queue<MyMatrix> MatrixQueue;

class MyInputFilter: public tbb::filter {
  int i_,j_;
  int count_; // number of inputs to generate.
  MatrixQueue mqueue_;
public:
  MyInputFilter( int i, int j  ):tbb::filter(serial_in_order),count_(0) {
    i_=i;j_=j;
    mqueue_.set_capacity(3);
  }
  void* operator()(void*){
    count_++;
    if(count_ < 10){
        MyMatrix bla(i_,j_);
        mqueue_.push(bla);
        return  &mqueue_;
      }
    else{
        return NULL;
      }
  }

};

typedef  tbb::concurrent_bounded_queue<std::vector<double> > DoubleQueue;

//! Filter that changes each decimal number to its square.
class MyTransformFilter: public tbb::filter {
  std::vector<double> rowsum_;
  DoubleQueue queue_;
public:
  MyTransformFilter():
    tbb::filter(parallel)
  {
    queue_.set_capacity(1);
  }

  void* operator()( void* item ){
    MatrixQueue  mqueue = * static_cast< MatrixQueue * >(item);
    MyMatrix mat;
    mqueue.pop(mat);

    rowsum_.resize( mat.tx.size() );
    for(std::size_t i = 0; i < mat.tx.size();++i){
        for(std::size_t j = 0; j < mat.tx[0].size(); ++j){
            double val= mat.tx[i][j];
            rowsum_[i] += val;
          }
      }
    queue_.push(rowsum_);
    return &queue_;
  }

};


//! Filter that writes each buffer to a file.
class MyOutputFilter: public tbb::filter {
public:
  MyOutputFilter(  ):
    tbb::filter(serial_in_order)
  {
  }

  void* operator()( void* item ){

    DoubleQueue qq = *static_cast<DoubleQueue*>(item);
    std::vector<double> rowsum;
    qq.pop(rowsum);

    for (std::size_t i=0; i< rowsum.size(); ++i){
        std::cout << rowsum[i] << "\t";
        std::cout << std::flush;
      }
    std::cout << std::endl;
    return NULL;
  }
};

bool silent = false;

int run_pipeline( int nthreads )
{


  // Create the pipeline
  tbb::pipeline pipeline;

  // Create file-reading writing stage and add it to the pipeline
  MyInputFilter input_filter( 5,5 );
  pipeline.add_filter( input_filter );

  // Create squaring stage and add it to the pipeline
  MyTransformFilter transform_filter;
  pipeline.add_filter( transform_filter );

  // Create file-writing stage and add it to the pipeline
  MyOutputFilter output_filter;
  pipeline.add_filter( output_filter );

  // Run the pipeline
  tbb::tick_count t0 = tbb::tick_count::now();
  // Need more than one token in flight per thread to keep all threads
  // busy; 2-4 works
  pipeline.run( nthreads*4 );
  tbb::tick_count t1 = tbb::tick_count::now();

  if ( !silent ) printf("time = %g\n", (t1-t0).seconds());

  return 1;
}

int main( int argc, char* argv[] ) {


  try {
    tbb::tick_count mainStartTime = tbb::tick_count::now();

    // The 1st argument is the function to obtain 'auto' value; the 2nd is the default value
    // The example interprets 0 threads as "run serially, then fully subscribed"
    tbb::task_scheduler_init init_parallel;
    //if(!run_pipeline (init_parallel.default_num_threads()))
    //  return 1;
    if(!run_pipeline (5))
      return 1;

    std::cout << (tbb::tick_count::now() - mainStartTime).seconds() << std::endl;
    return 0;
  } catch(std::exception& e) {
    std::cerr<<"error occurred. error text is :\"" <<e.what()<<"\"\n";
    return 1;
  }
}
#endif // PIPELINETEST_CPP
