# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

include_directories(${inc})
link_directories( ${Pwiz_LIBRARY} ${VIGRA_LIBRARY} ${Tbb_LIBRARY})

SET(featurefind_SOURCES filterQTView.cpp)

ADD_EXECUTABLE(filterQTView ${featurefind_SOURCES})
TARGET_LINK_LIBRARIES(filterQTView ${QT_LIBRARIES} ${Boost_LIBRARIES} pwiz ${vigralibs} widgets)

ADD_EXECUTABLE(testWidget testWidget.cpp)
TARGET_LINK_LIBRARIES(testWidget ${QT_LIBRARIES} ${Boost_LIBRARIES} widgets)

