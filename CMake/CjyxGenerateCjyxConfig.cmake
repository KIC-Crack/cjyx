# Generate the CjyxConfig.cmake file in the build tree and configure one
# for the installation tree.  This file tells external project how to use
# Cjyx.  This provides a mechanism for third party developers to build
# modules against a Cjyx installation.
#
#

# The configuration process is very different for a build tree and an
# installation. The resulting directory structures are vastly
# different. So, the two configured files not only have different
# settings, they have a different structure.

# Settings that are the same for build trees and installation trees
#
#

# Settings specific to build trees
#
#

set(Cjyx_USE_FILE_CONFIG ${Cjyx_USE_FILE})

# Compilation settings
set(Cjyx_CMAKE_CXX_COMPILER_CONFIG ${CMAKE_CXX_COMPILER})
set(Cjyx_CMAKE_C_COMPILER_CONFIG   ${CMAKE_C_COMPILER})
set(Cjyx_CMAKE_CXX_STANDARD_CONFIG          ${CMAKE_CXX_STANDARD})
set(Cjyx_CMAKE_CXX_STANDARD_REQUIRED_CONFIG ${CMAKE_CXX_STANDARD_REQUIRED})
set(Cjyx_CMAKE_CXX_EXTENSIONS_CONFIG        ${CMAKE_CXX_EXTENSIONS})

# Launcher command
set(Cjyx_LAUNCHER_EXECUTABLE_CONFIG ${Cjyx_LAUNCHER_EXECUTABLE})
set(Cjyx_LAUNCH_COMMAND_CONFIG ${Cjyx_LAUNCH_COMMAND})

# License and Readme file
set(Cjyx_LICENSE_FILE_CONFIG ${Cjyx_SOURCE_DIR}/License.txt)
set(Cjyx_README_FILE_CONFIG ${Cjyx_SOURCE_DIR}/README.txt)

# Test templates directory
set(Cjyx_CXX_MODULE_TEST_TEMPLATES_DIR_CONFIG ${Cjyx_CXX_MODULE_TEST_TEMPLATES_DIR})
set(Cjyx_PYTHON_MODULE_TEST_TEMPLATES_DIR_CONFIG ${Cjyx_PYTHON_MODULE_TEST_TEMPLATES_DIR})

# Path to extension CPack script(s)
set(Cjyx_EXTENSION_CPACK_CONFIG ${Cjyx_EXTENSION_CPACK})
set(Cjyx_EXTENSION_CPACK_BUNDLE_FIXUP_CONFIG ${Cjyx_SOURCE_DIR}/CMake/CjyxExtensionCPackBundleFixup.cmake.in)

set(Cjyx_GUI_LIBRARY_CONFIG ${Cjyx_GUI_LIBRARY})
set(Cjyx_CORE_LIBRARY_CONFIG ${Cjyx_CORE_LIBRARY})

