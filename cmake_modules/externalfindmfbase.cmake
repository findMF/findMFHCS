# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)
# Add findMFBase
ExternalProject_Add(
    findMFBase
    GIT_REPOSITORY git@github.com:wolski/findMFBase.git
    #SVN_REVISION -r 664
    TIMEOUT 10
    #PATCH_COMMAND svn patch ${CMAKE_SOURCE_DIR}/findMFBase.patch ${CMAKE_BINARY_DIR}/ThirdParty/src/findMFBase
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
               -DENABLE_TESTING=OFF
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

# Specify include dir
ExternalProject_Get_Property(findMFBase source_dir)
set(findMFBase_SourceDir ${source_dir}/include)


ExternalProject_Get_Property(findMFBase binary_dir)
IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(findMFBase_LIBRARY ${binary_dir}/DebugLibs)
ELSE(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(findMFBase_LIBRARY ${binary_dir}/ReleaseLibs)
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

