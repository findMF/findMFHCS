# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

FIND_PACKAGE(Gtest)
if(Gtest_FOUND)
  message(STATUS "Gtest found" ${Gtest_INCLUDE_DIR})
  message(STATUS "Gtest found" ${Gtest_LIBRARY})
  SET(inc ${inc} ${Gtest_INCLUDE_DIR})
else()
  message(STATUS "!!!!!!!!!!!!!!!! Gtest not found !!!!!!!!!!!!!!!! ")
  return()
endif()


ADD_SUBDIRECTORY(basetest)
ADD_SUBDIRECTORY(filtertest)
ADD_SUBDIRECTORY(resampletest)
ADD_SUBDIRECTORY(statstest)
ADD_SUBDIRECTORY(mstest)
ADD_SUBDIRECTORY(chemistrytest)
ADD_SUBDIRECTORY(utilstest)

