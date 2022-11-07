################################################################################
#
#  Program: 3D Cjyx
#
#  Copyright (c) Kitware Inc.
#
#  See COPYRIGHT.txt
#  or http://www.slicer.org/copyright/copyright.txt for details.
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
#  and was partially funded by NIH grant 3P41RR013218-12S1
#
################################################################################

#
# This module will add a target named 'CjyxConfigureVersionHeader'.
#
# It has been designed to be included in the build system of Cjyx.
#
# The following variables are expected to be defined in the including scope:
#
#  GIT_EXECUTABLE
#  Cjyx_BINARY_DIR
#  Cjyx_CMAKE_DIR
#
#  Cjyx_MAIN_PROJECT_APPLICATION_NAME
#  <Cjyx_MAIN_PROJECT_APPLICATION_NAME>_SOURCE_DIR
#  Cjyx_MAIN_PROJECT_VERSION_MAJOR
#  Cjyx_MAIN_PROJECT_VERSION_MINOR
#  Cjyx_MAIN_PROJECT_VERSION_PATCH
#
#  Cjyx_RELEASE_TYPE
#
#  Cjyx_SOURCE_DIR
#  Cjyx_VERSION_MAJOR
#  Cjyx_VERSION_MINOR
#  Cjyx_VERSION_PATCH
#
# Optionally, these variable can also be set:
#
#  Cjyx_FORCED_REVISION (default "")
#  Cjyx_REVISION_TYPE
#  Cjyx_FORCED_WC_LAST_CHANGED_DATE (default ""): Format YYYY-MM-DD
#  Cjyx_WC_COMMIT_COUNT_OFFSET
#
#  Cjyx_MAIN_PROJECT_FORCED_REVISION (default "")
#  Cjyx_MAIN_PROJECT_REVISION_TYPE
#  Cjyx_MAIN_PROJECT_FORCED_WC_LAST_CHANGED_DATE (default ""): Format YYYY-MM-DD
#  Cjyx_MAIN_PROJECT_WC_COMMIT_COUNT_OFFSET
#

# --------------------------------------------------------------------------
# Sanity checks
# --------------------------------------------------------------------------
set(expected_defined_vars
  GIT_EXECUTABLE
  Cjyx_BINARY_DIR
  Cjyx_CMAKE_DIR

  Cjyx_MAIN_PROJECT_APPLICATION_NAME # Used by CjyxVersion.cmake
  ${Cjyx_MAIN_PROJECT_APPLICATION_NAME}_SOURCE_DIR
  Cjyx_MAIN_PROJECT_VERSION_MAJOR
  Cjyx_MAIN_PROJECT_VERSION_MINOR
  Cjyx_MAIN_PROJECT_VERSION_PATCH

  Cjyx_RELEASE_TYPE

  Cjyx_SOURCE_DIR
  Cjyx_VERSION_MAJOR
  Cjyx_VERSION_MINOR
  Cjyx_VERSION_PATCH
  )
foreach(var ${expected_defined_vars})
  if(NOT DEFINED ${var})
    message(FATAL_ERROR "${var} is mandatory")
  endif()
endforeach()

if(NOT DEFINED CJYX_CONFIGURE_VERSION_HEADER)
  set(CJYX_CONFIGURE_VERSION_HEADER 0)
endif()

# --------------------------------------------------------------------------
# Add CjyxConfigureVersionHeader target
# --------------------------------------------------------------------------
if(NOT CJYX_CONFIGURE_VERSION_HEADER)
  set(script_args)
  foreach(var IN LISTS expected_defined_vars)
    list(APPEND script_args "-D${var}:STRING=${${var}}")
  endforeach()
  if(NOT DEFINED Cjyx_FORCED_REVISION)
    set(Cjyx_FORCED_REVISION "")
  endif()
  if(NOT DEFINED Cjyx_FORCED_WC_LAST_CHANGED_DATE)
    set(Cjyx_FORCED_WC_LAST_CHANGED_DATE "")
  endif()
  add_custom_target(CjyxConfigureVersionHeader ALL
    COMMAND ${CMAKE_COMMAND}
      ${script_args}
      -DCjyx_FORCED_REVISION:STRING=${Cjyx_FORCED_REVISION}
      -DCjyx_REVISION_TYPE:STRING=${Cjyx_REVISION_TYPE}
      -DCjyx_FORCED_WC_LAST_CHANGED_DATE:STRING=${Cjyx_FORCED_WC_LAST_CHANGED_DATE}
      -DCjyx_WC_COMMIT_COUNT_OFFSET:STRING=${Cjyx_WC_COMMIT_COUNT_OFFSET}

      -DCjyx_MAIN_PROJECT_FORCED_REVISION:STRING=${Cjyx_MAIN_PROJECT_FORCED_REVISION}
      -DCjyx_MAIN_PROJECT_REVISION_TYPE:STRING=${Cjyx_MAIN_PROJECT_REVISION_TYPE}
      -DCjyx_MAIN_PROJECT_FORCED_WC_LAST_CHANGED_DATE:STRING=${Cjyx_MAIN_PROJECT_FORCED_WC_LAST_CHANGED_DATE}
      -DCjyx_MAIN_PROJECT_WC_COMMIT_COUNT_OFFSET:STRING=${Cjyx_MAIN_PROJECT_WC_COMMIT_COUNT_OFFSET}

      -DCJYX_CONFIGURE_VERSION_HEADER=1
      -P ${CMAKE_CURRENT_LIST_FILE}
    COMMENT "Configuring vtkCjyxVersionConfigure.h"
    )
  return()
endif()

# --------------------------------------------------------------------------
# Configure header
# --------------------------------------------------------------------------

include(${Cjyx_CMAKE_DIR}/CjyxVersion.cmake)

# Variables expected to be set by 'CjyxVersion' module.
set(expected_defined_vars
  Cjyx_BUILDDATE
  Cjyx_VERSION
  Cjyx_VERSION_FULL
  Cjyx_REVISION
  Cjyx_WC_REVISION
  Cjyx_WC_REVISION_HASH
  Cjyx_WC_URL

  Cjyx_MAIN_PROJECT_BUILDDATE
  Cjyx_MAIN_PROJECT_VERSION
  Cjyx_MAIN_PROJECT_VERSION_FULL
  Cjyx_MAIN_PROJECT_REVISION
  Cjyx_MAIN_PROJECT_WC_REVISION
  Cjyx_MAIN_PROJECT_WC_REVISION_HASH
  Cjyx_MAIN_PROJECT_WC_URL
  )
foreach(var ${expected_defined_vars})
  if(NOT DEFINED ${var})
    message(FATAL_ERROR "${var} is mandatory")
  endif()
endforeach()

configure_file(
  ${Cjyx_SOURCE_DIR}/CMake/vtkCjyxVersionConfigure.h.in
  ${Cjyx_BINARY_DIR}/vtkCjyxVersionConfigure.h
  )
