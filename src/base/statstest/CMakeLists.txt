# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories(${Gtest_LIBRARY})

ADD_EXECUTABLE(statstest statstest.cpp )
TARGET_LINK_LIBRARIES(statstest  ${Boost_LIBRARIES} gtest)

ADD_EXECUTABLE(scaletest scaletest.cpp )
TARGET_LINK_LIBRARIES(scaletest  ${Boost_LIBRARIES} gtest)


add_test(NAME statstest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/statstest )
set_tests_properties(statstest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")


