# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

FIND_PATH(FindMFBase_INCLUDE_DIR base/base/base.h)

IF (FindMFBase_INCLUDE_DIR)
   MESSAGE(STATUS "XXXXXXXXXXXXX ${FindMFBase_INCLUDE_DIR} XXXXXXXXXXXXXXX")
   SET(FindMFBase_FOUND TRUE)
ENDIF (FindMFBase_INCLUDE_DIR)


IF (FindMFBase_FOUND)
   IF (NOT FindMFBase_FIND_QUIETLY)
      MESSAGE(STATUS "Found FindMFBase: ${FindMFBase_INCLUDE_DIR}")
   ENDIF (NOT FindMFBase_FIND_QUIETLY)
ELSE (FindMFBase_FOUND)
   IF (FindMFBase_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find FindMFBase please do specify the FindMFBase_INCLUDE_DIR variables using ccmake!")
   ENDIF (FindMFBase_FIND_REQUIRED)
ENDIF (FindMFBase_FOUND)

