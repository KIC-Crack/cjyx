
#
# This module will set the variables Cjyx_VERSION and Cjyx_VERSION_FULL.
#
# It will also set all variables describing the SCM associated
# with <Cjyx_MAIN_PROJECT_APPLICATION_NAME>_SOURCE_DIR.
#
# It has been designed to be included in the build system of Cjyx.
#
# The following variables are expected to be defined in the including scope:
#  GIT_EXECUTABLE
#  Cjyx_CMAKE_DIR
#  Cjyx_MAIN_PROJECT_APPLICATION_NAME
#  <Cjyx_MAIN_PROJECT_APPLICATION_NAME>_SOURCE_DIR
#  Cjyx_RELEASE_TYPE
#  Cjyx_VERSION_MAJOR
#  Cjyx_VERSION_MINOR
#  Cjyx_VERSION_PATCH
#

# --------------------------------------------------------------------------
# Sanity checks
# --------------------------------------------------------------------------
set(expected_defined_vars
  GIT_EXECUTABLE
  Cjyx_CMAKE_DIR
  Cjyx_MAIN_PROJECT_APPLICATION_NAME
  ${Cjyx_MAIN_PROJECT_APPLICATION_NAME}_SOURCE_DIR
  Cjyx_RELEASE_TYPE
  Cjyx_VERSION_MAJOR
  Cjyx_VERSION_MINOR
  Cjyx_VERSION_PATCH
  )
foreach(var ${expected_defined_vars})
  if(NOT DEFINED ${var})
    message(FATAL_ERROR "${var} is mandatory")
  endif()
endforeach()

message(STATUS "Configuring ${Cjyx_MAIN_PROJECT_APPLICATION_NAME} release type [${Cjyx_RELEASE_TYPE}]")

#-----------------------------------------------------------------------------
# Update CMake module path
#-----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  ${Cjyx_CMAKE_DIR}
  ${CMAKE_MODULE_PATH}
  )

include(CjyxMacroExtractRepositoryInfo)

#-----------------------------------------------------------------------------
# Cjyx version number
#-----------------------------------------------------------------------------
CjyxMacroExtractRepositoryInfo(
  VAR_PREFIX Cjyx
  SOURCE_DIR ${Cjyx_SOURCE_DIR}
  )

if(NOT "${Cjyx_FORCED_WC_LAST_CHANGED_DATE}" STREQUAL "")
  set(Cjyx_WC_LAST_CHANGED_DATE "${Cjyx_FORCED_WC_LAST_CHANGED_DATE}")
endif()
string(REGEX REPLACE ".*([0-9][0-9][0-9][0-9]\\-[0-9][0-9]\\-[0-9][0-9]).*" "\\1"
  Cjyx_BUILDDATE "${Cjyx_WC_LAST_CHANGED_DATE}")

# Set Cjyx_COMMIT_COUNT from working copy commit count adjusted by a custom offset.
if("${Cjyx_WC_COMMIT_COUNT_OFFSET}" STREQUAL "")
  set(Cjyx_WC_COMMIT_COUNT_OFFSET "0")
endif()
math(EXPR Cjyx_COMMIT_COUNT "${Cjyx_WC_COMMIT_COUNT}+${Cjyx_WC_COMMIT_COUNT_OFFSET}")

if("${Cjyx_REVISION_TYPE}" STREQUAL "")
  set(Cjyx_REVISION_TYPE "CommitCount")
endif()

if(NOT "${Cjyx_FORCED_REVISION}" STREQUAL "")
  set(Cjyx_REVISION "${Cjyx_FORCED_REVISION}")
elseif(Cjyx_REVISION_TYPE STREQUAL "CommitCount")
  set(Cjyx_REVISION "${Cjyx_COMMIT_COUNT}")
elseif(Cjyx_REVISION_TYPE STREQUAL "Hash")
  set(Cjyx_REVISION "${Cjyx_WC_REVISION_HASH}")
else()
  message(FATAL_ERROR "Invalid Cjyx_REVISION_TYPE value: ${Cjyx_REVISION_TYPE}")
endif()

set(Cjyx_VERSION      "${Cjyx_VERSION_MAJOR}.${Cjyx_VERSION_MINOR}")
set(Cjyx_VERSION_FULL "${Cjyx_VERSION}.${Cjyx_VERSION_PATCH}")

if(NOT "${Cjyx_RELEASE_TYPE}" STREQUAL "Stable")
  set(Cjyx_VERSION_FULL "${Cjyx_VERSION_FULL}-${Cjyx_BUILDDATE}")
endif()

message(STATUS "Configuring Cjyx version [${Cjyx_VERSION_FULL}]")
message(STATUS "Configuring Cjyx revision [${Cjyx_REVISION}]")

