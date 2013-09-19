FIND_PATH(STATS_INCLUDE_DIR stats/Version.hpp)
FIND_LIBRARY(STATS_LIBRARY NAMES stats)


IF (STATS_INCLUDE_DIR AND STATS_LIBRARY)
   message(STATUS " stats module, stats found")
   SET(STATS_FOUND TRUE)
ENDIF (STATS_INCLUDE_DIR AND STATS_LIBRARY)

IF (STATS_FOUND)
   # show which CppUnit was found only if not quiet
   IF (NOT STATS_FIND_QUIETLY)
      MESSAGE(STATUS "Found Stats: ${STATS_LIBRARY}")
   ENDIF (NOT STATS_FIND_QUIETLY)
ELSE (STATS_FOUND)
   # fatal error if CppUnit is required but not found
   IF (STATS_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Stats")
   ENDIF (STATS_FIND_REQUIRED)
ENDIF (STATS_FOUND)

