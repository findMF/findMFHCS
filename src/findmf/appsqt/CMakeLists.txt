include_directories(${inc})
link_directories( ${Pwiz_LIBRARY} ${VIGRA_LIBRARY} ${Tbb_LIBRARY} ${Glog_LIBRARY})


SET(featurefind_SOURCES filterQTView.cpp)

ADD_EXECUTABLE(filterQTView ${featurefind_SOURCES})
TARGET_LINK_LIBRARIES(filterQTView ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs}  datastructlib widgets glog)
