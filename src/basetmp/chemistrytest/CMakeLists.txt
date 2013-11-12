include_directories(${inc})
link_directories(${Gtest_LIBRARY})
ADD_EXECUTABLE(IisotopeEnvelopeTest IisotopeEnvelopeTest.cpp)
TARGET_LINK_LIBRARIES(IisotopeEnvelopeTest  gtest)


add_test(NAME IisotopeEnvelopeTest
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
COMMAND ${BinOut}/IisotopeEnvelopeTest )
set_tests_properties(IisotopeEnvelopeTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")
