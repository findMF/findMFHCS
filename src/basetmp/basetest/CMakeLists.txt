#Author : Witold Wolski

include_directories(${inc})
link_directories(${Gtest_LIBRARY})


ADD_EXECUTABLE(basetest basetest.cpp )
TARGET_LINK_LIBRARIES(basetest  ${Boost_LIBRARIES} gtest)


ADD_EXECUTABLE(cumsumtest cumsumtest.cpp )
TARGET_LINK_LIBRARIES(cumsumtest  ${Boost_LIBRARIES} gtest)


ADD_EXECUTABLE(difftest difftest.cpp )
TARGET_LINK_LIBRARIES(difftest  ${Boost_LIBRARIES} gtest)


ENABLE_TESTING()

add_test(NAME basetest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/basetest )
set_tests_properties(basetest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME cumsumtest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/cumsumtest )
set_tests_properties(cumsumtest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME difftest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/difftest )
set_tests_properties(difftest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")


