#moc headers are going here
SET(widget_HEADERS renderthread.h mandelbrotwidget.h)
QT4_WRAP_CPP(transition_HEADERS_MOC ${widget_HEADERS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

SET(widget_SOURCES mandelbrotwidget.cpp renderthread.cpp)
include_directories(${inc} .)
add_library(widgets ${widget_SOURCES} ${transition_HEADERS_MOC})
target_link_libraries(widgets ${QT_LIBRARIES})
