# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories(${Gtest_LIBRARY})

ADD_EXECUTABLE(copy_iftest copy_iftest.cpp )
TARGET_LINK_LIBRARIES(copy_iftest  ${Boost_LIBRARIES} gtest)

ADD_EXECUTABLE(readLines readLines.cpp )
TARGET_LINK_LIBRARIES(readLines  ${Boost_LIBRARIES} gtest)


add_test(NAME copy_iftest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/copy_iftest )
set_tests_properties(copy_iftest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")