#-----------------------------------------------------------------------------
# Cjyx main application version number
#-----------------------------------------------------------------------------
CjyxMacroExtractRepositoryInfo(
  VAR_PREFIX Cjyx_MAIN_PROJECT
  SOURCE_DIR ${${Cjyx_MAIN_PROJECT_APPLICATION_NAME}_SOURCE_DIR}
  )

if(NOT "${Cjyx_MAIN_PROJECT_FORCED_WC_LAST_CHANGED_DATE}" STREQUAL "")
  set(Cjyx_MAIN_PROJECT_WC_LAST_CHANGED_DATE "${Cjyx_MAIN_PROJECT_FORCED_WC_LAST_CHANGED_DATE}")
endif()
string(REGEX REPLACE ".*([0-9][0-9][0-9][0-9]\\-[0-9][0-9]\\-[0-9][0-9]).*" "\\1"
  Cjyx_MAIN_PROJECT_BUILDDATE "${Cjyx_MAIN_PROJECT_WC_LAST_CHANGED_DATE}")

# Set Cjyx_MAIN_PROJECT_COMMIT_COUNT from working copy commit count adjusted by a custom offset.
if("${Cjyx_MAIN_PROJECT_WC_COMMIT_COUNT_OFFSET}" STREQUAL "")
  if ("${Cjyx_MAIN_PROJECT_APPLICATION_NAME}" STREQUAL "Cjyx")
    # Force CjyxApp's commit count offset to be the same as Cjyx_WC_COMMIT_COUNT_OFFSET to make
    # Cjyx_MAIN_PROJECT_REVISION the same as Cjyx_REVISION if the default Cjyx application is built.
    set(Cjyx_MAIN_PROJECT_WC_COMMIT_COUNT_OFFSET "${Cjyx_WC_COMMIT_COUNT_OFFSET}")
  else()
    set(Cjyx_MAIN_PROJECT_WC_COMMIT_COUNT_OFFSET "0")
  endif()
endif()
math(EXPR Cjyx_MAIN_PROJECT_COMMIT_COUNT "${Cjyx_MAIN_PROJECT_WC_COMMIT_COUNT}+${Cjyx_MAIN_PROJECT_WC_COMMIT_COUNT_OFFSET}")

if("${Cjyx_MAIN_PROJECT_REVISION_TYPE}" STREQUAL "")
  if ("${Cjyx_MAIN_PROJECT_APPLICATION_NAME}" STREQUAL "Cjyx")
    # Force CjyxApp's revision type to be the same as Cjyx_REVISION_TYPE to make
    # Cjyx_MAIN_PROJECT_REVISION the same as Cjyx_REVISION if the default Cjyx application is built.
    set(Cjyx_MAIN_PROJECT_REVISION_TYPE "${Cjyx_REVISION_TYPE}")
  else()
    set(Cjyx_MAIN_PROJECT_REVISION_TYPE "CommitCount")
  endif()
endif()

if(NOT "${Cjyx_MAIN_PROJECT_FORCED_REVISION}" STREQUAL "")
  set(Cjyx_MAIN_PROJECT_REVISION "${Cjyx_FORCED_REVISION}")
elseif(Cjyx_MAIN_PROJECT_REVISION_TYPE STREQUAL "CommitCount")
  set(Cjyx_MAIN_PROJECT_REVISION "${Cjyx_MAIN_PROJECT_COMMIT_COUNT}")
elseif(Cjyx_MAIN_PROJECT_REVISION_TYPE STREQUAL "Hash")
  set(Cjyx_MAIN_PROJECT_REVISION "${Cjyx_MAIN_PROJECT_WC_REVISION_HASH}")
else()
  message(FATAL_ERROR "Invalid Cjyx_MAIN_PROJECT_REVISION_TYPE value: ${Cjyx_MAIN_PROJECT_REVISION_TYPE}")
endif()

set(Cjyx_MAIN_PROJECT_VERSION      "${Cjyx_MAIN_PROJECT_VERSION_MAJOR}.${Cjyx_MAIN_PROJECT_VERSION_MINOR}")
set(Cjyx_MAIN_PROJECT_VERSION_FULL "${Cjyx_MAIN_PROJECT_VERSION}.${Cjyx_MAIN_PROJECT_VERSION_PATCH}")

if(NOT "${Cjyx_RELEASE_TYPE}" STREQUAL "Stable")
  set(Cjyx_MAIN_PROJECT_VERSION_FULL "${Cjyx_MAIN_PROJECT_VERSION_FULL}-${Cjyx_MAIN_PROJECT_BUILDDATE}")
endif()

if(NOT "${Cjyx_MAIN_PROJECT_APPLICATION_NAME}" STREQUAL "Cjyx")
  message(STATUS "Configuring ${Cjyx_MAIN_PROJECT_APPLICATION_NAME} version [${Cjyx_MAIN_PROJECT_VERSION_FULL}]")
  message(STATUS "Configuring ${Cjyx_MAIN_PROJECT_APPLICATION_NAME} revision [${Cjyx_MAIN_PROJECT_REVISION}]")
endif()
