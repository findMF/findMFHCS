# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

# TODO move this out into findMFSQL

include_directories(${inc})
link_directories(  ${Soci_LIBRARY} )

ADD_EXECUTABLE(sqlfeaturestoragetest SQLFeatureStorageTest.cpp )
TARGET_LINK_LIBRARIES(sqlfeaturestoragetest  ${QT_LIBRARIES} ${Boost_LIBRARIES} )

ADD_EXECUTABLE(sqlmapinfotabletest sqlmapinfotabletest.cpp )
TARGET_LINK_LIBRARIES(sqlmapinfotabletest  ${QT_LIBRARIES} ${Boost_LIBRARIES} )

ADD_EXECUTABLE( testRtree testRtree.cpp )
TARGET_LINK_LIBRARIES(testRtree  ${QT_LIBRARIES} ${Boost_LIBRARIES})

ADD_EXECUTABLE(SQLFacadeTest SQLFacadeTest.cpp )
TARGET_LINK_LIBRARIES(SQLFacadeTest  ${QT_LIBRARIES} ${Boost_LIBRARIES} )

ENABLE_TESTING()

add_test(NAME sqlfeaturestoragetest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/sqlfeaturestoragetest )
set_tests_properties(sqlfeaturestoragetest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME sqlmapinfotabletest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/sqlmapinfotabletest )
set_tests_properties(sqlmapinfotabletest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME testRtree
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/testRtree )
set_tests_properties(testRtree PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

add_test(NAME SQLFacadeTest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/SQLFacadeTest )
set_tests_properties(SQLFacadeTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")


