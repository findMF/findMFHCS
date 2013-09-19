FIND_PATH(Tbb_INCLUDE_DIR tbb/tbb.h)
FIND_LIBRARY(Tbb_LIBRARY NAMES tbb)

IF (Tbb_INCLUDE_DIR AND Tbb_LIBRARY)
   MESSAGE(STATUS "XXXXXXXXXXXXX ${Tbb_LIBRARY} XXXXXXXXXXXXXXX")
   SET(Tbb_FOUND TRUE)
ENDIF (Tbb_INCLUDE_DIR AND Tbb_LIBRARY)

IF (Tbb_FOUND)
   # show which CppUnit was found only if not quiet
   IF (NOT Tbb_FIND_QUIETLY)
      MESSAGE(STATUS "Found Tbb: ${Tbb_LIBRARY}")
   ENDIF (NOT Tbb_FIND_QUIETLY)
ELSE (Tbb_FOUND)
   # fatal error if CppUnit is required but not found
   IF (Tbb_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Tbb please do specify the Tbb_INCLUDE_DIR and the Tbb_LIBRARY variables using ccmake!")
   ENDIF (Tbb_FIND_REQUIRED)
ENDIF (Tbb_FOUND)
