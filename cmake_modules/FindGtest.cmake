# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

FIND_PATH(Gtest_INCLUDE_DIR gtest/gtest.h)
FIND_LIBRARY(Gtest_LIBRARY NAMES gtest)

IF (Gtest_INCLUDE_DIR AND Gtest_LIBRARY)
   MESSAGE(STATUS "XXXXXXXXXXXXX ${Gtest_LIBRARY} XXXXXXXXXXXXXXX")
   SET(Gtest_FOUND TRUE)
ENDIF (Gtest_INCLUDE_DIR AND Gtest_LIBRARY)

IF (Gtest_FOUND)
   # show which CppUnit was found only if not quiet
   IF (NOT Gtest_FIND_QUIETLY)
      MESSAGE(STATUS "Found Gtest: ${Gtest_LIBRARY}")
   ENDIF (NOT Gtest_FIND_QUIETLY)
ELSE (Gtest_FOUND)
   # fatal error if CppUnit is required but not found
   IF (Gtest_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Gtest please do specify the Gtest_INCLUDE_DIR and the Gtest_LIBRARY variables using ccmake!")
   ENDIF (Gtest_FIND_REQUIRED)
ENDIF (Gtest_FOUND)

