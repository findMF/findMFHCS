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
#include "tbb/concurrent_queue.h"

//#include "tbbutils/utility.h"
//#include "../../common/utility/utility.h"

using namespace std;

//! Holds a slice of text.
/** Instances *must* be allocated/freed using methods herein, because the C++ declaration
    represents only the header of a much larger object in memory. */
class MyMatrix {
public:
  std::vector<std::vector<double> > tx;
  int count_;

  MyMatrix(){}
  MyMatrix(int i,int j, int count):count_(count){
    tx.resize(i);
    for(int x = 0; x < i; ++x){
        tx[x]=std::vector<double>(j,x);
      }
  }


};


class MyInputFilter: public tbb::filter {
  int i_,j_;
  int count_;
  MyMatrix map_;

public:
  MyInputFilter( int i, int j  ):tbb::filter(serial_in_order),count_(0) {
    i_=i;j_=j;
  }

  ~MyInputFilter(){
    std::cout << "~MyInputFilter" <<std::endl;
  }

  void* operator()(void*){
    count_++;
    if(count_ < 100){
        map_ = MyMatrix(i_, j_,count_);

        return  &map_;
      }
    else{
        return NULL;
      }
  }
};

struct VectID{
 typedef std::vector<double> Vectr;
  Vectr data_;
  int count_;
  VectID(uint32_t l, int count):data_(l),count_(count){}
};

//! Filter that changes each decimal number to its square.
class MyTransformFilter: public tbb::filter {
public:
  typedef std::vector<double> Vectr;

  MyTransformFilter():
    tbb::filter(parallel)
  {}

  void* operator()( void* item ){
    MyMatrix * x = static_cast< MyMatrix * >(item);
    VectID * rowsum_ =  new VectID(x->tx.size(),x->count_);

    for(std::size_t i = 0; i < x->tx.size();++i){
        for(std::size_t j = 0; j < x->tx[0].size(); ++j){
            double val= x->tx[i][j];
            (*rowsum_).data_[i] += val;
          }
      }
    return rowsum_;
  }
  ~MyTransformFilter(){
    std::cout << "~MyTransformFilter" << std::endl;
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

    VectID * v =  static_cast<VectID *>(item);
    std::cout << v->count_ << "\t";
    for (std::size_t i=0; i< v->data_.size(); ++i){
        std::cout << (*v).data_[i] << "\t";
        std::cout << std::flush;
      }
    delete v;
    std::cout << std::endl;
    return NULL;
  }
  ~MyOutputFilter(){
    std::cout << "~MyOutputFilter" << std::endl;
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

  MyMatrix x = MyMatrix(10,10,0);
  try {
    tbb::tick_count mainStartTime = tbb::tick_count::now();
    // The 1st argument is the function to obtain 'auto' value; the 2nd is the default value
    // The example interprets 0 threads as "run serially, then fully subscribed"
    tbb::task_scheduler_init init_parallel;
    //if(!run_pipeline (init_parallel.default_num_threads()))
    //  return 1;
    if(!run_pipeline (6))
      return 1;
    std::cout << (tbb::tick_count::now() - mainStartTime).seconds() << std::endl;
    return 0;
  } catch(std::exception& e) {
    std::cerr<<"error occurred. error text is :\"" <<e.what()<<"\"\n";
    
  }
}
#endif // PIPELINETEST_CPP

