# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories(${Gtest_LIBRARY})


ADD_EXECUTABLE(scaletest scaletest.cpp )
TARGET_LINK_LIBRARIES(scaletest  ${Boost_LIBRARIES} gtest)


ADD_EXECUTABLE(quantilestest quantilestest.cpp )
TARGET_LINK_LIBRARIES(quantilestest  ${Boost_LIBRARIES} gtest)


ADD_EXECUTABLE(momentstest momentstest.cpp )
TARGET_LINK_LIBRARIES(momentstest  ${Boost_LIBRARIES} gtest)


ADD_EXECUTABLE(momentsWtest momentsWtest.cpp )
TARGET_LINK_LIBRARIES(momentsWtest  ${Boost_LIBRARIES} gtest)



add_test(NAME scaletest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/scaletest )
set_tests_properties(scaletest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME quantilestest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/quantilestest )
set_tests_properties(quantilestest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME momentstest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/momentstest )
set_tests_properties(momentstest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME momentsWtest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/momentsWtest )
set_tests_properties(momentsWtest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

