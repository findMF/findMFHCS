# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

FIND_PATH(Pwiz_INCLUDE_DIR pwiz/Version.hpp)
FIND_LIBRARY(Pwiz_LIBRARY NAMES pwiz)

IF (Pwiz_INCLUDE_DIR AND Pwiz_LIBRARY)
   MESSAGE(STATUS "XXXXXXXXXXXXX ${Pwiz_LIBRARY} XXXXXXXXXXXXXXX")
   SET(Pwiz_FOUND TRUE)
ENDIF (Pwiz_INCLUDE_DIR AND Pwiz_LIBRARY)

IF (Pwiz_FOUND)
   # show which CppUnit was found only if not quiet
   IF (NOT Pwiz_FIND_QUIETLY)
      MESSAGE(STATUS "Found Pwiz: ${Pwiz_LIBRARY}")
   ENDIF (NOT Pwiz_FIND_QUIETLY)
ELSE (Pwiz_FOUND)
   # fatal error if CppUnit is required but not found
   IF (Pwiz_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Pwiz please do specify the Pwiz_INCLUDE_DIR and the Pwiz_LIBRARY variables using ccmake!")
   ENDIF (Pwiz_FIND_REQUIRED)
ENDIF (Pwiz_FOUND)