get_property(_module_targets GLOBAL PROPERTY CJYX_MODULE_TARGETS)
if(_module_targets)
  foreach(target ${_module_targets})
    set(Cjyx_INCLUDE_MODULE_DIRS_CONFIG
"${Cjyx_INCLUDE_MODULE_DIRS_CONFIG}
set(${target}_INCLUDE_DIRS
  \"${${target}_INCLUDE_DIRS}\")"
)
  endforeach()
endif()

get_property(_module_logic_targets GLOBAL PROPERTY CJYX_MODULE_LOGIC_TARGETS)
if(_module_logic_targets)
  foreach(target ${_module_logic_targets})
    set(Cjyx_INCLUDE_MODULE_LOGIC_DIRS_CONFIG
"${Cjyx_INCLUDE_MODULE_LOGIC_DIRS_CONFIG}
set(${target}_INCLUDE_DIRS
  \"${${target}_INCLUDE_DIRS}\")"
)
    list(APPEND Cjyx_ModuleLogic_INCLUDE_DIRS_CONFIG "\${${target}_INCLUDE_DIRS}")
  endforeach()
endif()

get_property(_module_mrml_targets GLOBAL PROPERTY CJYX_MODULE_MRML_TARGETS)
if(_module_mrml_targets)
  foreach(target ${_module_mrml_targets})
    set(Cjyx_INCLUDE_MODULE_MRML_DIRS_CONFIG
"${Cjyx_INCLUDE_MODULE_MRML_DIRS_CONFIG}
set(${target}_INCLUDE_DIRS
  \"${${target}_INCLUDE_DIRS}\")"
)
    list(APPEND Cjyx_ModuleMRML_INCLUDE_DIRS_CONFIG "\${${target}_INCLUDE_DIRS}")
  endforeach()
endif()

get_property(_module_widget_targets GLOBAL PROPERTY CJYX_MODULE_WIDGET_TARGETS)
if(_module_widget_targets)
  foreach(target ${_module_widget_targets})
    set(Cjyx_INCLUDE_MODULE_WIDGET_DIRS_CONFIG
"${Cjyx_INCLUDE_MODULE_WIDGET_DIRS_CONFIG}
set(${target}_INCLUDE_DIRS
  \"${${target}_INCLUDE_DIRS}\")"
)
    list(APPEND Cjyx_ModuleWidgets_INCLUDE_DIRS_CONFIG "\${${target}_INCLUDE_DIRS}")
  endforeach()
endif()

get_property(_wrap_hierarchy_targets GLOBAL PROPERTY CJYX_WRAP_HIERARCHY_TARGETS)
if(_wrap_hierarchy_targets)
  foreach(target ${_wrap_hierarchy_targets})
    set(Cjyx_WRAP_HIERARCHY_FILES_CONFIG
"${Cjyx_WRAP_HIERARCHY_FILES_CONFIG}
set(${target}_WRAP_HIERARCHY_FILE
  \"${${target}_WRAP_HIERARCHY_FILE}\")"
)
  endforeach()
endif()

set(Cjyx_Libs_INCLUDE_DIRS_CONFIG ${Cjyx_Libs_INCLUDE_DIRS})
set(Cjyx_Base_INCLUDE_DIRS_CONFIG ${Cjyx_Base_INCLUDE_DIRS})

set(ITKFactoryRegistration_INCLUDE_DIRS_CONFIG ${ITKFactoryRegistration_INCLUDE_DIRS})
set(MRMLCore_INCLUDE_DIRS_CONFIG ${MRMLCore_INCLUDE_DIRS})
set(MRMLLogic_INCLUDE_DIRS_CONFIG ${MRMLLogic_INCLUDE_DIRS})
set(MRMLCLI_INCLUDE_DIRS_CONFIG ${MRMLCLI_INCLUDE_DIRS})
set(qMRMLWidgets_INCLUDE_DIRS_CONFIG ${qMRMLWidgets_INCLUDE_DIRS})
set(RemoteIO_INCLUDE_DIRS_CONFIG ${RemoteIO_INCLUDE_DIRS})
set(vtkTeem_INCLUDE_DIRS_CONFIG ${vtkTeem_INCLUDE_DIRS})
set(vtkAddon_INCLUDE_DIRS_CONFIG ${vtkAddon_INCLUDE_DIRS})
set(vtkITK_INCLUDE_DIRS_CONFIG ${vtkITK_INCLUDE_DIRS})
set(vtkSegmentationCore_INCLUDE_DIRS_CONFIG ${vtkSegmentationCore_INCLUDE_DIRS})

# Note: For sake of simplification, the macro 'cjyx_config_set_ep' is not invoked conditionally, if
# the configured 'value' parameter is an empty string, the macro 'cjyx_config_set_ep' is a no-op.

# Cjyx external projects variables
set(Cjyx_SUPERBUILD_EP_VARS_CONFIG)
foreach(varname ${Cjyx_EP_LABEL_FIND_PACKAGE} QtTesting_DIR BRAINSCommonLib_DIR)
  set(Cjyx_SUPERBUILD_EP_VARS_CONFIG
   "${Cjyx_SUPERBUILD_EP_VARS_CONFIG}
cjyx_config_set_ep(
  ${varname}
  \"${${varname}}\"
  CACHE STRING \"Path to project build directory or file\" FORCE)
")
endforeach()

# Cjyx external project component variables
set(Cjyx_EP_COMPONENT_VARS_CONFIG
  "set(Cjyx_VTK_COMPONENTS \"${Cjyx_VTK_COMPONENTS}\")")

# List all required external project
set(Cjyx_EXTERNAL_PROJECTS_CONFIG CTK CTKAppLauncherLib ITK CURL Teem VTK RapidJSON)
set(Cjyx_EXTERNAL_PROJECTS_NO_USEFILE_CONFIG CURL CTKAppLauncherLib RapidJSON)
if(Cjyx_USE_CTKAPPLAUNCHER)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_CONFIG CTKAppLauncher)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_NO_USEFILE_CONFIG CTKAppLauncher)
endif()
if(Cjyx_USE_QtTesting)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_CONFIG QtTesting)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_NO_USEFILE_CONFIG QtTesting)
endif()
if(Cjyx_BUILD_CLI_SUPPORT)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_CONFIG CjyxExecutionModel)
endif()
if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_CONFIG qRestAPI)
endif()
if(Cjyx_BUILD_DICOM_SUPPORT)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_CONFIG DCMTK)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_NO_USEFILE_CONFIG DCMTK)
endif()
if(Cjyx_USE_PYTHONQT)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_CONFIG PythonLibs PythonInterp)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_NO_USEFILE_CONFIG PythonLibs PythonInterp)
endif()
if(Cjyx_USE_SimpleITK)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_CONFIG SWIG)
  list(APPEND Cjyx_EXTERNAL_PROJECTS_NO_USEFILE_CONFIG SWIG)
