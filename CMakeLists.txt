#Author : Witold Wolski

PROJECT(findmf)
cmake_minimum_required(VERSION 2.8)

message(STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
#set(CMAKE_BUILD_TYPE)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
include(findmfDefaults)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-value")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_definitions("-Doverride=")
#add_definitions("-DBOOST_FILESYSTEM_VERSION=3")
add_definitions("-DCMAKE_VERBOSE_MAKEFILE=ON")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

##define output
set( BinOut ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}out/${} )
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BinOut} )
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BinOut} )
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BinOut} )

##
## find packages
##

SET(BOOST_MIN_VERSION "1.48.0")
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


FIND_PACKAGE(Pwiz)
if(Pwiz_FOUND)
  message(STATUS "Pwiz found" ${Pwiz_INCLUDE_DIR})
  message(STATUS "Pwiz found" ${Pwiz_LIBRARY})
  SET(inc ${inc} ${Pwiz_INCLUDE_DIR})
else()
  message(STATUS " !!!!!!!!!!!!!!!! Pwiz not found !!!!!!!!!!!!!!!! ")
  return()
endif()

FIND_PACKAGE(Vigra)
if(VIGRA_FOUND)
  message(STATUS "Vigra include found" ${VIGRA_INCLUDE_DIR})
  message(STATUS "Vigra library found" ${VIGRA_LIBRARY})
else()
  message(STATUS " !!!!!!!!!!!!!!!! Vigra not found !!!!!!!!!!!!!!!! ")
  return()
endif()
SET(inc ${inc} ${VIGRA_INCLUDE_DIR})

SET(inc ${inc} ${CMAKE_SOURCE_DIR})
SET(inc ${inc} ${CMAKE_SOURCE_DIR}/include)


#QT tweak so that it sees headers
file(GLOB Demo_HEADERS "include/*.h" "include/**/*.h" "include/**/**/*.h" "include/**/**/**/*.h" "src/**/*.h" "src/**/**/*.h")
add_library(headers SHARED ${Demo_HEADERS} Dummy.cpp)



ENABLE_TESTING()
ADD_SUBDIRECTORY(src)

