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

#ifndef MorphologicalFilter_H
#define MorphologicalFilter_H


#include <algorithm>
#include <iterator>
#include <vector>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/same_traits.hpp>

namespace OpenMS
{
  /**
      @brief This class implements baseline filtering operations using methods
      from mathematical MorphologicalFilter.

      The fundamental operations are erosion and dilation.  These are defined with
      respect to a structuring element.  In our case, this is just a straight line
      and the definitions can be given as follows:

      Assume that the input is \f$x_0, x_1, x_2, ...\f$.  Then the <i>erosion</i>
      of \f$x\f$ contains the minima of a sliding window of size struc_size around
      \f$ i \f$, i.e. \f[ \mathrm{erosion}_i = \min\{x_{i-\mathrm{struc\_size}/2},
      \ldots, x_{i+\mathrm{struc\_size}/2}\} \f].  The <i>dilation</i> of \f$x\f$
      contains the maxima of a sliding window of size struc_size around \f$ i \f$,
      i.e. \f[ \mathrm{dilation}_i = \max\{x_{i-\mathrm{struc\_size}/2}, \ldots,
      x_{i+\mathrm{struc\_size}/2}\} \f].

      For morphological baseline filtering the <i>tophat</i> method is used.  The
      tophat transform is defined as signal minus opening, where the opening is
      the dilation of the erosion of the signal.

      Several other morphological operations are implemented as well.  See the
      image below and the documentation for further explanation.

      @note The class #MorphologicalFilter is designed for uniformly spaced profile data.

      @note The data must be sorted according to ascending m/z!

      @ingroup SignalProcessing
  */
  template<typename Tvalue>
  class MorphologicalFilter
  {
    uint32_t struct_size_in_datapoints_;
    std::size_t buffersize_;
    typedef  Tvalue value_type;
    std::vector<value_type> buffer_;
    std::vector<value_type> erosionbuffer_;
    std::vector<value_type> dilationbuffer_;

  public:

    MorphologicalFilter() :struct_size_in_datapoints_(0),buffersize_(0),buffer_(),erosionbuffer_(),dilationbuffer_()
    {}


    /** @brief Applies the morphological filtering operation to an iterator range.

    Input and output range must be valid, i.e. allocated before.
    InputIterator must be a random access iterator type.

    @param input_begin the begin of the input range
    @param input_end  the end of the input range
    @param output_begin the begin of the output range

    @exception Exception::IllegalArgument The given method is not one of the values defined in the @em method paramter.
    */
    template <typename InputIterator, typename OutputIterator>
    typename boost::enable_if<boost::is_same<Tvalue , typename std::iterator_traits<InputIterator>::value_type> >::type
    erosion(InputIterator input_begin, InputIterator input_end, OutputIterator output_begin,
            uint32_t struct_size_in_datapoints_ )
    {
      applyErosion_(struct_size_in_datapoints_, input_begin, input_end, output_begin);
    }

    template <typename InputIterator, typename OutputIterator>
    typename boost::enable_if<boost::is_same<Tvalue , typename std::iterator_traits<InputIterator>::value_type> >::type
    dilation(InputIterator input_begin, InputIterator input_end, OutputIterator output_begin,
             uint32_t struct_size_in_datapoints_){

      applyDilation_(struct_size_in_datapoints_, input_begin, input_end, output_begin);
    }

    template <typename InputIterator, typename OutputIterator>
    typename boost::enable_if<boost::is_same<Tvalue , typename std::iterator_traits<InputIterator>::value_type> >::type
    opening(InputIterator input_begin, InputIterator input_end, OutputIterator output_begin,
            uint32_t struct_size_in_datapoints_)
    {
      // the buffer_ is static only to avoid reallocation


      buffersize_= std::distance(input_begin,input_end);
      buffer_.resize(buffersize_);
      applyErosion_(struct_size_in_datapoints_, input_begin, input_end, buffer_.begin());
      applyDilation_(struct_size_in_datapoints_, buffer_.begin(), buffer_.begin() + buffersize_, output_begin);
    }

    template <typename InputIterator, typename OutputIterator>
    typename boost::enable_if<boost::is_same<Tvalue , typename std::iterator_traits<InputIterator>::value_type> >::type
    closing(InputIterator input_begin, InputIterator input_end, OutputIterator output_begin,
            uint32_t struct_size_in_datapoints_)
    {

      buffersize_= std::distance(input_begin,input_end);
      buffer_.resize(buffersize_);
      applyDilation_(struct_size_in_datapoints_, input_begin, input_end, buffer_.begin());
      applyErosion_(struct_size_in_datapoints_, buffer_.begin(), buffer_.begin() + buffersize_, output_begin);
    }