endif()
# Prevent VTK displaying the warning "The `VTK_USE_FILE` is no longer used starting with 8.90."
list(APPEND Cjyx_EXTERNAL_PROJECTS_NO_USEFILE_CONFIG VTK)

# Configure Cjyx_USE_SYSTEM_* variables
set(Cjyx_EP_USE_SYSTEM_VARS_CONFIG "")
foreach(varname ${Cjyx_EP_LABEL_USE_SYSTEM})
  set(Cjyx_EP_USE_SYSTEM_VARS_CONFIG
    "${Cjyx_EP_USE_SYSTEM_VARS_CONFIG}
set(Cjyx_USE_SYSTEM_${varname} \"${Cjyx_USE_SYSTEM_${varname}}\")"
    )
endforeach()

if(Cjyx_BUILD_CLI_SUPPORT)
  set(CjyxExecutionModel_CLI_LIBRARY_WRAPPER_CXX_CONFIG ${CjyxExecutionModel_CLI_LIBRARY_WRAPPER_CXX})
  set(CjyxExecutionModel_EXTRA_INCLUDE_DIRECTORIES_CONFIG ${CjyxExecutionModel_EXTRA_INCLUDE_DIRECTORIES})
  set(CjyxExecutionModel_EXTRA_EXECUTABLE_TARGET_LIBRARIES_CONFIG ${CjyxExecutionModel_EXTRA_EXECUTABLE_TARGET_LIBRARIES})
endif()

# Export Targets file.
set(Cjyx_TARGETS_FILE "${Cjyx_BINARY_DIR}/CjyxTargets.cmake")

# Configure CjyxConfig.cmake for the build tree.
configure_file(
  ${Cjyx_SOURCE_DIR}/CMake/CjyxConfig.cmake.in
  ${Cjyx_BINARY_DIR}/CjyxConfig.cmake @ONLY)
