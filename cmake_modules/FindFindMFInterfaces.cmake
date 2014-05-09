# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

FIND_PATH(FindMFInterfaces_INCLUDE_DIR findmf/interfaces/ifeatureaccess.h)

IF (FindMFInterfaces_INCLUDE_DIR)
   MESSAGE(STATUS "XXXXXXXXXXXXX ${FindMFInterfaces_INCLUDE_DIR} XXXXXXXXXXXXXXX")
   SET(FindMFInterfaces_FOUND TRUE)
ENDIF (FindMFInterfaces_INCLUDE_DIR)

IF (FindMFInterfaces_FOUND)
   # show which CppUnit was found only if not quiet
   IF (NOT FindMFInterfaces_FIND_QUIETLY)
      MESSAGE(STATUS "Found FindMFInterfaces: ${FindMFInterfaces_INCLUDE_DIR}")
   ENDIF (NOT FindMFInterfaces_FIND_QUIETLY)
ELSE (FindMFInterfaces_FOUND)
   # fatal error if CppUnit is required but not found
   IF (FindMFInterfaces_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find FindMFInterfaces please do specify the FindMFInterfaces_INCLUDE_DIR variables using ccmake!")
   ENDIF (FindMFInterfaces_FIND_REQUIRED)
ENDIF (FindMFInterfaces_FOUND)

