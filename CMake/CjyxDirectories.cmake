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
#  and was partially funded by NIH grant 1U24CA194354-01
#
################################################################################

#
#  W A R N I N G
#  -------------
#
# This module was designed to be included in (1) the Cjyx top-level CMakeLists.txt
# and (2) a CjyxCustomApp CMakeLists.txt to conveniently set the value of the common
# Cjyx relative directories for build and install tree.
#
# Using this module outside of these contexts may break from version to version without notice.
#
# We mean it.
#


#-----------------------------------------------------------------------------
# Sanity checks
#-----------------------------------------------------------------------------
set(expected_defined_vars
  Cjyx_MAIN_PROJECT
  ${Cjyx_MAIN_PROJECT}_APPLICATION_NAME
  Cjyx_VERSION_MAJOR
  Cjyx_VERSION_MINOR
  )
foreach(var IN LISTS expected_defined_vars)
  if(NOT DEFINED ${var})
    message(FATAL_ERROR "Variable ${var} is expected to be defined !")
  endif()
endforeach()

if(NOT DEFINED Cjyx_MAIN_PROJECT_APPLICATION_NAME)
  set(Cjyx_MAIN_PROJECT_APPLICATION_NAME ${${Cjyx_MAIN_PROJECT}_APPLICATION_NAME})
endif()

#-----------------------------------------------------------------------------
# Cjyx relative directories
#-----------------------------------------------------------------------------
# for build tree
set(Cjyx_BIN_DIR "bin")
set(Cjyx_LIB_DIR "lib/${Cjyx_MAIN_PROJECT_APPLICATION_NAME}-${Cjyx_VERSION_MAJOR}.${Cjyx_VERSION_MINOR}")
set(Cjyx_INCLUDE_DIR "include/${Cjyx_MAIN_PROJECT_APPLICATION_NAME}-${Cjyx_VERSION_MAJOR}.${Cjyx_VERSION_MINOR}")
set(Cjyx_SHARE_DIR "share/${Cjyx_MAIN_PROJECT_APPLICATION_NAME}-${Cjyx_VERSION_MAJOR}.${Cjyx_VERSION_MINOR}")
set(Cjyx_LIBEXEC_DIR "libexec/${Cjyx_MAIN_PROJECT_APPLICATION_NAME}-${Cjyx_VERSION_MAJOR}.${Cjyx_VERSION_MINOR}")
set(Cjyx_ITKFACTORIES_DIR "${Cjyx_LIB_DIR}/ITKFactories")
set(Cjyx_QM_DIR "${Cjyx_BIN_DIR}/translations")

# for install tree
set(Cjyx_INSTALL_ROOT "./")
set(Cjyx_BUNDLE_LOCATION "${Cjyx_MAIN_PROJECT_APPLICATION_NAME}.app/Contents")
if(APPLE)
  set(Cjyx_INSTALL_ROOT "${Cjyx_BUNDLE_LOCATION}/") # Set to create Bundle
endif()
set(Cjyx_INSTALL_BIN_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_BIN_DIR}")
set(Cjyx_INSTALL_LIB_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_LIB_DIR}")
set(Cjyx_INSTALL_INCLUDE_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_INCLUDE_DIR}")
set(Cjyx_INSTALL_SHARE_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_SHARE_DIR}")
set(Cjyx_INSTALL_LIBEXEC_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_LIBEXEC_DIR}")
set(Cjyx_INSTALL_ITKFACTORIES_DIR "${Cjyx_INSTALL_LIB_DIR}/ITKFactories")
set(Cjyx_INSTALL_QM_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QM_DIR}")


#-----------------------------------------------------------------------------
# Cjyx CLI relative directories
#-----------------------------------------------------------------------------

# NOTE: Make sure to update vtkCjyxApplicationLogic::GetModuleShareDirectory()
#       if the following variables are changed.

set(Cjyx_CLIMODULES_SUBDIR "cli-modules")

# for build tree
set(Cjyx_CLIMODULES_BIN_DIR "${Cjyx_LIB_DIR}/${Cjyx_CLIMODULES_SUBDIR}")
set(Cjyx_CLIMODULES_LIB_DIR "${Cjyx_LIB_DIR}/${Cjyx_CLIMODULES_SUBDIR}")
set(Cjyx_CLIMODULES_SHARE_DIR "${Cjyx_SHARE_DIR}/${Cjyx_CLIMODULES_SUBDIR}")

# for install tree
set(Cjyx_INSTALL_CLIMODULES_BIN_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_CLIMODULES_BIN_DIR}")
set(Cjyx_INSTALL_CLIMODULES_LIB_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_CLIMODULES_LIB_DIR}")
set(Cjyx_INSTALL_CLIMODULES_SHARE_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_CLIMODULES_SHARE_DIR}")


#-----------------------------------------------------------------------------
# Qt Loadable Modules relative directories
#-----------------------------------------------------------------------------

