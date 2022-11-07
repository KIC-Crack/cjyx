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
#  This file was originally developed by
#   Dave Partyka and Jean-Christophe Fillion-Robin, Kitware Inc.
#  and was partially funded by NIH grant 3P41RR013218-12S1
#
################################################################################

#-----------------------------------------------------------------------------
# CMake https support
#-----------------------------------------------------------------------------
include(CjyxCheckCMakeHTTPS)

#-----------------------------------------------------------------------------
# Git protocol option
#-----------------------------------------------------------------------------
if(EP_GIT_PROTOCOL STREQUAL "https")
  # Verify that the global git config has been updated with the expected "insteadOf" option.
  # XXX CMake 3.8: Replace this with use of GIT_CONFIG option provided by ExternalProject
  function(_check_for_required_git_config_insteadof base insteadof)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} config --global --get "url.${base}.insteadof"
      OUTPUT_VARIABLE output
      OUTPUT_STRIP_TRAILING_WHITESPACE
      RESULT_VARIABLE error_code
      )
    if(error_code OR NOT "${output}" STREQUAL "${insteadof}")
      message(FATAL_ERROR
"Since the ExternalProject modules doesn't provide a mechanism to customize the clone step by "
"adding 'git config' statement between the 'git checkout' and the 'submodule init', it is required "
"to manually update your global git config to successfully build ${CMAKE_PROJECT_NAME} with "
"option Cjyx_USE_GIT_PROTOCOL set to FALSE. "
"See https://mantisarchive.slicer.org/view.php?id=2731"
"\nYou could do so by running the command:\n"
"  ${GIT_EXECUTABLE} config --global url.${base}.insteadOf ${insteadof}\n")
    endif()
  endfunction()

endif()

#-----------------------------------------------------------------------------
# Enable and setup External project global properties
#-----------------------------------------------------------------------------

set(ep_common_c_flags "${CMAKE_C_FLAGS_INIT} ${ADDITIONAL_C_FLAGS}")
set(ep_common_cxx_flags "${CMAKE_CXX_FLAGS_INIT} ${ADDITIONAL_CXX_FLAGS}")

#-----------------------------------------------------------------------------
# Define list of additional options used to configure Cjyx
#------------------------------------------------------------------------------

if(DEFINED CTEST_CONFIGURATION_TYPE)
  mark_as_superbuild(CTEST_CONFIGURATION_TYPE)
endif()

if(DEFINED CMAKE_CONFIGURATION_TYPES)
  mark_as_superbuild(CMAKE_CONFIGURATION_TYPES)
endif()

# Provide a mechanism to disable/enable one or more modules.
mark_as_superbuild(
  Cjyx_QTLOADABLEMODULES_DISABLED:STRING
  Cjyx_QTLOADABLEMODULES_ENABLED:STRING
  Cjyx_QTSCRIPTEDMODULES_DISABLED:STRING
  Cjyx_QTSCRIPTEDMODULES_ENABLED:STRING
  Cjyx_CLIMODULES_DISABLED:STRING
  Cjyx_CLIMODULES_ENABLED:STRING
  )

#------------------------------------------------------------------------------
# Cjyx dependency list
#------------------------------------------------------------------------------

set(ITK_EXTERNAL_NAME ITK)

set(VTK_EXTERNAL_NAME VTK)

set(Cjyx_DEPENDENCIES
  curl
  CTKAppLauncherLib
  teem
  ${VTK_EXTERNAL_NAME}
  ${ITK_EXTERNAL_NAME}
  CTK
  LibArchive
  RapidJSON
  )

set(CURL_ENABLE_SSL ${Cjyx_USE_PYTHONQT_WITH_OPENSSL})

if(Cjyx_USE_SimpleITK)
  list(APPEND Cjyx_DEPENDENCIES SimpleITK)
endif()

if(Cjyx_BUILD_CLI_SUPPORT)
  list(APPEND Cjyx_DEPENDENCIES CjyxExecutionModel)
endif()

if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  list(APPEND Cjyx_DEPENDENCIES qRestAPI)
endif()

if(Cjyx_BUILD_DICOM_SUPPORT)
  list(APPEND Cjyx_DEPENDENCIES DCMTK)
endif()