    //TODO write test never ever tested...
    template <typename InputIterator, typename OutputIterator>
    typename boost::enable_if<boost::is_same<Tvalue , typename std::iterator_traits<InputIterator>::value_type> >::type
    gradient(InputIterator input_begin, InputIterator input_end, OutputIterator output_begin,
             uint32_t struct_size_in_datapoints_)
    {
      buffersize_= std::distance(input_begin,input_end);
      buffer_.resize(buffersize_);
      applyErosion_(struct_size_in_datapoints_, input_begin, input_end, buffer_.begin());
      applyDilation_(struct_size_in_datapoints_, input_begin, input_end, output_begin);
      for (uint32_t i = 0; i < buffersize_; ++i) output_begin[i] -= buffer_[i];
    }

    template <typename InputIterator, typename OutputIterator>
    typename boost::enable_if<boost::is_same<Tvalue , typename std::iterator_traits<InputIterator>::value_type> >::type
    tophat(InputIterator input_begin, InputIterator input_end, OutputIterator output_begin,
           uint32_t struct_size_in_datapoints_)
    {
      buffersize_= std::distance(input_begin,input_end);
      buffer_.resize(buffersize_);
      applyErosion_(struct_size_in_datapoints_, input_begin, input_end, buffer_.begin());
      applyDilation_(struct_size_in_datapoints_, buffer_.begin(), buffer_.begin() + buffersize_, output_begin);
      for (uint32_t i = 0; i < buffersize_; ++i) output_begin[i] = input_begin[i] - output_begin[i];
    }

    template <typename InputIterator, typename OutputIterator>
    void bothat(InputIterator input_begin, InputIterator input_end, OutputIterator output_begin,
                uint32_t struct_size_in_datapoints_)
    {
      buffersize_= std::distance(input_begin,input_end);
      buffer_.resize(buffersize_);
      applyDilation_(struct_size_in_datapoints_, input_begin, input_end, buffer_.begin());
      applyErosion_(struct_size_in_datapoints_, buffer_.begin(), buffer_.begin() + buffersize_, output_begin);
      for (uint32_t i = 0; i < buffersize_; ++i) output_begin[i] = input_begin[i] - output_begin[i];
    }



  protected:
    /** @brief Applies erosion.  This implementation uses van Herk's method.
    Only 3 min/max comparisons are required per data point, independent of
    struc_size.
    */
    template <typename InputIterator, typename OutputIterator>
    void applyErosion_(int32_t struc_size, InputIterator input, InputIterator input_end, OutputIterator output)
    {

      const int32_t size = input_end - input;
      const int32_t struc_size_half = struc_size / 2;           // yes, integer division

      erosionbuffer_.resize(struc_size);

      int32_t anchor;           // anchoring position of the current block
      int32_t i;                // index relative to anchor, used for 'for' loops
      int32_t ii = 0;           // input index
      int32_t oi = 0;           // output index
      value_type current;           // current value

      // we just can't get the case distinctions right in these cases, resorting to simple method.
      if (size <= struc_size || size <= 5)
        {
          applyErosionSimple_(struc_size, input, input_end, output);
          return;
        }
      {
        // lower margin area
        current = input[0];
        for (++ii; ii < struc_size_half; ++ii) if (current > input[ii]) current = input[ii];
        for (; ii < std::min(int32_t(struc_size), size); ++ii, ++oi)
          {
            if (current > input[ii]) current = input[ii];
            output[oi] = current;
          }
      }
      {
        // middle (main) area
        for (anchor = struc_size;
             anchor <= size - struc_size;
             anchor += struc_size
             )
          {
            ii = anchor;
            current = input[ii];
            erosionbuffer_[0] = current;
            for (i = 1; i < struc_size; ++i, ++ii)
              {
                if (current > input[ii]) current = input[ii];
                erosionbuffer_[i] = current;
              }
            ii = anchor - 1;
            oi = ii + struc_size_half;
            current = input[ii];
            for (i = 1; i < struc_size; ++i, --ii, --oi)
              {
                if (current > input[ii]) current = input[ii];
                output[oi] = std::min(erosionbuffer_[struc_size - i], current);
              }
            if (current > input[ii]) current = input[ii];
            output[oi] = current;
          }
      }
      {
        // higher margin area
        ii = size - 1;
        oi = ii;
        current = input[ii];
        for (--ii; ii >= size - struc_size_half; --ii) if (current > input[ii]) current = input[ii];
        for (; ii >= std::max(size - int32_t(struc_size), 0); --ii, --oi)
          {
            if (current > input[ii]) current = input[ii];
            output[oi] = current;
          }
        anchor = size - struc_size;
        ii = anchor;
        current = input[ii];
        erosionbuffer_[0] = current;
        for (i = 1; i < struc_size; ++i, ++ii)
          {
            if (current > input[ii]) current = input[ii];
            erosionbuffer_[i] = current;
          }
        ii = anchor - 1;
        oi = ii + struc_size_half;
        current = input[ii];
        for (i = 1; (ii >= 0) && (i < struc_size); ++i, --ii, --oi)
          {
            if (current > input[ii]) current = input[ii];
            output[oi] = std::min(erosionbuffer_[struc_size - i], current);
          }
        if (ii >= 0)
          {
            if (current > input[ii]) current = input[ii];
            output[oi] = current;
          }
      }
      return;
    }

