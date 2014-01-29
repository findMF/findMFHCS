# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

SET(BOOST_MIN_VERSION "1.46.0")
find_package( Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS filesystem system iostreams thread regex program_options unit_test_framework)
if(Boost_FOUND)
  SET( inc ${inc} ${Boost_INCLUDE_DIRS} )
  SET( ${boostlib} ${Boost_LIBRARIES} )
else()
  message(STATUS ">>>>> Couldn't find Boost <<<<< ")
endif()

message( STATUS "lib ${lib}" )
message( STATUS "inc boost ${inc}" )
message( STATUS ">>>> ${Boost_LIBRARIES} <<<<" )


include(ExternalProject)

include(externalvigra)
set(inc ${inc} ${VIGRA_SourceDir})
message(STATUS "VIGRA_LIBRARY ${VIGRA_LIBRARY}")
message(STATUS "inc ${inc}")


FIND_PACKAGE(FindMFBase)
if(FindMFBase_FOUND)
  message(STATUS "FindMF found" ${FindMFBase_INCLUDE_DIR})
  SET(inc ${inc} ${FindMFBase_INCLUDE_DIR})
else(FindMFBase_FOUND)
  message(STATUS " !!!!!!!!!!!!!!!! Tbb not found !!!!!!!!!!!!!!!! ")
  return()
endif(FindMFBase_FOUND)


#if(0)
#include(externalfindmfbase)
#set(inc ${inc} ${findMFBase_SourceDir})
#message(STATUS "findMFBase_LIBRARY ${findMFBase_LIBRARY}")
#message(STATUS "inc ${inc}")
#endif(0)


ADD_SUBDIRECTORY(findmf)

#IF(WITH_TEST)
#ENABLE_TESTING()
#ADD_SUBDIRECTORY(base)
#ENDIF()