if(Cjyx_USE_CTKAPPLAUNCHER)
  list(APPEND Cjyx_DEPENDENCIES CTKAPPLAUNCHER)
endif()

if(Cjyx_USE_PYTHONQT)
  set(PYTHON_ENABLE_SSL ${Cjyx_USE_PYTHONQT_WITH_OPENSSL})
  list(APPEND Cjyx_DEPENDENCIES python)
  list(APPEND Cjyx_DEPENDENCIES
    python-pythonqt-requirements  # This provides the "packaging.version.parse()" function
    )
  if(Cjyx_USE_SCIPY)
    list(APPEND Cjyx_DEPENDENCIES python-scipy)
  endif()
  if(Cjyx_USE_NUMPY)
    list(APPEND Cjyx_DEPENDENCIES python-numpy)
  endif()
  if(Cjyx_BUILD_DICOM_SUPPORT AND Cjyx_USE_PYTHONQT_WITH_OPENSSL)
    list(APPEND Cjyx_DEPENDENCIES python-dicom-requirements)
  endif()
  if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
    list(APPEND Cjyx_DEPENDENCIES
      python-extension-manager-requirements
      )
    if(Cjyx_USE_PYTHONQT_WITH_OPENSSL OR Cjyx_USE_SYSTEM_python)
      # python-PyGithub requires SSL support in Python
      list(APPEND Cjyx_DEPENDENCIES
        python-extension-manager-ssl-requirements
        )
    else()
      message(STATUS "--------------------------------------------------")
      message(STATUS "Python was built without SSL support; "
                    "github integration will not be available. "
                    "Set Cjyx_USE_PYTHONQT_WITH_OPENSSL=ON to enable this feature.")
      message(STATUS "--------------------------------------------------")
    endif()
  endif()
endif()

if(Cjyx_USE_TBB)
  list(APPEND Cjyx_DEPENDENCIES tbb)
endif()

include(CjyxCheckModuleEnabled)

# JsonCpp is required to build VolumeRendering module
cjyx_is_loadable_builtin_module_enabled("VolumeRendering" _build_volume_rendering_module)
if(_build_volume_rendering_module)
  list(APPEND Cjyx_DEPENDENCIES JsonCpp)
endif()

#------------------------------------------------------------------------------
include(ExternalProjectAddSource)

macro(list_conditional_append cond list)
  if(${cond})
    list(APPEND ${list} ${ARGN})
  endif()
endmacro()

#
# Support for remote sources:
#
# * Calling Cjyx_Remote_Add downloads the corresponding sources and set the variable
#   <proj>_SOURCE_DIR.
#
# * Cjyx_Remote_Add ensures <proj>_SOURCE_DIR is passed to inner build using mark_as_superbuild.
#
# * If <proj>_SOURCE_DIR is already defined, no sources are downloaded.
#
# * Specifying OPTION_NAME adds a CMake option allowing to disable the download of the sources.
#   It is enabled by default and passed to the inner build using mark_as_superbuild.
#
# * Specifying labels REMOTE_MODULE or REMOTE_EXTENSION allows the corresponding sources to be
#   be automatically added. See "Bundle remote modules and extensions adding source directories"
#   in top-level CMakeLists.txt.
#
# Corresponding logic is implemented in ExternalProjectAddSource.cmake
#

#------------------------------------------------------------------------------
# Include remote libraries
#------------------------------------------------------------------------------

Cjyx_Remote_Add(vtkAddon
  GIT_REPOSITORY "${EP_GIT_PROTOCOL}://github.com/Slicer/vtkAddon"
  GIT_TAG 8b5c4b2336a4d2d2aaafe87db3642b5302ddcaa5
  OPTION_NAME Cjyx_BUILD_vtkAddon
  )
list_conditional_append(Cjyx_BUILD_vtkAddon Cjyx_REMOTE_DEPENDENCIES vtkAddon)

set(vtkAddon_CMAKE_DIR ${vtkAddon_SOURCE_DIR}/CMake)
mark_as_superbuild(vtkAddon_CMAKE_DIR:PATH)

set(vtkAddon_LAUNCH_COMMAND ${Cjyx_LAUNCH_COMMAND})
mark_as_superbuild(vtkAddon_LAUNCH_COMMAND:STRING)

