# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

FIND_PATH(Glog_INCLUDE_DIR glog/logging.h)
FIND_LIBRARY(Glog_LIBRARY NAMES glog)


IF(Glog_INCLUDE_DIR AND Glog_LIBRARY)
   message(STATUS " Glog module, Glog found")
   SET(Glog_FOUND TRUE)
ENDIF(Glog_INCLUDE_DIR AND Glog_LIBRARY)

IF (Glog_FOUND)
   # show which CppUnit was found only if not quiet
   IF (NOT Glog_FIND_QUIETLY)
      MESSAGE(STATUS "Found Glog: ${Glog_LIBRARY}")
   ENDIF (NOT Glog_FIND_QUIETLY)
ELSE (Glog_FOUND)
       MESSAGE(STATUS "NOT Found Glog: ${Glog_LIBRARY} ${Glog_INCLUDE_DIR}")
   # fatal error if CppUnit is required but not found
   IF (Glog_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Glog")
   ENDIF (Glog_FIND_REQUIRED)
ENDIF (Glog_FOUND)

