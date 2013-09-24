# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

FIND_PATH(Soci_INCLUDE_DIR soci/soci.h)
FIND_LIBRARY(Soci_LIBRARY NAMES soci)

IF (Soci_INCLUDE_DIR AND Soci_LIBRARY)
   MESSAGE(STATUS "XXXXXXXXXXXXX ${Soci_LIBRARY} XXXXXXXXXXXXXXX")
   SET(Soci_FOUND TRUE)
ENDIF (Soci_INCLUDE_DIR AND Soci_LIBRARY)

IF (Soci_FOUND)
   # show which CppUnit was found only if not quiet
   IF (NOT Soci_FIND_QUIETLY)
      MESSAGE(STATUS "Found Soci: ${Soci_LIBRARY}")
   ENDIF (NOT Soci_FIND_QUIETLY)
ELSE (Soci_FOUND)
   # fatal error if CppUnit is required but not found
   IF (Soci_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Soci please do specify the Soci_INCLUDE_DIR and the Soci_LIBRARY variables using ccmake!")
   ENDIF (Soci_FIND_REQUIRED)
ENDIF (Soci_FOUND)