set(vtkAddon_USE_UTF8 ON)
mark_as_superbuild(vtkAddon_USE_UTF8:BOOL)

set(vtkAddon_WRAP_PYTHON ${Cjyx_USE_PYTHONQT})
mark_as_superbuild(vtkAddon_WRAP_PYTHON:BOOL)

#------------------------------------------------------------------------------
# Include remote modules
#------------------------------------------------------------------------------

option(Cjyx_BUILD_MULTIVOLUME_SUPPORT "Build MultiVolume support." ON)
mark_as_advanced(Cjyx_BUILD_MULTIVOLUME_SUPPORT)

Cjyx_Remote_Add(MultiVolumeExplorer
  GIT_REPOSITORY ${EP_GIT_PROTOCOL}://github.com/KIC-Crack/MultiVolumeExplorer.git
  GIT_TAG 3a9c87fcf3ed14b773e9eb9380ce291b4504ca99
  OPTION_NAME Cjyx_BUILD_MultiVolumeExplorer
  OPTION_DEPENDS "Cjyx_BUILD_QTLOADABLEMODULES;Cjyx_BUILD_MULTIVOLUME_SUPPORT;Cjyx_USE_PYTHONQT"
  LABELS REMOTE_MODULE
  )
list_conditional_append(Cjyx_BUILD_MultiVolumeExplorer Cjyx_REMOTE_DEPENDENCIES MultiVolumeExplorer)

Cjyx_Remote_Add(MultiVolumeImporter
  GIT_REPOSITORY ${EP_GIT_PROTOCOL}://github.com/KIC-Crack/MultiVolumeImporter.git
  GIT_TAG f04d867377fb6d2de62eac8390643574fde00f3a
  OPTION_NAME Cjyx_BUILD_MultiVolumeImporter
  OPTION_DEPENDS "Cjyx_BUILD_QTLOADABLEMODULES;Cjyx_BUILD_MULTIVOLUME_SUPPORT;Cjyx_USE_PYTHONQT"
  LABELS REMOTE_MODULE
  )
list_conditional_append(Cjyx_BUILD_MultiVolumeImporter Cjyx_REMOTE_DEPENDENCIES MultiVolumeImporter)

Cjyx_Remote_Add(SimpleFilters
  GIT_REPOSITORY ${EP_GIT_PROTOCOL}://github.com/KIC-Crack/SlicerSimpleFilters.git
  GIT_TAG 56c48373843bd8de4430572d2679cdbb1a0411d2
  OPTION_NAME Cjyx_BUILD_SimpleFilters
  OPTION_DEPENDS "Cjyx_BUILD_QTSCRIPTEDMODULES;Cjyx_USE_SimpleITK"
  LABELS REMOTE_MODULE
  )
list_conditional_append(Cjyx_BUILD_SimpleFilters Cjyx_REMOTE_DEPENDENCIES SimpleFilters)


