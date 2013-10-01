# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


FIND_PACKAGE(Pwiz)
if(Pwiz_FOUND)
  message(STATUS "Pwiz found" ${Pwiz_INCLUDE_DIR})
  message(STATUS "Pwiz found" ${Pwiz_LIBRARY})
  SET(inc ${inc} ${Pwiz_INCLUDE_DIR})
else()
  message(SEND_ERROR " !!!!!!!!!!!!!!!! Pwiz not found !!!!!!!!!!!!!!!! ")
  return()
endif()


FIND_PACKAGE(Glog)
if(Glog_FOUND)
  message(STATUS "Glog found" ${Glog_INCLUDE_DIR})
  message(STATUS "Glog found" ${Glog_LIBRARY})
  SET(inc ${inc} ${Glog_INCLUDE_DIR})
else()
  message(SEND_ERROR " !!!!!!!!!!!!!!!! Glog not found !!!!!!!!!!!!!!!! ")
  return()
endif()

FIND_PACKAGE(Vigra)
if(VIGRA_FOUND)
  message(STATUS "Vigra include found" ${VIGRA_INCLUDE_DIR})
  message(STATUS "Vigra library found" ${VIGRA_LIBRARY})
else()
  message(SEND_ERROR " !!!!!!!!!!!!!!!! Vigra not found !!!!!!!!!!!!!!!! ")
  return()
endif()
SET(inc ${inc} ${VIGRA_INCLUDE_DIR})

FIND_PACKAGE(Soci)
if(Soci_FOUND)
  message(STATUS "Soci include found" ${Soci_INCLUDE_DIR})
  message(STATUS "Soci library found" ${Soci_LIBRARY})
else()
  message(SEND_ERROR " !!!!!!!!!!!!!!!! Soci not found !!!!!!!!!!!!!!!! ")
  return()
endif()
SET(inc ${inc} ${Soci_INCLUDE_DIR})
message(STATUS "inc with soci :::::::::::::"  ${inc})
message(STATUS "${Soci_LIBRARY}")
SET(SOCI_LIBS "soci_sqlite3 soci_core")

message(STATUS ${SOCI_LIBS})


FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtSql REQUIRED)
INCLUDE(${QT_USE_FILE})
message( STATUS "<<<<<<<<<<<<<<QTQTsrc  ${QT_LIBRARIES} QTQT>>>>>>>>>>>>")

#message(STATUS "incincincincinc")
#message(STATUS "inc directory in src = ${inc}")
#message(STATUS "incincincincinc")
ADD_SUBDIRECTORY(utils)
ADD_SUBDIRECTORY(application)
ADD_SUBDIRECTORY(apps)
ADD_SUBDIRECTORY(appsqt)
ADD_SUBDIRECTORY(qtwidgets)


IF(WITH_TEST)
ADD_SUBDIRECTORY(tests)
ENDIF()
