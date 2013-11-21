# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories(${VIGRA_LIBRARY})

set(src projectionstats.cpp)
ADD_EXECUTABLE(projectionstats ${src})
TARGET_LINK_LIBRARIES(projectionstats ${Boost_LIBRARIES} ${vigralibs})

add_test(NAME projectionstats
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/projectionstats )
set_tests_properties(projectionstats PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")
