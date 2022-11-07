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
#  and was partially funded by NIH grant 1R01EB021391
#
################################################################################

#
# This CMake module provides functions allowing to check if a given Cjyx built-in module
# should be enabled or disabled.
#
# Definitions:
#
# * A Cjyx built-in module is a module whose source code is found under one of the
#   "Slilcer/Modules/*" subdirectories. These do not include Cjyx remote modules or
#   modules associated with bundled extensions.
#
# * Enabling (or disabling) a module means that it will (or will not) be built.
#
# * Building a module means that the corresponding directory is added using the add_subdirectory
#   CMake command and that the associated CMakeLists.txt is processed.
#
# By default, all built-in Cjyx modules are assumed to be enabled. Then, based on Cjyx
# build options, built-in module may be disabled.
#
# Setting any of these variables allows to explicitly disable modules of the
# corresponding module type ignoring effect of Cjyx build options:
# * Cjyx_CLIMODULES_DISABLED
# * Cjyx_QTLOADABLEMODULES_DISABLED
# * Cjyx_QTSCRIPTEDMODULES_DISABLED
#
# Setting any of these variables allows to explicitly list the modules
# to be built ignoring effect of Cjyx build options:
# * Cjyx_CLIMODULES_ENABLED
# * Cjyx_QTLOADABLEMODULES_ENABLED
# * Cjyx_QTSCRIPTEDMODULES_ENABLED
#
# For a given module type (CLIMODULES, QTLOADABLEMODULES or QTSCRIPTEDMODULES), setting the
# Cjyx_<module_type>_ENABLED variable will cause any module list in the Cjyx_<module_type>_DISABLED
# variables to be ignored.
#
# Finally, the following functions allows to check if a given module is enabled or disabled:
#
#  cjyx_is_cli_builtin_module_enabled(<modulename> <output_var>)
#
#  cjyx_is_loadable_builtin_module_enabled(<modulename> <output_var>)
#
#  cjyx_is_scripted_builtin_module_enabled(<modulename> <output_var>)
#

macro(cjyx_is_cli_builtin_module_enabled module_name output_var)
  _cjyx_is_module_enabled("Cjyx_CLIMODULES" ${module_name} ${output_var})
endmacro()

macro(cjyx_is_loadable_builtin_module_enabled module_name output_var)
  _cjyx_is_module_enabled("Cjyx_QTLOADABLEMODULES" ${module_name} ${output_var})
endmacro()

macro(cjyx_is_scripted_builtin_module_enabled module_name output_var)
  _cjyx_is_module_enabled("Cjyx_QTSCRIPTEDMODULES" ${module_name} ${output_var})
endmacro()

function(_cjyx_is_module_enabled enabling_var_prefix module_name output_var)
  set(_build_module 1)
  # Is module explicitly disabled ?
  if(${enabling_var_prefix}_DISABLED)
    list(FIND ${enabling_var_prefix}_DISABLED ${module_name} _module_disabled)
    if(${_module_disabled} GREATER -1)
      set(_build_module 0)
    endif()
  endif()
  # Is module explicitly enabled ?
  if(${enabling_var_prefix}_ENABLED)
    list(FIND ${enabling_var_prefix}_ENABLED ${module_name} _module_enabled)
    if(NOT ${_module_enabled} GREATER -1)
      set(_build_module 0)
    endif()
  endif()
  set(${output_var} ${_build_module} PARENT_SCOPE)
endfunction()

