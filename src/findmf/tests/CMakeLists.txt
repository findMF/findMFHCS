#Author : Witold Wolski

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

#ADD_SUBDIRECTORY(algotests)
ADD_SUBDIRECTORY(datastruct)
ADD_SUBDIRECTORY(fileio)
ADD_SUBDIRECTORY(tbbtests)
ADD_SUBDIRECTORY(pwiztests)

