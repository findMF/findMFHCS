# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

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

ADD_EXECUTABLE(testThreading testThreading.cpp)
TARGET_LINK_LIBRARIES(testThreading ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs} datastructlib widgets tbb glog)


ADD_EXECUTABLE(findMF findMF.cpp  )
TARGET_LINK_LIBRARIES(findMF ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs}  datastructlib widgets tbb glog)


ADD_EXECUTABLE(PeakPicker PeakPicker.cpp)
TARGET_LINK_LIBRARIES(PeakPicker ${Boost_LIBRARIES} pwiz datastructlib glog)
