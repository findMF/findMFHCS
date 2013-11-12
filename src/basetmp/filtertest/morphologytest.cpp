// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2013.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Clemens Groepl, Witold Wolski $
// $Authors: $
// --------------------------------------------------------------------------

#include "base/filter/morphology.h"
#include <vector>

#include "gtest/gtest.h"
///////////////////////////

template < typename ValueT >
struct SimpleTopHat
{
  static void erosion( const std::vector<ValueT> & input, std::vector<ValueT> & output, const uint32_t struc_elem_length )
  {
    const int32_t size = int32_t(input.size());
    const int32_t struc_elem_half = struc_elem_length / 2; // yes integer division
    output.clear();
    output.resize(size);
    for ( int32_t index = 0; index < size; ++ index )
      {
        int32_t begin = std::max( 0,    index - struc_elem_half );
        int32_t end   = std::min( size - 1, index + struc_elem_half );
        ValueT value = std::numeric_limits<ValueT>::max();
        for ( int32_t i = begin; i <= end; ++i )
          {
            if ( value > input[i] ) value = input[i];
          }
        output[index] = value;
      }
    return;
  }

  static void dilation( const std::vector<ValueT> & input, std::vector<ValueT> & output, const uint32_t struc_elem_length )
  {
    const int32_t size = int32_t(input.size());
    const int32_t struc_elem_half = struc_elem_length / 2; // yes integer division
    output.clear();
    output.resize(size);
    for ( int32_t index = 0; index < size; ++ index )
      {
        int32_t begin = std::max( 0,    index - struc_elem_half );
        int32_t end   = std::min( size - 1, index + struc_elem_half );
        ValueT value = - std::numeric_limits<ValueT>::max();
        for ( int32_t i = begin; i <= end; ++i )
          {
            if ( value < input[i] ) value = input[i];
          }
        output[index] = value;
      }
    return;
  }

  static void gradient( const std::vector<ValueT> & input, std::vector<ValueT> & output, const uint32_t struc_elem_length )
  {
    const int32_t size = int32_t(input.size());
    output.clear();
    output.resize(size);
    std::vector<ValueT> dilation_;
    std::vector<ValueT> erosion_;
    erosion(input,erosion_,struc_elem_length);
    dilation(input,dilation_,struc_elem_length);
    for ( int32_t index = 0; index < size; ++ index )
      {
        output[index] = dilation_[index] - erosion_[index];
      }
    return;
  }

  static void tophat( const std::vector<ValueT> & input, std::vector<ValueT> & output, const uint32_t struc_elem_length )
  {
    const int32_t size = int32_t(input.size());
    std::vector<ValueT> opening;
    erosion(input,output,struc_elem_length);
    dilation(output,opening,struc_elem_length);
    for ( int32_t index = 0; index < size; ++ index )
      {
        output[index] = input[index] - opening[index];
      }
    return;
  }

  static void bothat( const std::vector<ValueT> & input, std::vector<ValueT> & output, const uint32_t struc_elem_length )
  {
    const int32_t size = int32_t(input.size());
    std::vector<ValueT> closing;
    dilation(input,output,struc_elem_length);
    erosion(output,closing,struc_elem_length);
    for ( int32_t index = 0; index < size; ++ index )
      {
        output[index] = input[index] - closing[index];
      }
    return;
  }
};


///////////////////////////

namespace{

  class MorphologyTest : public ::testing::Test{
  protected:
    std::vector<int32_t> data_;
    std::vector<int32_t> input;
    uint32_t struc_elem_length;
    typedef SimpleTopHat<int32_t> STH;
    std::vector<int32_t> erosion;
    std::vector<int32_t> dilation;
    std::vector<int32_t> gradient;
    std::vector<int32_t> opening;
    std::vector<int32_t> closing;
    std::vector<int32_t> tophat;
    std::vector<int32_t> bothat;
    std::size_t data_size;


  public:
    MorphologyTest():struc_elem_length(3){
      int32_t data[] = { 1, 2, 3, -2, 0, 1, 0, 0, 0, 1, 1, 1, 4, 5, 6, 4, 3, 2, 2, 5, 5, 6, 6, 1, 0, 0, -1, 0, 0, 3, -2, -3, -1, 1, 1, 1, 1, 4, 6, 2 };
      data_size = sizeof(data)/sizeof(*data);
      input.reserve(data_size);

      init(data_size, data);
    }

