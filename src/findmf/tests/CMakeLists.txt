# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include(externalgoogletest)
#set(inc ${inc} ${Gtest_SourceDir})
message(STATUS ">>>>>>>>>>>>>>>>>>> Gtest_LIBRARY ${Gtest_LIBRARY} <<<<<<<<<<<<<<< ")
message(STATUS ">>>>>>>>>>>>>>>>>>> vigralibs ${vigralibs} <<<<<<<<<<<<<<< ")

#message(STATUS "inc ${inc}")
set(vigralibs vigraimpex)

include_directories(${inc})
link_directories(${Pwiz_LIBRARY} ${VIGRA_LIBRARY})


ADD_EXECUTABLE(premtest preliminarytests.cpp)
TARGET_LINK_LIBRARIES(premtest ${Boost_LIBRARIES} pwiz ${vigralibs}  datastructlib )

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