# BRAINSTools_hidden_options are internal options needed for BRAINSTools that should be hidden
set(BRAINSTools_hidden_options
  BRAINSTools_SUPERBUILD:INTERNAL=OFF
  BRAINSTools_BUILD_DICOM_SUPPORT:INTERNAL=${Cjyx_BUILD_DICOM_SUPPORT}
  BRAINSTools_CLI_LIBRARY_OUTPUT_DIRECTORY:INTERNAL=${CMAKE_BINARY_DIR}/${Cjyx_BINARY_INNER_SUBDIR}/${Cjyx_CLIMODULES_LIB_DIR}
  BRAINSTools_CLI_ARCHIVE_OUTPUT_DIRECTORY:INTERNAL=${CMAKE_BINARY_DIR}/${Cjyx_BINARY_INNER_SUBDIR}/${Cjyx_CLIMODULES_LIB_DIR}
  BRAINSTools_CLI_RUNTIME_OUTPUT_DIRECTORY:INTERNAL=${CMAKE_BINARY_DIR}/${Cjyx_BINARY_INNER_SUBDIR}/${Cjyx_CLIMODULES_BIN_DIR}
  BRAINSTools_CLI_INSTALL_LIBRARY_DESTINATION:INTERNAL=${Cjyx_INSTALL_CLIMODULES_LIB_DIR}
  BRAINSTools_CLI_INSTALL_ARCHIVE_DESTINATION:INTERNAL=${Cjyx_INSTALL_CLIMODULES_LIB_DIR}
  BRAINSTools_CLI_INSTALL_RUNTIME_DESTINATION:INTERNAL=${Cjyx_INSTALL_CLIMODULES_BIN_DIR}
  BRAINSTools_ExternalData_DATA_MANAGEMENT_TARGET:INTERNAL=${Cjyx_ExternalData_DATA_MANAGEMENT_TARGET}
  BRAINSTools_DISABLE_TESTING:INTERNAL=ON
  # BRAINSTools comes with some extra tool that should not be compiled by default
  USE_DebugImageViewer:INTERNAL=OFF
  BRAINS_DEBUG_IMAGE_WRITE:INTERNAL=OFF
  USE_BRAINSRefacer:INTERNAL=OFF
  USE_AutoWorkup:INTERNAL=OFF
  USE_ReferenceAtlas:INTERNAL=OFF
  USE_ANTS:INTERNAL=OFF
  USE_BRAINSABC:INTERNAL=OFF
  USE_BRAINSTalairach:INTERNAL=OFF
  USE_BRAINSMush:INTERNAL=OFF
  USE_BRAINSInitializedControlPoints:INTERNAL=OFF
  USE_BRAINSLabelStats:INTERNAL=OFF
  USE_BRAINSMultiModeSegment:INTERNAL=OFF
  USE_ImageCalculator:INTERNAL=OFF
  USE_BRAINSSnapShotWriter:INTERNAL=OFF
  USE_ConvertBetweenFileFormats:INTERNAL=OFF
  USE_BRAINSMultiSTAPLE:INTERNAL=OFF
  USE_BRAINSCreateLabelMapFromProbabilityMaps:INTERNAL=OFF
  USE_BRAINSContinuousClass:INTERNAL=OFF
  USE_BRAINSPosteriorToContinuousClass:INTERNAL=OFF
  USE_GTRACT:INTERNAL=OFF
  USE_BRAINSConstellationDetector:INTERNAL=OFF
  USE_BRAINSLandmarkInitializer:INTERNAL=OFF
  )

# BRAINSTools_cjyx_options are options exposed when building Cjyx
set(BRAINSTools_cjyx_options
  USE_BRAINSFit:BOOL=ON
  USE_BRAINSROIAuto:BOOL=ON
  USE_BRAINSResample:BOOL=ON
  USE_BRAINSTransformConvert:BOOL=ON
  USE_DWIConvert:BOOL=${Cjyx_BUILD_DICOM_SUPPORT} ## Need to figure out library linking
)


Cjyx_Remote_Add(BRAINSTools
  GIT_REPOSITORY ${EP_GIT_PROTOCOL}://github.com/KIC-Crack/BRAINSTools.git
  GIT_TAG "ebda957241375d656c686132e62097c1c3915623"  # Jan 26th 2022
  LICENSE_FILES "https://www.apache.org/licenses/LICENSE-2.0.txt"
  OPTION_NAME Cjyx_BUILD_BRAINSTOOLS
  OPTION_DEPENDS "Cjyx_BUILD_CLI_SUPPORT;Cjyx_BUILD_CLI"
  LABELS REMOTE_MODULE
  VARS ${BRAINSTools_cjyx_options} ${BRAINSTools_hidden_options}
  )
list_conditional_append(Cjyx_BUILD_BRAINSTOOLS Cjyx_REMOTE_DEPENDENCIES BRAINSTools)
if(Cjyx_BUILD_BRAINSTOOLS)
  # This is added to CjyxConfig and is useful for extension depending on BRAINSTools
  set(BRAINSCommonLib_DIR "${Cjyx_BINARY_DIR}/${Cjyx_BINARY_INNER_SUBDIR}/Modules/Remote/BRAINSTools/BRAINSCommonLib")
  mark_as_superbuild(BRAINSCommonLib_DIR:PATH)
endif()

Cjyx_Remote_Add(CompareVolumes
  GIT_REPOSITORY "${EP_GIT_PROTOCOL}://github.com/KIC-Crack/CompareVolumes"
  GIT_TAG 8f27fc45af35772dd4a38a018902fb0152b28286
  OPTION_NAME Cjyx_BUILD_CompareVolumes
  OPTION_DEPENDS "Cjyx_USE_PYTHONQT"
  LABELS REMOTE_MODULE
  )