    void init(uint32_t data_size, int32_t * data){
      for ( uint32_t i = 0; i != data_size; ++i ) input.push_back(data[i]);
      STH::erosion(input,erosion,struc_elem_length);
      STH::dilation(input,dilation,struc_elem_length);
      STH::gradient(input,gradient,struc_elem_length);
      STH::dilation(erosion,opening,struc_elem_length);
      STH::erosion(dilation,closing,struc_elem_length);
      STH::tophat(input,tophat,struc_elem_length);
      STH::bothat(input,bothat,struc_elem_length);
    }
  };

  using namespace OpenMS;
  TEST_F(MorphologyTest,constructor)
  {
    MorphologicalFilter<int32_t>* tophat_nullPointer = 0;
    MorphologicalFilter<int32_t>* tophat_ptr = new MorphologicalFilter<int32_t>;
    ASSERT_TRUE(tophat_ptr != tophat_nullPointer);
  }


  TEST_F(MorphologyTest,erosion)
  {
    // This test uses increasing and decreasing sequences of numbers.  This way
    // we are likely to catch all off-by-one errors. ;-) An [EXTRA] test
    // follows, which uses more realisic data.
    //typedef SimpleTopHat<int32_t> STHF;

    for ( int32_t data_size = 0; data_size < 50; ++ data_size )
      {
        int32_t offset = data_size / 2;
        std::vector<int32_t> raw;
        std::vector<int32_t> inputf;
        raw.clear();
        inputf.clear();
        inputf.clear();
        for ( int32_t i = 0; i != data_size; ++i )
          {
            raw.push_back(i-offset);
            inputf.push_back(i-offset);
          }

        std::vector<int32_t> filtered;
        std::vector<int32_t> simple_filtered_1;
        MorphologicalFilter<int32_t> mf;

        for ( int32_t struc_length = 3; struc_length <= 2 * data_size + 2; struc_length += 2 )
          {


            filtered.clear();
            filtered.resize(data_size);
            simple_filtered_1.clear();
            simple_filtered_1.resize(data_size);


            mf.erosion(  raw.begin(),
                         raw.end(),
                         filtered.begin(),
                         struc_length
                         );
            STH::erosion( inputf,
                          simple_filtered_1,
                          struc_length
                          );
            for ( int32_t i = 0; i != data_size; ++i )
              {
                ASSERT_EQ(filtered[i],simple_filtered_1[i]);
              }
          }


        filtered.clear();
        filtered.resize(data_size);
        simple_filtered_1.clear();
        simple_filtered_1.resize(data_size);

        uint32_t struc_length = 3;

        mf.dilation(  raw.begin(),
                      raw.end(),
                      filtered.begin(),
                      struc_length
                      );
        STH::dilation( inputf,
                       simple_filtered_1,
                       struc_length
                       );
        for ( int32_t i = 0; i != data_size; ++i )
          {
            ASSERT_EQ(filtered[i],simple_filtered_1[i]);
          }
      }
  }

  TEST_F(MorphologyTest,filterRange)
  {
    typedef SimpleTopHat<double> STHF;
    for ( int32_t data_size = 0; data_size < 50; ++ data_size )
      {

        int32_t offset = data_size / 2;
        std::vector<double> raw;
        std::vector<double> inputf;

        raw.clear();
        inputf.clear();

        for ( int32_t i = 0; i != data_size; ++i )
          {
            raw.push_back(i-offset);
            inputf.push_back(i-offset);
          }

        std::vector<double> filtered;
        std::vector<double> simple_filtered_1;


        MorphologicalFilter<double> mf;

        for ( int32_t struc_length = 3; struc_length <= 2 * data_size + 2; struc_length += 2 )
          {

            filtered.clear();
            filtered.resize(data_size);
            simple_filtered_1.clear();
            simple_filtered_1.resize(data_size);


            mf.erosion(  raw.begin(),
                         raw.end(),
                         filtered.begin(),
                         struc_length
                         );
            STHF::erosion( inputf,
                           simple_filtered_1,
                           struc_length
                           );
            for ( int32_t i = 0; i != data_size; ++i )
              {
                ASSERT_NEAR(filtered[i],simple_filtered_1[i], 1e-10);
              }
          }


        filtered.clear();
        filtered.resize(data_size);
        simple_filtered_1.clear();
        simple_filtered_1.resize(data_size);


        uint32_t struc_length = 3;
        mf.dilation(  raw.begin(),
                      raw.end(),
                      filtered.begin(),struc_length  );
        STHF::dilation( inputf,
                        simple_filtered_1,
                        struc_length
                        );
        for ( int32_t i = 0; i != data_size; ++i )
          {

            ASSERT_NEAR(filtered[i],simple_filtered_1[i],1e-10);
          }
      }
  }

