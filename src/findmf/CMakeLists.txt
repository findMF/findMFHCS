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

FIND_PACKAGE(Tbb)
if(Tbb_FOUND)
  message(STATUS "Tbb found" ${Tbb_INCLUDE_DIR})
  message(STATUS "Tbb found" ${Tbb_LIBRARY})
  SET(inc ${inc} ${Tbb_INCLUDE_DIR})
else()
  message(STATUS " !!!!!!!!!!!!!!!! Tbb not found !!!!!!!!!!!!!!!! ")
  return()
endif()



FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtSql REQUIRED)
INCLUDE(${QT_USE_FILE})
message( STATUS "<<<<<<<<<<<<<<QTQTsrc  ${QT_LIBRARIES} QTQT>>>>>>>>>>>>")


ADD_SUBDIRECTORY(application)
ADD_SUBDIRECTORY(apps)


IF(WITH_QT)
ADD_SUBDIRECTORY(appsqt)
ADD_SUBDIRECTORY(qtwidgets)
ENDIF()


IF(WITH_TEST)
ADD_SUBDIRECTORY(tests)
ENDIF()
