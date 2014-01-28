# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories(${VIGRA_LIBRARY})

message(STATUS ">>>>>>>>>>>>>>>>>>> VIGRA_LIBRARY ${VIGRA_LIBRARY} <<<<<<<<<<<<<<< ")
message(STATUS ">>>>>>>>>>>>>>>>>>> vigralibs ${vigralibs} <<<<<<<<<<<<<<< ")


#ADD_EXECUTABLE(map2test  map2test.cpp)
#TARGET_LINK_LIBRARIES(map2test ${Boost_LIBRARIES} ${vigralibs} datastructlib )

ADD_EXECUTABLE(featuresmaptest featuresmaptest.cpp)
TARGET_LINK_LIBRARIES(featuresmaptest ${Boost_LIBRARIES} ${vigralibs}  )

ADD_EXECUTABLE(maplcmsdescriptiontest maplcmsdescriptiontest.cpp)
TARGET_LINK_LIBRARIES(maplcmsdescriptiontest ${Boost_LIBRARIES} ${vigralibs}  )


ADD_EXECUTABLE(map2dtest map2dtest.cpp)
TARGET_LINK_LIBRARIES(map2dtest ${Boost_LIBRARIES} ${vigralibs})


ADD_EXECUTABLE(lcmsimagetest lcmsimagetest.cpp)
TARGET_LINK_LIBRARIES(lcmsimagetest ${Boost_LIBRARIES} ${vigralibs})


ENABLE_TESTING()

add_test(NAME lcmsimagetest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/lcmsimagetest )
set_tests_properties(lcmsimagetest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME maplcmsdescriptiontest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/maplcmsdescriptiontest )
set_tests_properties(maplcmsdescriptiontest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME map2test
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/map2test )
set_tests_properties(map2test PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME featuresmaptest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/featuresmaptest )
set_tests_properties(featuresmaptest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")
