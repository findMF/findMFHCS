
FIND_PACKAGE(Tbb)
if(Tbb_FOUND)
  message(STATUS "Tbb found" ${Tbb_INCLUDE_DIR})
  message(STATUS "Tbb found" ${Tbb_LIBRARY})
  SET(inc ${inc} ${Tbb_INCLUDE_DIR})
else()
  message(STATUS " !!!!!!!!!!!!!!!! Tbb not found !!!!!!!!!!!!!!!! ")
  return()
endif()




ADD_EXECUTABLE(testtbb testtbb.cpp )
TARGET_LINK_LIBRARIES(testtbb  tbb )

ADD_EXECUTABLE(pipelinetest pipelinetest.cpp )
TARGET_LINK_LIBRARIES(pipelinetest tbb rt)

ADD_EXECUTABLE(pipeline2 pipeline2.cpp )
TARGET_LINK_LIBRARIES(pipeline2 tbb rt)


ENABLE_TESTING()
add_test(NAME testtbb
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/testtbb )
set_tests_properties(testtbb PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")