list_conditional_append(Cjyx_BUILD_CompareVolumes Cjyx_REMOTE_DEPENDENCIES CompareVolumes)

Cjyx_Remote_Add(LandmarkRegistration
  GIT_REPOSITORY "${EP_GIT_PROTOCOL}://github.com/KIC-Crack/LandmarkRegistration"
  GIT_TAG 6f7559b206101ba0a7c616eed953a6a5e87e2193
  OPTION_NAME Cjyx_BUILD_LandmarkRegistration
  OPTION_DEPENDS "Cjyx_BUILD_CompareVolumes;Cjyx_USE_PYTHONQT"
  LABELS REMOTE_MODULE
 )
list_conditional_append(Cjyx_BUILD_LandmarkRegistration Cjyx_REMOTE_DEPENDENCIES LandmarkRegistration)

Cjyx_Remote_Add(SurfaceToolbox
  GIT_REPOSITORY "${EP_GIT_PROTOCOL}://github.com/KIC-Crack/SlicerSurfaceToolbox"
  GIT_TAG dd15cc6665aa2be2c673cbca9391ab4d4d670c45
  OPTION_NAME Cjyx_BUILD_SurfaceToolbox
  OPTION_DEPENDS "Cjyx_USE_PYTHONQT"
  LABELS REMOTE_MODULE
  )
list_conditional_append(Cjyx_BUILD_SurfaceToolbox Cjyx_REMOTE_DEPENDENCIES SurfaceToolbox)

#------------------------------------------------------------------------------
# Superbuild-type bundled extensions
#------------------------------------------------------------------------------

# The following logic is documented in the "Bundle remote modules and extensions adding source directories."
# section found in the top-level CMakeLists.txt

set(_all_extension_depends )

# Build only inner-build for superbuild-type extensions
set(Cjyx_BUNDLED_EXTENSION_NAMES)
foreach(extension_dir ${Cjyx_EXTENSION_SOURCE_DIRS})
  get_filename_component(extension_dir ${extension_dir} ABSOLUTE)
  get_filename_component(extension_name ${extension_dir} NAME) # The assumption is that source directories are named after the extension project
  if(EXISTS ${extension_dir}/SuperBuild OR EXISTS ${extension_dir}/Superbuild)
    set(${extension_name}_SUPERBUILD 0)
    mark_as_superbuild(${extension_name}_SUPERBUILD:BOOL)

    if(NOT DEFINED ${extension_name}_EXTERNAL_PROJECT_EXCLUDE_ALL)
      set(${extension_name}_EXTERNAL_PROJECT_EXCLUDE_ALL FALSE)
    endif()
    if(NOT ${extension_name}_EXTERNAL_PROJECT_EXCLUDE_ALL)
      list(APPEND EXTERNAL_PROJECT_ADDITIONAL_DIRS "${extension_dir}/SuperBuild")
      list(APPEND EXTERNAL_PROJECT_ADDITIONAL_DIRS "${extension_dir}/Superbuild")
    endif()
    if(NOT DEFINED ${extension_name}_EXTERNAL_PROJECT_DEPENDENCIES)
      set(${extension_name}_EXTERNAL_PROJECT_DEPENDENCIES )
    endif()

    set(_external_project_cmake_files)

    # SuperBuild
    file(GLOB _external_project_cmake_files1 RELATIVE "${extension_dir}/SuperBuild" "${extension_dir}/SuperBuild/External_*.cmake")
    list(APPEND _external_project_cmake_files ${_external_project_cmake_files1})

    # Superbuild
    file(GLOB _external_project_cmake_files2 RELATIVE "${extension_dir}/Superbuild" "${extension_dir}/Superbuild/External_*.cmake")
    list(APPEND _external_project_cmake_files ${_external_project_cmake_files2})

    list(REMOVE_DUPLICATES _external_project_cmake_files)

    set(_extension_depends)
    set(_msg_extension_depends)
    foreach (_external_project_cmake_file ${_external_project_cmake_files})
      string(REGEX MATCH "External_(.+)\.cmake" _match ${_external_project_cmake_file})
      set(_additional_project_name "${CMAKE_MATCH_1}")
      if(${extension_name}_EXTERNAL_PROJECT_EXCLUDE_ALL)
        set(_include FALSE)
      else()
        set(_include TRUE)
        if(NOT "${${extension_name}_EXTERNAL_PROJECT_DEPENDENCIES}" STREQUAL "")
          list(FIND ${extension_name}_EXTERNAL_PROJECT_DEPENDENCIES ${_additional_project_name} _index)
          if(_index EQUAL -1)
            set(_include FALSE)
          endif()
        endif()
      endif()
      if(_include)
          list(APPEND _extension_depends ${_additional_project_name})
          list(APPEND _msg_extension_depends ${_additional_project_name})
      else()
        list(APPEND _msg_extension_depends "exclude(${_additional_project_name})")
      endif()
    endforeach()

    list(APPEND Cjyx_BUNDLED_EXTENSION_NAMES ${extension_name})

    message(STATUS "SuperBuild - ${extension_name} extension => ${_msg_extension_depends}")

    list(APPEND _all_extension_depends ${_extension_depends})
  endif()
