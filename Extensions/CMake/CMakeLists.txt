
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16.3...3.19.7 FATAL_ERROR)
#-----------------------------------------------------------------------------

if(Cjyx_SOURCE_DIR)
  message(FATAL_ERROR "Building extensions within Cjyx is not supported any more. Instead consider
building this project specifying -DCjyx_DIR:PATH=/path/to/Cjyx-SuperBuild/Cjyx-build.")
endif()

if(APPLE)
  # Note: By setting CMAKE_OSX_* variables before any enable_language() or project() calls,
  #       we ensure that the bitness will be properly detected.
  include(${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/CjyxInitializeOSXVariables.cmake)
endif()

project(CjyxExtensions)

#-----------------------------------------------------------------------------
set(CJYX_PACKAGE_MANAGER_URL $ENV{CJYX_PACKAGE_MANAGER_URL})
if("${CJYX_PACKAGE_MANAGER_URL}" STREQUAL "")
  set(CJYX_PACKAGE_MANAGER_URL "https://slicer-packages.kitware.com")
endif()

#-----------------------------------------------------------------------------
# Compute default values
#-----------------------------------------------------------------------------
get_filename_component(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} REALPATH) # Resolved symlinks if any
set(default_extension_description_dir ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(default_local_extension_dir ${default_extension_description_dir})

set(Cjyx_EXTENSIONS_TRACK_QUALIFIER "master" CACHE STRING "CDash extensions track qualifier (master, 4.4, ...)")

#-----------------------------------------------------------------------------
# Prerequisites
#-----------------------------------------------------------------------------
include(ExternalProject)
find_package(Cjyx REQUIRED)
find_package(Git REQUIRED)
find_package(Subversion)

#-----------------------------------------------------------------------------
# Set a default build type if none was specified
#-----------------------------------------------------------------------------
include(CjyxInitializeBuildType)

#-----------------------------------------------------------------------------
# Options
#-----------------------------------------------------------------------------
set(Cjyx_BUILD_EXTENSIONS ON CACHE INTERNAL "Build Cjyx extensions.")
set(Cjyx_EXTENSION_DESCRIPTION_DIR "${default_extension_description_dir}" CACHE PATH "Path to folder containing *.s4ext files to consider.")
set(Cjyx_LOCAL_EXTENSIONS_DIR "${default_local_extension_dir}" CACHE STRING "Path to extension sources locally available")
option(BUILD_TESTING "Test extensions." ${Cjyx_BUILD_TESTING})

option(Cjyx_UPLOAD_EXTENSIONS "Build, test, package and upload extensions" OFF)
set(CTEST_DROP_SITE "cjyx.cdash.org" CACHE STRING "Dashboard results are uploaded to this URL.")
set(CDASH_PROJECT_NAME "CjyxPreview" CACHE STRING "Name of the dashboard project.")

configure_file(
  CTestConfig.cmake.in
  ${CMAKE_BINARY_DIR}/CTestConfig.cmake
  )

include(CjyxBlockBuildPackageAndUploadExtensions)
