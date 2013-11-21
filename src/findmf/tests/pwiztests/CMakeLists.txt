# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories(${VIGRA_LIBRARY})

ADD_EXECUTABLE(deisotopertest DeisotoperTest.cpp )
TARGET_LINK_LIBRARIES(deisotopertest  ${Boost_LIBRARIES}  pwiz)


ADD_EXECUTABLE(IsotopeEnvelope IsotopeEnvelope.cpp )
TARGET_LINK_LIBRARIES(IsotopeEnvelope  ${Boost_LIBRARIES} pwiz)


add_test(NAME deisotopertest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/deisotopertest )
set_tests_properties(deisotopertest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")
