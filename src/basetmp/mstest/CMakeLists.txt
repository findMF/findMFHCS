# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories(${Gtest_LIBRARY})

ADD_EXECUTABLE(simplepickertest simplepickertest.cpp )
TARGET_LINK_LIBRARIES(simplepickertest  ${Boost_LIBRARIES} gtest)

ADD_EXECUTABLE(peakpickertest peakpickertest.cpp )
TARGET_LINK_LIBRARIES(peakpickertest  ${Boost_LIBRARIES} gtest)

ADD_EXECUTABLE(parabolafittest parabolafittest.cpp )
TARGET_LINK_LIBRARIES(parabolafittest  ${Boost_LIBRARIES} gtest)


ENABLE_TESTING()

add_test(NAME parabolafittest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/parabolafittest )
set_tests_properties(parabolafittest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")


add_test(NAME simplepickertest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/simplepickertest )
set_tests_properties(simplepickertest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")


add_test(NAME peakpickertest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/peakpickertest )
set_tests_properties(peakpickertest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")


