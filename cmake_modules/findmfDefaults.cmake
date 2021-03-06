# Copyright : ETH Zurich
# License   : three-clause BSD license
# Authors   : Witold Wolski
# for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

IF(NOT DEFINED WITH_TEST)
    SET(WITH_TEST "ON")
ENDIF()
SET(WITH_TEST ${WITH_TEST}
    CACHE BOOL "Build with test ?"
    FORCE)

IF(NOT DEFINED WITH_QT)
    SET(WITH_QT "OFF")
ENDIF()
SET(WITH_QT ${WITH_QT}
    CACHE BOOL "Build with qt ?"
    FORCE)


IF(NOT DEFINED WITH_QT)
    SET(WITH_DOX "OFF")
ENDIF()
SET(WITH_DOX ${WITH_DOX}
    CACHE BOOL "Build with doxygen ?"
    FORCE)