# NOTE: Make sure to update vtkCjyxApplicationLogic::GetModuleShareDirectory()
#       if the following variables are changed.

set(Cjyx_QTLOADABLEMODULES_SUBDIR "qt-loadable-modules")

# for build tree
set(Cjyx_QTLOADABLEMODULES_BIN_DIR "${Cjyx_LIB_DIR}/${Cjyx_QTLOADABLEMODULES_SUBDIR}")
set(Cjyx_QTLOADABLEMODULES_LIB_DIR "${Cjyx_LIB_DIR}/${Cjyx_QTLOADABLEMODULES_SUBDIR}")
set(Cjyx_QTLOADABLEMODULES_PYTHON_LIB_DIR "${Cjyx_LIB_DIR}/${Cjyx_QTLOADABLEMODULES_SUBDIR}/Python")
set(Cjyx_QTLOADABLEMODULES_INCLUDE_DIR "${Cjyx_INCLUDE_DIR}/${Cjyx_QTLOADABLEMODULES_SUBDIR}")
set(Cjyx_QTLOADABLEMODULES_SHARE_DIR "${Cjyx_SHARE_DIR}/${Cjyx_QTLOADABLEMODULES_SUBDIR}")

# for install tree
set(Cjyx_INSTALL_QTLOADABLEMODULES_BIN_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QTLOADABLEMODULES_BIN_DIR}")
set(Cjyx_INSTALL_QTLOADABLEMODULES_LIB_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QTLOADABLEMODULES_LIB_DIR}")
set(Cjyx_INSTALL_QTLOADABLEMODULES_PYTHON_LIB_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QTLOADABLEMODULES_PYTHON_LIB_DIR}")
set(Cjyx_INSTALL_QTLOADABLEMODULES_INCLUDE_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QTLOADABLEMODULES_INCLUDE_DIR}")
set(Cjyx_INSTALL_QTLOADABLEMODULES_SHARE_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QTLOADABLEMODULES_SHARE_DIR}")


#-----------------------------------------------------------------------------
# Scripted Modules relative directories
#-----------------------------------------------------------------------------

# NOTE: Make sure to update vtkCjyxApplicationLogic::GetModuleShareDirectory()
#       if the following variables are changed.

set(Cjyx_QTSCRIPTEDMODULES_SUBDIR "qt-scripted-modules")

# for build tree
set(Cjyx_QTSCRIPTEDMODULES_BIN_DIR "${Cjyx_LIB_DIR}/${Cjyx_QTSCRIPTEDMODULES_SUBDIR}")
set(Cjyx_QTSCRIPTEDMODULES_LIB_DIR "${Cjyx_LIB_DIR}/${Cjyx_QTSCRIPTEDMODULES_SUBDIR}")
set(Cjyx_QTSCRIPTEDMODULES_INCLUDE_DIR "${Cjyx_INCLUDE_DIR}/${Cjyx_QTSCRIPTEDMODULES_SUBDIR}")
set(Cjyx_QTSCRIPTEDMODULES_SHARE_DIR "${Cjyx_SHARE_DIR}/${Cjyx_QTSCRIPTEDMODULES_SUBDIR}")

# for install tree
set(Cjyx_INSTALL_QTSCRIPTEDMODULES_BIN_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QTSCRIPTEDMODULES_BIN_DIR}")
set(Cjyx_INSTALL_QTSCRIPTEDMODULES_LIB_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QTSCRIPTEDMODULES_LIB_DIR}")
set(Cjyx_INSTALL_QTSCRIPTEDMODULES_INCLUDE_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QTSCRIPTEDMODULES_INCLUDE_DIR}")
set(Cjyx_INSTALL_QTSCRIPTEDMODULES_SHARE_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QTSCRIPTEDMODULES_SHARE_DIR}")


# --------------------------------------------------------------------------
# ThirdParty: Used to superbuild projects built in Cjyx extension.
# --------------------------------------------------------------------------

# for build tree
set(Cjyx_THIRDPARTY_BIN_DIR ${Cjyx_BIN_DIR})
set(Cjyx_THIRDPARTY_LIB_DIR ${Cjyx_LIB_DIR})
set(Cjyx_THIRDPARTY_SHARE_DIR ${Cjyx_SHARE_DIR})

# for install tree:
#
# These variables can be used when configuring extension external projects in
# two different scenarios: (1) bundled extensions and (2) regular extensions.
#
# The values set below corresponds to scenario (1). Value for scenario (2) are set
# in UseCjyx.cmake.
set(Cjyx_INSTALL_THIRDPARTY_BIN_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_THIRDPARTY_BIN_DIR}")
set(Cjyx_INSTALL_THIRDPARTY_LIB_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_THIRDPARTY_LIB_DIR}")
set(Cjyx_INSTALL_THIRDPARTY_SHARE_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_THIRDPARTY_SHARE_DIR}")