endforeach()

if(_all_extension_depends)
  list(REMOVE_DUPLICATES _all_extension_depends)
endif()

list(APPEND Cjyx_DEPENDENCIES ${_all_extension_depends})

mark_as_superbuild(Cjyx_BUNDLED_EXTENSION_NAMES:STRING)


#------------------------------------------------------------------------------
# Cjyx_ADDITIONAL_PROJECTS
#------------------------------------------------------------------------------

#
# List of <proj>_DIR variables that will be passed to the inner build.
# Then, the variables are:
# (1) associated with CPACK_INSTALL_CMAKE_PROJECTS in CjyxCPack
# (2) used to get <proj>_LIBRARY_DIRS and update "libs_path" in CjyxCPackBundleFixup.
#

list(APPEND Cjyx_ADDITIONAL_PROJECTS ${Cjyx_ADDITIONAL_DEPENDENCIES})
if(Cjyx_ADDITIONAL_PROJECTS)
  list(REMOVE_DUPLICATES Cjyx_ADDITIONAL_PROJECTS)
  foreach(additional_project ${Cjyx_ADDITIONAL_PROJECTS})
    # needed to do find_package within Cjyx
    mark_as_superbuild(${additional_project}_DIR:PATH)
  endforeach()
  mark_as_superbuild(Cjyx_ADDITIONAL_PROJECTS:STRING)
endif()


#------------------------------------------------------------------------------
# Cjyx_ADDITIONAL_DEPENDENCIES, EXTERNAL_PROJECT_ADDITIONAL_DIR, EXTERNAL_PROJECT_ADDITIONAL_DIRS
#------------------------------------------------------------------------------

#
# Setting the variable Cjyx_ADDITIONAL_DEPENDENCIES allows to introduce additional
# Cjyx external project dependencies.
#
# Additional external project files are looked up in the EXTERNAL_PROJECT_ADDITIONAL_DIR and EXTERNAL_PROJECT_ADDITIONAL_DIRS
#

if(DEFINED Cjyx_ADDITIONAL_DEPENDENCIES)
  list(APPEND Cjyx_DEPENDENCIES ${Cjyx_ADDITIONAL_DEPENDENCIES})
endif()

mark_as_superbuild(Cjyx_DEPENDENCIES:STRING)


#------------------------------------------------------------------------------
# Process external projects, aggregate variable marked as superbuild and set <proj>_EP_ARGS variable.
#------------------------------------------------------------------------------

ExternalProject_Include_Dependencies(Cjyx DEPENDS_VAR Cjyx_DEPENDENCIES)

#------------------------------------------------------------------------------
# Define list of additional options used to configure Cjyx
#------------------------------------------------------------------------------

set(EXTERNAL_PROJECT_OPTIONAL_ARGS)
if(WIN32)
  list(APPEND EXTERNAL_PROJECT_OPTIONAL_ARGS -DCjyx_SKIP_ROOT_DIR_MAX_LENGTH_CHECK:BOOL=ON)
endif()

#------------------------------------------------------------------------------
# Customizing CjyxApp metadata
#------------------------------------------------------------------------------

