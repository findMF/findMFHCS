# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

SET(datastruct_SOURCES lcmsimage.cpp)


include_directories(${inc})
link_directories(${Glog_LIBRARY})

add_library(datastructlib ${datastruct_SOURCES})
target_link_libraries(datastructlib glog vigraimpex ${Boost_LIBRARIES})