    /** @brief Applies dilation.  This implementation uses van Herk's method.
    Only 3 min/max comparisons are required per data point, independent of
    struc_size.
    */
    template <typename InputIterator, typename OutputIterator>
    void applyDilation_(int32_t struc_size, InputIterator input, InputIterator input_end, OutputIterator output)
    {
      typedef typename InputIterator::value_type ValueType;
      const int32_t size = input_end - input;
      const int32_t struc_size_half = struc_size / 2;           // yes, integer division


      if (int32_t(dilationbuffer_.size()) < struc_size) dilationbuffer_.resize(struc_size);

      int32_t anchor;           // anchoring position of the current block
      int32_t i;                // index relative to anchor, used for 'for' loops
      int32_t ii = 0;           // input index
      int32_t oi = 0;           // output index
      ValueType current;           // current value

      // we just can't get the case distinctions right in these cases, resorting to simple method.
      if (size <= struc_size || size <= 5)
        {
          applyDilationSimple_(struc_size, input, input_end, output);
          return;
        }
      {
        // lower margin area
        current = input[0];
        for (++ii; ii < struc_size_half; ++ii) if (current < input[ii]) current = input[ii];
        for (; ii < std::min(int32_t(struc_size), size); ++ii, ++oi)
          {
            if (current < input[ii]) current = input[ii];
            output[oi] = current;
          }
      }
      {
        // middle (main) area
        for (anchor = struc_size;
             anchor <= size - struc_size;
             anchor += struc_size
             )
          {
            ii = anchor;
            current = input[ii];
            dilationbuffer_[0] = current;
            for (i = 1; i < struc_size; ++i, ++ii)
              {
                if (current < input[ii]) current = input[ii];
                dilationbuffer_[i] = current;
              }
            ii = anchor - 1;
            oi = ii + struc_size_half;
            current = input[ii];
            for (i = 1; i < struc_size; ++i, --ii, --oi)
              {
                if (current < input[ii]) current = input[ii];
                output[oi] = std::max(dilationbuffer_[struc_size - i], current);
              }
            if (current < input[ii]) current = input[ii];
            output[oi] = current;
          }
      }
      {
        // higher margin area
        ii = size - 1;
        oi = ii;
        current = input[ii];
        for (--ii; ii >= size - struc_size_half; --ii) if (current < input[ii]) current = input[ii];
        for (; ii >= std::max(size - int32_t(struc_size), 0); --ii, --oi)
          {
            if (current < input[ii]) current = input[ii];
            output[oi] = current;
          }
        anchor = size - struc_size;
        ii = anchor;
        current = input[ii];
        dilationbuffer_[0] = current;
        for (i = 1; i < struc_size; ++i, ++ii)
          {
            if (current < input[ii]) current = input[ii];
            dilationbuffer_[i] = current;
          }
        ii = anchor - 1;
        oi = ii + struc_size_half;
        current = input[ii];
        for (i = 1; (ii >= 0) && (i < struc_size); ++i, --ii, --oi)
          {
            if (current < input[ii]) current = input[ii];
            output[oi] = std::max(dilationbuffer_[struc_size - i], current);
          }
        if (ii >= 0)
          {
            if (current < input[ii]) current = input[ii];
            output[oi] = current;
          }
      }
      return;
    }

    /// Applies erosion.  Simple implementation, possibly faster if struc_size is very small, and used in some special cases.
    template <typename InputIterator, typename OutputIterator>
    void applyErosionSimple_(int32_t struc_size, InputIterator input_begin, InputIterator input_end, OutputIterator output_begin)
    {
      typedef typename InputIterator::value_type ValueType;
      const int32_t size = input_end - input_begin;
      const int32_t struc_size_half = struc_size / 2;           // yes integer division
      for (int32_t index = 0; index < size; ++index)
        {
          int32_t start = std::max(0, index - struc_size_half);
          int32_t stop  = std::min(size - 1, index + struc_size_half);
          ValueType value = input_begin[start];
          for (int32_t i = start + 1; i <= stop; ++i) if (value > input_begin[i]) value = input_begin[i];
          output_begin[index] = value;
        }
      return;
    }

    /// Applies dilation.  Simple implementation, possibly faster if struc_size is very small, and used in some special cases.
    template <typename InputIterator, typename OutputIterator>
    void applyDilationSimple_(int32_t struc_size, InputIterator input_begin, InputIterator input_end, OutputIterator output_begin)
    {
      typedef typename InputIterator::value_type ValueType;
      const int32_t size = input_end - input_begin;
      const int32_t struc_size_half = struc_size / 2;           // yes integer division
      for (int32_t index = 0; index < size; ++index)
        {
          int32_t start = std::max(0, index - struc_size_half);
          int32_t stop   = std::min(size - 1, index + struc_size_half);
          ValueType value = input_begin[start];
          for (int32_t i = start + 1; i <= stop; ++i) if (value < input_begin[i]) value = input_begin[i];
          output_begin[index] = value;
        }
      return;
    }


  };

} // namespace OpenMS

#endif // MorphologicalFilter_H
