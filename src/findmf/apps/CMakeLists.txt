# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


include_directories(${inc})
link_directories( ${Pwiz_LIBRARY} ${VIGRA_LIBRARY} ${Tbb_LIBRARY} )

ADD_EXECUTABLE(testThreading testThreading.cpp)
TARGET_LINK_LIBRARIES(testThreading ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs} datastructlib tbb )

ADD_EXECUTABLE(findMF findMF.cpp)
TARGET_LINK_LIBRARIES(findMF ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs}  datastructlib tbb )

ADD_EXECUTABLE(PeakPicker PeakPicker.cpp)
TARGET_LINK_LIBRARIES(PeakPicker ${Boost_LIBRARIES} pwiz datastructlib )

#ADD_EXECUTABLE(conv2Image conv2Image.cpp)
#TARGET_LINK_LIBRARIES(conv2Image  ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz datastructlib ${vigralibs} tbb)
