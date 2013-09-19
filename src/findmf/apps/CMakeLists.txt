#ADD_SUBDIRECTORY(denoise)
#ADD_SUBDIRECTORY(featurefind)

FIND_PACKAGE(Tbb)
if(Tbb_FOUND)
  message(STATUS "Tbb found" ${Tbb_INCLUDE_DIR})
  message(STATUS "Tbb found" ${Tbb_LIBRARY})
  SET(inc ${inc} ${Tbb_INCLUDE_DIR})
else()
  message(STATUS " !!!!!!!!!!!!!!!! Tbb not found !!!!!!!!!!!!!!!! ")
  return()
endif()


include_directories(${inc})
link_directories( ${Pwiz_LIBRARY} ${VIGRA_LIBRARY} ${Tbb_LIBRARY} ${Glog_LIBRARY})


#SET(transition_SOURCES transitionExtractMain.cpp)

#ADD_EXECUTABLE(transitionExtract ${transition_SOURCES})
#TARGET_LINK_LIBRARIES(transitionExtract ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs}  stats utils wavelib datastructlib widgets glog)

ADD_EXECUTABLE(testThreading testThreading.cpp)
TARGET_LINK_LIBRARIES(testThreading ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs} datastructlib widgets tbb glog)


ADD_EXECUTABLE(findMF findMF.cpp  )
TARGET_LINK_LIBRARIES(findMF ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs}  datastructlib widgets tbb glog)

SET(featurefind_SOURCES filterQTView.cpp)

ADD_EXECUTABLE(filterQTView ${featurefind_SOURCES})
TARGET_LINK_LIBRARIES(filterQTView ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs}  datastructlib widgets glog)

ADD_EXECUTABLE(PeakPicker PeakPicker.cpp)
TARGET_LINK_LIBRARIES(PeakPicker ${Boost_LIBRARIES} pwiz datastructlib glog)
