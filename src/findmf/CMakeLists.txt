FIND_PACKAGE(Glog)
if(Glog_FOUND)
  message(STATUS "Glog found" ${Glog_INCLUDE_DIR})
  message(STATUS "Glog found" ${Glog_LIBRARY})
  SET(inc ${inc} ${Glog_INCLUDE_DIR})
else()
  message(STATUS " !!!!!!!!!!!!!!!! Glog not found !!!!!!!!!!!!!!!! ")
  return()
endif()


FIND_PACKAGE(Pwiz)
if(Pwiz_FOUND)
  message(STATUS "Pwiz found" ${Pwiz_INCLUDE_DIR})
  message(STATUS "Pwiz found" ${Pwiz_LIBRARY})
  SET(inc ${inc} ${Pwiz_INCLUDE_DIR})
else()
  message(STATUS " !!!!!!!!!!!!!!!! Pwiz not found !!!!!!!!!!!!!!!! ")
  return()
endif()

FIND_PACKAGE(Vigra)
if(VIGRA_FOUND)
  message(STATUS "Vigra include found" ${VIGRA_INCLUDE_DIR})
  message(STATUS "Vigra library found" ${VIGRA_LIBRARY})
else()
  message(STATUS " !!!!!!!!!!!!!!!! Vigra not found !!!!!!!!!!!!!!!! ")
  return()
endif()
SET(inc ${inc} ${VIGRA_INCLUDE_DIR})


#FIND_PACKAGE(Soci)
#if(SOCI_FOUND)
#  message(STATUS "Vigra include found" ${SOCI_INCLUDE_DIR})
#  message(STATUS "Vigra library found" ${SOCI_LIBRARY})
#else()
#  message(STATUS " !!!!!!!!!!!!!!!! Soci not found !!!!!!!!!!!!!!!! ")
#  return()
#endif()
#SET(inc ${inc} ${SOCI_INCLUDE_DIR})


FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtSql REQUIRED)
INCLUDE(${QT_USE_FILE})
message( STATUS "<<<<<<<<<<<<<<QTQTsrc  ${QT_LIBRARIES} QTQT>>>>>>>>>>>>")

#message(STATUS "incincincincinc")
#message(STATUS "inc directory in src = ${inc}")
#message(STATUS "incincincincinc")


ADD_SUBDIRECTORY(application)
ADD_SUBDIRECTORY(apps)
ADD_SUBDIRECTORY(appsqt)
ADD_SUBDIRECTORY(qtwidgets)


IF(WITH_TEST)
ADD_SUBDIRECTORY(tests)
ENDIF()
