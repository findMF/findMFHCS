# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

FIND_PACKAGE(Soci)
if(Soci_FOUND)
  message(STATUS "Soci include found" ${Soci_INCLUDE_DIR})
  message(STATUS "Soci library found" ${Soci_LIBRARY})
else()
  message(SEND_ERROR " !!!!!!!!!!!!!!!! Soci not found !!!!!!!!!!!!!!!! ")
  return()
endif()
SET(inc ${inc} ${Soci_INCLUDE_DIR})
message(STATUS "inc with soci :::::::::::::"  ${inc})
message(STATUS "${Soci_LIBRARY}")
SET(SOCI_LIBS "soci_sqlite3 soci_core")


FIND_PACKAGE(Gtest)
if(Gtest_FOUND)
  message(STATUS "Gtest found" ${Gtest_INCLUDE_DIR})
  message(STATUS "Gtest found" ${Gtest_LIBRARY})
  SET(inc ${inc} ${Gtest_INCLUDE_DIR})
else()
  message(STATUS " !!!!!!!!!!!!!!!! Gtest not found !!!!!!!!!!!!!!!! ")
  return()
endif()

include_directories(${inc})
link_directories(${Pwiz_LIBRARY} ${VIGRA_LIBRARY} ${Glog_LIBRARY})

SET(transition_SOURCES preliminarytests.cpp)

ADD_EXECUTABLE(premtest ${transition_SOURCES})
TARGET_LINK_LIBRARIES(premtest ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs}  datastructlib widgets glog)

ENABLE_TESTING()
add_test(NAME premtest
WORKING_DIRECTORY ${BinOut}
COMMAND ${BinOut}/premtest)
set_tests_properties(premtest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

ADD_SUBDIRECTORY(datastruct)
ADD_SUBDIRECTORY(fileio)
ADD_SUBDIRECTORY(tbbtests)
ADD_SUBDIRECTORY(pwiztests)
ADD_SUBDIRECTORY(algo)

