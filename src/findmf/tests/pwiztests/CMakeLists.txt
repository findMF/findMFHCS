# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

ADD_EXECUTABLE(IsotopeEnvelope IsotopeEnvelope.cpp )
TARGET_LINK_LIBRARIES(IsotopeEnvelope  ${Boost_LIBRARIES} glog pwiz)


