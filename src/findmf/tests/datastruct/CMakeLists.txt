# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories( ${VIGRA_LIBRARY} ${Glog_LIBRARY})

set(src map2test.cpp)
ADD_EXECUTABLE(map2test ${src})
TARGET_LINK_LIBRARIES(map2test ${Boost_LIBRARIES} ${vigralibs} datastructlib glog)



ENABLE_TESTING()
add_test(NAME map2test
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/map2test )
set_tests_properties(map2test PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")