  TEST_F(MorphologyTest,filterRange2)
  {
    std::vector<double> inputf;
    std::vector<double> raw;
    inputf.assign(input.begin(),input.end());
    raw.assign(inputf.begin(),inputf.end());
    std::vector<double> filtered;
    std::vector<double> simple_filtered_1;
    std::vector<double> simple_filtered_2;
    std::vector<double> simple_filtered_3;
    MorphologicalFilter<double> mf;
    typedef SimpleTopHat<double> STHF;
    for ( uint32_t struc_length = 3; struc_length <= 2 * data_size + 2; struc_length += 2 )
      {


        filtered.clear();
        filtered.resize(data_size);
        simple_filtered_1.clear();
        simple_filtered_1.resize(data_size);

        mf.erosion( raw.begin(),
                    raw.end(),
                    filtered.begin(),
                    struc_length
                    );
        STHF::erosion(inputf,simple_filtered_1,struc_length);
        for ( uint32_t i = 0; i != data_size; ++i )
          {
            ASSERT_NEAR(filtered[i],simple_filtered_1[i],1e-10);
          }


        filtered.clear();
        filtered.resize(data_size);
        //simple_filtered_1.clear();
        //simple_filtered_1.resize(data_size);


        //opening
        filtered.clear();
        filtered.resize(data_size);
        simple_filtered_2.clear();
        simple_filtered_2.resize(data_size);


        mf.opening(  raw.begin(),
                     raw.end(),
                     filtered.begin(),
                     struc_length
                     );
        STHF::dilation(simple_filtered_1,simple_filtered_2,struc_length);
        for ( uint32_t i = 0; i != data_size; ++i )
          {
            ASSERT_NEAR(filtered[i],simple_filtered_2[i],1e-10);
          }

        //"tophat";
        filtered.clear();
        filtered.resize(data_size);
        simple_filtered_3.clear();
        simple_filtered_3.resize(data_size);

        mf.tophat(  raw.begin(),
                    raw.end(),
                    filtered.begin(),
                    struc_length
                    );
        STHF::tophat(inputf,simple_filtered_3,struc_length);
        for ( uint32_t i = 0; i != data_size; ++i )
          {
            ASSERT_NEAR(filtered[i],simple_filtered_3[i],1e-10);
          }


        //"dilation"
        filtered.clear();
        filtered.resize(data_size);
        simple_filtered_1.clear();
        simple_filtered_1.resize(data_size);



        mf.dilation(  raw.begin(),
                      raw.end(),
                      filtered.begin(),
                      struc_length
                      );
        STHF::dilation(inputf,simple_filtered_1,struc_length);
        for ( uint32_t i = 0; i != data_size; ++i )
          {
            ASSERT_NEAR(filtered[i],simple_filtered_1[i], 1e-10);
          }


        //"closing";
        filtered.clear();
        filtered.resize(data_size);
        simple_filtered_2.clear();
        simple_filtered_2.resize(data_size);


        mf.closing( raw.begin(),
                    raw.end(),
                    filtered.begin(),
                    struc_length
                    );
        STHF::erosion(simple_filtered_1,simple_filtered_2,struc_length);
        for ( uint32_t i = 0; i != data_size; ++i )
          {
            ASSERT_NEAR(filtered[i],simple_filtered_2[i],1e-10);
          }

        //"bothat"
        filtered.clear();
        filtered.resize(data_size);
        simple_filtered_3.clear();
        simple_filtered_3.resize(data_size);



        mf.bothat( raw.begin(),
                   raw.end(),
                   filtered.begin(),
                   struc_length
                   );
        STHF::bothat(inputf,simple_filtered_3,struc_length);
        for ( uint32_t i = 0; i != data_size; ++i )
          {
            ASSERT_NEAR(filtered[i],simple_filtered_3[i],1e-10);
          }

      }
  }

}//end namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