# Configuring Cjyx setting these variables allows to overwrite the properties
# associated with the CjyxApp application.

foreach(name IN ITEMS
  DESCRIPTION_SUMMARY
  DESCRIPTION_FILE
  LAUNCHER_SPLASHSCREEN_FILE
  APPLE_ICON_FILE
  WIN_ICON_FILE
  LICENSE_FILE
  )
  if(DEFINED CjyxApp_${name})
    list(APPEND EXTERNAL_PROJECT_OPTIONAL_ARGS
      -DCjyxApp_${name}:STRING=${CjyxApp_${name}}
      )
  endif()
endforeach()

#------------------------------------------------------------------------------
# Cjyx_EXTENSION_SOURCE_DIRS
#------------------------------------------------------------------------------

#
# Configuring Cjyx using
#
#   cmake -DCjyx_EXTENSION_SOURCE_DIRS:STRING=/path/to/ExtensionA;/path/to/ExtensionB [...] /path/to/source/Cjyx
#
# will ensure the source of each extensions are *built* by Cjyx. This is done
# as part of the Cjyx inner build by adding each directory in the top-level CMakeLists.txt.
#
# Note that using 'Cjyx_Remote_Add' specifying the label 'REMOTE_EXTENSION' (see above)
# will checkout the extension sources and append the corresponding source directory to
# the variable Cjyx_EXTENSION_SOURCE_DIRS.
#

#------------------------------------------------------------------------------
# Cjyx_EXTENSION_INSTALL_DIRS
#------------------------------------------------------------------------------

#
# Configuring Cjyx using
#
#   cmake -DCjyx_EXTENSION_INSTALL_DIRS:STRING=/path/to/ExtensionA-install-tree;/path/to/ExtensionB-install-tree [...] /path/to/source/Cjyx
#
# will ensure the content of each extensions install directories are *packaged*
# with Cjyx.
#
# Corresponding install rules are found in "CMake/CjyxBlockInstallExtensionPackages.cmake"
#

#------------------------------------------------------------------------------
# Configure and build Cjyx
#------------------------------------------------------------------------------
set(proj Cjyx)

ExternalProject_Add(${proj}
  ${${proj}_EP_ARGS}
  DEPENDS ${Cjyx_DEPENDENCIES} ${Cjyx_REMOTE_DEPENDENCIES}
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  BINARY_DIR ${CMAKE_BINARY_DIR}/${Cjyx_BINARY_INNER_SUBDIR}
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ""
  CMAKE_CACHE_ARGS
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${ep_common_cxx_flags}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${ep_common_c_flags}
    -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}
    -DCMAKE_CXX_STANDARD_REQUIRED:BOOL=${CMAKE_CXX_STANDARD_REQUIRED}
    -DCMAKE_CXX_EXTENSIONS:BOOL=${CMAKE_CXX_EXTENSIONS}
    -DADDITIONAL_C_FLAGS:STRING=${ADDITIONAL_C_FLAGS}
    -DADDITIONAL_CXX_FLAGS:STRING=${ADDITIONAL_CXX_FLAGS}
    -DCjyx_REQUIRED_C_FLAGS:STRING=${Cjyx_REQUIRED_C_FLAGS}
    -DCjyx_REQUIRED_CXX_FLAGS:STRING=${Cjyx_REQUIRED_CXX_FLAGS}
    -DCjyx_SUPERBUILD:BOOL=OFF
    -DCjyx_SUPERBUILD_DIR:PATH=${CMAKE_BINARY_DIR}
    -D${Cjyx_MAIN_PROJECT}_APPLICATION_NAME:STRING=${${Cjyx_MAIN_PROJECT}_APPLICATION_NAME}
    -DCjyx_EXTENSION_SOURCE_DIRS:STRING=${Cjyx_EXTENSION_SOURCE_DIRS}
    -DCjyx_EXTENSION_INSTALL_DIRS:STRING=${Cjyx_EXTENSION_INSTALL_DIRS}
    -DExternalData_OBJECT_STORES:PATH=${ExternalData_OBJECT_STORES}
    ${EXTERNAL_PROJECT_OPTIONAL_ARGS}
  INSTALL_COMMAND ""
  )

ExternalProject_AlwaysConfigure(${proj})
