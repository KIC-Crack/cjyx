cmake_minimum_required(VERSION 3.16.3...3.19.7 FATAL_ERROR)
# Two possible approaches to use this script:
# (1) Copy and adapt to your specific configuration or (2) Use as it is by passing options
# Either way, the script can be executed using ctest:
#  ctest [-DOPTION:TYPE=<value> [...]] -S /path/to/this/script.cmake [-C <CTEST_BUILD_CONFIGURATION>] [-V]
# Note that '-C <CTEST_BUILD_CONFIGURATION>' is mandatory on windows
macro(dashboard_set var value)
  if(NOT DEFINED "${var}")
    set(${var} "${value}")
  endif()
endmacro()

dashboard_set(DASHBOARDS_DIR        "$ENV{HOME}/Dashboards/")
dashboard_set(ORGANIZATION          "Kitware")        # One word, no ponctuation
dashboard_set(HOSTNAME              "karakoram")
dashboard_set(OPERATING_SYSTEM      "Linux")
dashboard_set(SCRIPT_MODE           "Experimental")   # Experimental, Continuous or Nightly
dashboard_set(Cjyx_RELEASE_TYPE   "Experimental")   # (E)xperimental, (P)review or (S)table
dashboard_set(EXTENSIONS_INDEX_BRANCH "master")       # "master", X.Y, ...
if(APPLE)
  dashboard_set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13")
endif()
dashboard_set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
dashboard_set(COMPILER              "g++-X.Y.Z")      # Used only to set the build name
dashboard_set(CTEST_BUILD_FLAGS     "")               # Use multiple CPU cores to build. For example "-j -l4" on unix
# By default, CMake auto-discovers the compilers
#dashboard_set(CMAKE_C_COMPILER      "/path/to/c/compiler")
#dashboard_set(CMAKE_CXX_COMPILER    "/path/to/cxx/compiler")
dashboard_set(CTEST_BUILD_CONFIGURATION "Release")
dashboard_set(EXTENSIONS_BUILDSYSTEM_TESTING FALSE)   # If enabled, build <Cjyx_SOURCE_DIR>/Extensions/*.s4ext

dashboard_set(QT_VERSION            "5.15.0")         # Used only to set the build name

#   Cjyx_SOURCE_DIR: <DASHBOARDS_DIR>/<Cjyx_DASHBOARD_SUBDIR>/<Cjyx_DIRECTORY_BASENAME>-<Cjyx_DIRECTORY_IDENTIFIER>
#   Cjyx_DIR       : <DASHBOARDS_DIR>/<Cjyx_DASHBOARD_SUBDIR>/<Cjyx_DIRECTORY_BASENAME>-<Cjyx_DIRECTORY_IDENTIFIER>-build
dashboard_set(Cjyx_DIRECTORY_BASENAME   "Cjyx")
dashboard_set(Cjyx_DASHBOARD_SUBDIR     "${Cjyx_RELEASE_TYPE}")
dashboard_set(Cjyx_DIRECTORY_IDENTIFIER "0")        # Set to arbitrary integer to distinguish different Experimental/Preview release build
                                                      # Set to Cjyx version XYZ for Stable release build
dashboard_set(Cjyx_SOURCE_DIR "${DASHBOARDS_DIR}/${Cjyx_DASHBOARD_SUBDIR}/${Cjyx_DIRECTORY_BASENAME}-${Cjyx_DIRECTORY_IDENTIFIER}")
dashboard_set(Cjyx_DIR        "${DASHBOARDS_DIR}/${Cjyx_DASHBOARD_SUBDIR}/${Cjyx_DIRECTORY_BASENAME}-${Cjyx_DIRECTORY_IDENTIFIER}-build/Cjyx-build")

# CTEST_SOURCE_DIRECTORY: <Cjyx_SOURCE_DIR>/Extensions/CMake
# CTEST_BINARY_DIRECTORY: <DASHBOARDS_DIR>/<EXTENSION_DASHBOARD_SUBDIR>/<EXTENSION_DIRECTORY_BASENAME>-<Cjyx_DIRECTORY_IDENTIFIER>-E[-T]-b
dashboard_set(EXTENSION_DASHBOARD_SUBDIR   "${Cjyx_RELEASE_TYPE}")
dashboard_set(EXTENSION_DIRECTORY_BASENAME "S")

dashboard_set(EXTENSIONS_INDEX_GIT_TAG        "origin/${EXTENSIONS_INDEX_BRANCH}") # origin/master, origin/X.Y, ...
dashboard_set(EXTENSIONS_INDEX_GIT_REPOSITORY "https://github.com/Slicer/ExtensionsIndex.git")

# Build Name: <OPERATING_SYSTEM>-<COMPILER>-<BITNESS>bits-Qt<QT_VERSION>[-<BUILD_NAME_SUFFIX>]-<CTEST_BUILD_CONFIGURATION
set(BUILD_NAME_SUFFIX "")

set(ADDITIONAL_CMAKECACHE_OPTION "
")

##########################################
# WARNING: DO NOT EDIT BEYOND THIS POINT #
##########################################
set(EXTENSIONS_TRACK_QUALIFIER ${EXTENSIONS_INDEX_BRANCH})
if(NOT DEFINED DRIVER_SCRIPT)
  set(url https://raw.githubusercontent.com/Slicer/Slicer/master/Extensions/CMake/SlicerExtensionsDashboardDriverScript.cmake)
  set(dest ${DASHBOARDS_DIR}/${EXTENSION_DASHBOARD_SUBDIR}/${CTEST_SCRIPT_NAME}.driver)
  file(DOWNLOAD ${url} ${dest} STATUS status)
  if(NOT status MATCHES "0.*")
    message(FATAL_ERROR "error: Failed to download ${url} - ${status}")
  endif()
  set(DRIVER_SCRIPT ${dest})
endif()
include(${DRIVER_SCRIPT})
