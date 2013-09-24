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
