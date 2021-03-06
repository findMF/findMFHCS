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

#### configuring vigra ####
#include(externalvigra)
#set(inc ${inc} ${VIGRA_SourceDir})
#message(STATUS "VIGRA_LIBRARY ${VIGRA_LIBRARY}")
#message(STATUS "inc ${inc}")
#set(vigralibs vigraimpex)

FIND_PACKAGE(Vigra REQUIRED)
if(VIGRA_FOUND)
  message(STATUS "Vigra found :" ${VIGRA_INCLUDE_DIR})
  message(STATUS "Vigra library :" ${VIGRA_LIBRARY})

  SET(inc ${inc} ${VIGRA_INCLUDE_DIR})
  SET(vigralibs vigraimpex)
else(VIGRA_FOUND)
  message(FATAL_ERROR " !!!!!!!!!!!!!!!! Vigra !!!!!!!!!!!!!!!! ")
  return()
endif(VIGRA_FOUND)


FIND_PACKAGE(FindMFBase REQUIRED)
if(FindMFBase_FOUND)
  message(STATUS "FindMF found" ${FindMFBase_INCLUDE_DIR})
  SET(inc ${inc} ${FindMFBase_INCLUDE_DIR})
else(FindMFBase_FOUND)
  message(FATAL_ERROR " !!!!!!!!!!!!!!!! FindMFBase !!!!!!!!!!!!!!!! ")
  return()
endif(FindMFBase_FOUND)


FIND_PACKAGE(FindMFInterfaces REQUIRED)
if(FindMFInterfaces_FOUND)
  message(STATUS "FindMF found" ${FindMFInterfaces_INCLUDE_DIR})
  SET(inc ${inc} ${FindMFInterfaces_INCLUDE_DIR})
else(FindMFInterfaces_FOUND)
  message(FATAL_ERROR " !!!!!!!!!!!!!!!! FindMFInterfaces !!!!!!!!!!!!!!!! ")
  return()
endif(FindMFInterfaces_FOUND)


ADD_SUBDIRECTORY(findmf)

ENABLE_TESTING()


