# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories(  ${Soci_LIBRARY} )

message(STATUS ">>>>>>> ${Boost_LIBRARIES} >>>>")
message(STATUS ">>>>>>> >>>>")

ADD_EXECUTABLE(sqlfeaturestoragetest SQLFeatureStorageTest.cpp )
TARGET_LINK_LIBRARIES(sqlfeaturestoragetest  ${QT_LIBRARIES} ${Boost_LIBRARIES} )

ADD_EXECUTABLE(featureTable featureTable.cpp )
TARGET_LINK_LIBRARIES(featureTable  ${QT_LIBRARIES} ${Boost_LIBRARIES} )

ADD_EXECUTABLE( testRtree testRtree.cpp )
TARGET_LINK_LIBRARIES(testRtree  ${QT_LIBRARIES} ${Boost_LIBRARIES})

ADD_EXECUTABLE(SQLFacadeTest SQLFacadeTest.cpp )
TARGET_LINK_LIBRARIES(SQLFacadeTest  ${QT_LIBRARIES} ${Boost_LIBRARIES} )

ENABLE_TESTING()

add_test(NAME sqlfeaturestoragetest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/sqlfeaturestoragetest )
set_tests_properties(sqlfeaturestoragetest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME featureTable
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/featureTable )
set_tests_properties(featureTable PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME testRtree
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/testRtree )
set_tests_properties(testRtree PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME SQLFacadeTest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/SQLFacadeTest )
set_tests_properties(SQLFacadeTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")


