# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)
# Add VIGRA
ExternalProject_Add(
    vigra
    GIT_REPOSITORY https://github.com/ukoethe/vigra.git
    #SVN_REVISION -r 664
    TIMEOUT 10
    #PATCH_COMMAND svn patch ${CMAKE_SOURCE_DIR}/VIGRA.patch ${CMAKE_BINARY_DIR}/ThirdParty/src/vigra
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
               -DWITH_HDF5=OFF
               -DWITH_OPENEXR=OFF
               -DWITH_VIGRANUMPY=OFF
               -DWITH_VALGRIND=OFF
               -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    # Disable install step
    INSTALL_COMMAND "" # ${CMAKE_BUILD_TOOL} install
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

# Specify include dir
ExternalProject_Get_Property(vigra source_dir)
set(VIGRA_SourceDir ${source_dir}/include)


ExternalProject_Get_Property(vigra binary_dir)
IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(VIGRA_LIBRARY ${binary_dir}/src/impex/)
ELSE(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(VIGRA_LIBRARY ${binary_dir}/src/impex/)
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

