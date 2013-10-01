SET(datastruct_SOURCES parsesql.cpp)

include_directories(${inc})
#link_directories(${Glog_LIBRARY})

add_library(utilslib ${datastruct_SOURCES})
#target_link_libraries(datastructlib)
