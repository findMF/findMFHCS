# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


include_directories(${inc})
link_directories( ${Pwiz_LIBRARY} ${VIGRA_LIBRARY} ${Tbb_LIBRARY} )

ADD_EXECUTABLE(filterSingleMap filterSingleMap.cpp)
TARGET_LINK_LIBRARIES(filterSingleMap ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs}  tbb )

ADD_EXECUTABLE(findMF findMF.cpp)
TARGET_LINK_LIBRARIES(findMF ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs}   tbb )

ADD_EXECUTABLE(PeakPicker PeakPicker.cpp)
TARGET_LINK_LIBRARIES(PeakPicker ${Boost_LIBRARIES} pwiz  )

ADD_EXECUTABLE(conv2Image conv2Image.cpp)
TARGET_LINK_LIBRARIES(conv2Image  ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz  ${vigralibs} tbb)
