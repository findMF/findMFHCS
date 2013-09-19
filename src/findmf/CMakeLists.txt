FIND_PACKAGE(Glog)
if(Glog_FOUND)
  message(STATUS "Glog found" ${Glog_INCLUDE_DIR})
  message(STATUS "Glog found" ${Glog_LIBRARY})
  SET(inc ${inc} ${Glog_INCLUDE_DIR})
else()
  message(STATUS " !!!!!!!!!!!!!!!! Glog not found !!!!!!!!!!!!!!!! ")
  return()
endif()

FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtSql REQUIRED)
INCLUDE(${QT_USE_FILE})
message( STATUS "<<<<<<<<<<<<<<QTQTsrc  ${QT_LIBRARIES} QTQT>>>>>>>>>>>>")


message(STATUS "incincincincinc")
message(STATUS "inc directory in src = ${inc}")
message(STATUS "incincincincinc")



ADD_SUBDIRECTORY(application)
ADD_SUBDIRECTORY(apps)
ADD_SUBDIRECTORY(qtwidgets)


IF(WITH_TEST)
ADD_SUBDIRECTORY(tests)
ENDIF()
