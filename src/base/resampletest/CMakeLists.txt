# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories(${Gtest_LIBRARY})


ADD_EXECUTABLE(equispaceinterpolationtest equispaceinterpolationtest.cpp )
TARGET_LINK_LIBRARIES(equispaceinterpolationtest  ${Boost_LIBRARIES} gtest)

ADD_EXECUTABLE(bucket1dtest bucket1dtest.cpp )
TARGET_LINK_LIBRARIES(bucket1dtest  ${Boost_LIBRARIES} gtest)


ADD_EXECUTABLE(convert2densetest convert2densetest.cpp )
TARGET_LINK_LIBRARIES(convert2densetest  ${Boost_LIBRARIES} gtest)

ADD_EXECUTABLE(determinbinwithtest determinbinwithtest.cpp )
TARGET_LINK_LIBRARIES(determinbinwithtest  ${Boost_LIBRARIES} gtest)

add_test(NAME equispaceinterpolationtest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/equispaceinterpolationtest )
set_tests_properties(equispaceinterpolationtest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME bucket1dtest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/bucket1dtest )
set_tests_properties(bucket1dtest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME convert2densetest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/convert2densetest )
set_tests_properties(convert2densetest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")


add_test(NAME determinbinwithtest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/determinbinwithtest )
set_tests_properties(determinbinwithtest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

