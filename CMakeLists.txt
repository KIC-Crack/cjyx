cmake_minimum_required(VERSION 3.16.3...3.19.7 FATAL_ERROR)

#-----------------------------------------------------------------------------
# Setting C++ Standard
#-----------------------------------------------------------------------------
set(_msg "Setting C++ standard")
message(STATUS "${_msg}")
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "${_msg} - C++${CMAKE_CXX_STANDARD}")
if(NOT CMAKE_CXX_STANDARD MATCHES "^(14|17|20)$")
  message(FATAL_ERROR "CMAKE_CXX_STANDARD must be set to 14, 17 or 20")
endif()

#-----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
set(SUPERBUILD_TOPLEVEL_PROJECT Cjyx)
list(APPEND EXTERNAL_PROJECT_ADDITIONAL_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/SuperBuild)
include(ExternalProject)
include(ExternalProjectDependency)
include(ExternalProjectDependencyForPython)
include(ExternalProjectGenerateProjectDescription)

#-----------------------------------------------------------------------------
if(APPLE)
  # Note: By setting CMAKE_OSX_* variables before any enable_language() or project() calls,
  #       we ensure that the bitness, and C++ standard library will be properly detected.
  include(CjyxInitializeOSXVariables)
  mark_as_superbuild(
    VARS CMAKE_OSX_ARCHITECTURES:STRING CMAKE_OSX_SYSROOT:PATH CMAKE_OSX_DEPLOYMENT_TARGET:STRING
    ALL_PROJECTS
    )

  set(CMAKE_MACOSX_RPATH 0)
  mark_as_superbuild(VARS CMAKE_MACOSX_RPATH:BOOL ALL_PROJECTS)
endif()

#-----------------------------------------------------------------------------
if(NOT DEFINED Cjyx_VERSION_MAJOR)
  set(Cjyx_VERSION_MAJOR "5")
endif()
if(NOT DEFINED Cjyx_VERSION_MINOR)
  set(Cjyx_VERSION_MINOR "0")
endif()
if(NOT DEFINED Cjyx_VERSION_PATCH)
  set(Cjyx_VERSION_PATCH "3")
endif()
project(Cjyx VERSION "${Cjyx_VERSION_MAJOR}.${Cjyx_VERSION_MINOR}.${Cjyx_VERSION_PATCH}")

#-----------------------------------------------------------------------------
# Update CMake module path
#------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  ${Cjyx_SOURCE_DIR}/Extensions/CMake
  ${CMAKE_MODULE_PATH}
  )

set(Cjyx_CMAKE_DIR ${Cjyx_SOURCE_DIR}/CMake)
set(Cjyx_EXTENSIONS_CMAKE_DIR ${Cjyx_SOURCE_DIR}/Extensions/CMake)

#-----------------------------------------------------------------------------
# https://www.slicer.org/w/index.php/Documentation/Nightly/Developers/DevelopmentWithGit
#-----------------------------------------------------------------------------
include(CjyxCheckSourceTree)

#-----------------------------------------------------------------------------
# Superbuild Option - Enabled by default
#-----------------------------------------------------------------------------
option(Cjyx_SUPERBUILD "Build ${PROJECT_NAME} and the projects it depends on." ON)
mark_as_advanced(Cjyx_SUPERBUILD)
set(Cjyx_BINARY_INNER_SUBDIR Cjyx-build)

#-----------------------------------------------------------------------------
include(CTestUseLaunchers OPTIONAL)
if(${CTEST_USE_LAUNCHERS})
  message(STATUS "CTest launchers enabled")
endif()

#-----------------------------------------------------------------------------
# Sanity checks
#------------------------------------------------------------------------------
if(WIN32)
  set(${PROJECT_NAME}_ROOT_DIR_MAX_LENGTH 40)
  if(NOT ${PROJECT_NAME}_SUPERBUILD)
    string(LENGTH ${Cjyx_BINARY_INNER_SUBDIR} _inner_subdir_length)
    math(EXPR ${PROJECT_NAME}_ROOT_DIR_MAX_LENGTH "${${PROJECT_NAME}_ROOT_DIR_MAX_LENGTH} + ${_inner_subdir_length}")
  endif()
  include(PreventDirWithTooManyChars)
endif()
include(PreventInSourceBuilds)
include(PreventInBuildInstalls)
include(PreventDirWithSpaces)
mark_as_superbuild(${PROJECT_NAME}_SKIP_DIR_WITH_SPACES_CHECK:BOOL)

#-----------------------------------------------------------------------------
# Overwrite default options
#-----------------------------------------------------------------------------
set(_overwrite_options_file ${Cjyx_CMAKE_DIR}/CjyxOverwriteDefaultOptions.cmake)
if(EXISTS ${_overwrite_options_file})
  include(${_overwrite_options_file})
endif()

#-----------------------------------------------------------------------------
# Check if the linker will resolve symbols of underlinked libraries
#-----------------------------------------------------------------------------
if(UNIX AND NOT APPLE)
  include(CjyxLinkerAsNeededFlagCheck)
  if(Cjyx_LINKER_NO_AS_NEEDED_FLAG_REQUIRED)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed"
      CACHE STRING "Flags used by the linker"
      FORCE)
  endif()
endif()

#-----------------------------------------------------------------------------
# Cjyx Build and Release type
#-----------------------------------------------------------------------------
include(CjyxInitializeBuildType)
include(CjyxInitializeReleaseType)

#-----------------------------------------------------------------------------
# Set the header template which defines custom export/import macros
# for shared libraries
#-----------------------------------------------------------------------------
set(Cjyx_EXPORT_HEADER_TEMPLATE "${Cjyx_SOURCE_DIR}/CMake/qCjyxExport.h.in")
set(Cjyx_LOGOS_RESOURCE "${Cjyx_SOURCE_DIR}/Resources/qCjyx.qrc")

#-----------------------------------------------------------------------------
# Platform checks
#-----------------------------------------------------------------------------
include(CjyxBlockPlatformCheck)
mark_as_superbuild(Cjyx_PLATFORM_CHECK:BOOL)

#-----------------------------------------------------------------------------
# Determine endian type
#-----------------------------------------------------------------------------
include(TestBigEndian)
test_big_endian(CMAKE_WORDS_BIGENDIAN)

#-----------------------------------------------------------------------------
# Prerequisites
#-----------------------------------------------------------------------------
find_package(Git)
if(NOT GIT_FOUND)
  message(FATAL_ERROR "error: Install Git and try to re-configure")
endif()
mark_as_superbuild(GIT_EXECUTABLE)

find_package(Patch REQUIRED)
mark_as_superbuild(Patch_EXECUTABLE)

#-----------------------------------------------------------------------------
# Qt requirements
#-----------------------------------------------------------------------------
if(NOT DEFINED Cjyx_REQUIRED_QT_VERSION)
  # Official builds and tests are performed with Qt-5.15 on all platforms,
  # but since it is not easy to get latest Qt versions on Linux, the version
  # requirement is relaxed on Linux so that it builds on Ubuntu-20.04.
  # See https://github.com/Slicer/Slicer/issues/5804 for more details.
  if(UNIX AND NOT APPLE)
    # Linux
    set(_required_qt_version "5.12")
  else()
    # Windows and macOS
    set(_required_qt_version "5.15")
  endif()
  set(Cjyx_REQUIRED_QT_VERSION ${_required_qt_version} CACHE STRING "Minimum required Qt version" FORCE)
endif()
mark_as_superbuild(Cjyx_REQUIRED_QT_VERSION)

#-----------------------------------------------------------------------------
# Python requirements
#-----------------------------------------------------------------------------
if(NOT DEFINED Cjyx_REQUIRED_PYTHON_VERSION)
  set(Cjyx_REQUIRED_PYTHON_VERSION "3.9.10")
endif()
mark_as_superbuild(Cjyx_REQUIRED_PYTHON_VERSION)

if(NOT Cjyx_REQUIRED_PYTHON_VERSION MATCHES "([0-9]+)\\.([0-9]+)\\.([0-9]+)")
  message(FATAL_ERROR "Cjyx_REQUIRED_PYTHON_VERSION [${Cjyx_REQUIRED_PYTHON_VERSION}] is invalid. Value expected to be formatted as X.Y.Z")
endif()

set(Cjyx_REQUIRED_PYTHON_VERSION_MAJOR ${CMAKE_MATCH_1})
set(Cjyx_REQUIRED_PYTHON_VERSION_MINOR ${CMAKE_MATCH_2})
set(Cjyx_REQUIRED_PYTHON_VERSION_PATCH ${CMAKE_MATCH_3})

set(Cjyx_REQUIRED_PYTHON_VERSION_DOT ${Cjyx_REQUIRED_PYTHON_VERSION_MAJOR}.${Cjyx_REQUIRED_PYTHON_VERSION_MINOR})

set(Cjyx_REQUIRED_PYTHON_ABIFLAGS "")
if(Cjyx_REQUIRED_PYTHON_VERSION_DOT VERSION_LESS "3.8")
  set(Cjyx_REQUIRED_PYTHON_ABIFLAGS "m")
endif()

#-----------------------------------------------------------------------------
# Build shared lib by default
#------------------------------------------------------------------------------
# option(BUILD_SHARED_LIBS "Build Cjyx with shared libraries." ON)
set(BUILD_SHARED_LIBS ON)
mark_as_superbuild(BUILD_SHARED_LIBS:BOOL)
set(Cjyx_BUILD_SHARED ${BUILD_SHARED_LIBS})

#-----------------------------------------------------------------------------
# Cjyx application options
#-----------------------------------------------------------------------------
include(CjyxApplicationOptions)

#-----------------------------------------------------------------------------
# Append the library version information to the library target properties.
#------------------------------------------------------------------------------
option(Cjyx_WITH_LIBRARY_VERSION "Build with library version information" OFF)
mark_as_advanced(Cjyx_WITH_LIBRARY_VERSION)
mark_as_superbuild(Cjyx_WITH_LIBRARY_VERSION)
if(Cjyx_WITH_LIBRARY_VERSION)
  set(Cjyx_LIBRARY_PROPERTIES ${Cjyx_LIBRARY_PROPERTIES}
    VERSION ${Cjyx_VERSION_FULL}
    SOVERSION ${Cjyx_VERSION}
  )
endif()

#-----------------------------------------------------------------------------
# General Cjyx Options
#-----------------------------------------------------------------------------
include(CMakeDependentOption)

option(BUILD_TESTING "Test the project" ON)
mark_as_superbuild(BUILD_TESTING)

#option(WITH_MEMCHECK "Run tests through valgrind." OFF)
#mark_as_superbuild(WITH_MEMCHECK)

option(WITH_COVERAGE "Enable/Disable coverage" OFF)
mark_as_superbuild(WITH_COVERAGE)

option(Cjyx_USE_VTK_DEBUG_LEAKS "Enable VTKs Debug Leaks functionality in both VTK and Cjyx." ON)
mark_as_superbuild(Cjyx_USE_VTK_DEBUG_LEAKS:BOOL)
set(VTK_DEBUG_LEAKS ${Cjyx_USE_VTK_DEBUG_LEAKS})

option(Cjyx_BUILD_DICOM_SUPPORT "Build Cjyx with DICOM support" ON)
mark_as_superbuild(Cjyx_BUILD_DICOM_SUPPORT)

option(Cjyx_BUILD_DIFFUSION_SUPPORT "Build Cjyx with diffusion (DWI, DTI) support" ON)
mark_as_superbuild(Cjyx_BUILD_DIFFUSION_SUPPORT)

option(Cjyx_BUILD_I18N_SUPPORT "Build Cjyx with Internationalization support" ON)
mark_as_superbuild(Cjyx_BUILD_I18N_SUPPORT)

option(Cjyx_BUILD_WEBENGINE_SUPPORT "Build Cjyx with Qt WebEngine support" ON)
mark_as_superbuild(Cjyx_BUILD_WEBENGINE_SUPPORT)

option(Cjyx_BUILD_QTLOADABLEMODULES "Build Cjyx Qt Loadable Modules" ON)
mark_as_advanced(Cjyx_BUILD_QTLOADABLEMODULES)
mark_as_superbuild(Cjyx_BUILD_QTLOADABLEMODULES)

option(Cjyx_USE_PYTHONQT "Integrate a Python-Qt interpreter into Cjyx." ON)
mark_as_superbuild(Cjyx_USE_PYTHONQT)

CMAKE_DEPENDENT_OPTION(
  Cjyx_BUILD_QTSCRIPTEDMODULES "Build Cjyx Python Qt Modules" ON
  "Cjyx_USE_PYTHONQT" OFF)
mark_as_advanced(Cjyx_BUILD_QTSCRIPTEDMODULES)
mark_as_superbuild(Cjyx_BUILD_QTSCRIPTEDMODULES)

option(Cjyx_BUILD_CLI_SUPPORT "Build Cjyx with CLI support" ON)
mark_as_superbuild(Cjyx_BUILD_CLI_SUPPORT)
CMAKE_DEPENDENT_OPTION(
  Cjyx_BUILD_CLI "Build Cjyx CLI Plugins" ON
  "Cjyx_BUILD_CLI_SUPPORT" OFF
  )

mark_as_superbuild(Cjyx_BUILD_CLI)
CMAKE_DEPENDENT_OPTION(
  Cjyx_BUILD_LEGACY_CLI "Build Cjyx LEGACY_CLI Plugins" ON
  "Cjyx_BUILD_CLI" OFF
  )

option(Cjyx_BUILD_QT_DESIGNER_PLUGINS "Build Qt designer plugins" ON)
mark_as_superbuild(Cjyx_BUILD_QT_DESIGNER_PLUGINS)

option(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT "Build Cjyx extensions manager" ON)
mark_as_superbuild(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)

option(Cjyx_BUILD_MULTIMEDIA_SUPPORT "Build Cjyx with Multimedia support" ON)
mark_as_superbuild(Cjyx_BUILD_MULTIMEDIA_SUPPORT)

CMAKE_DEPENDENT_OPTION(Cjyx_UPDATE_TRANSLATION "update translation" OFF "Cjyx_BUILD_I18N_SUPPORT" OFF)
mark_as_advanced(Cjyx_UPDATE_TRANSLATION)
mark_as_superbuild(Cjyx_UPDATE_TRANSLATION)

set(Cjyx_FORCED_WC_LAST_CHANGED_DATE "" CACHE STRING "Overwrite value of auto-discovered Cjyx_WC_LAST_CHANGED_DATE (format: YYYY-MM-DD)")
mark_as_advanced(Cjyx_FORCED_WC_LAST_CHANGED_DATE)
mark_as_superbuild(Cjyx_FORCED_WC_LAST_CHANGED_DATE)

set(Cjyx_FORCED_REVISION "" CACHE STRING "Overwrite value of automatically set Cjyx_REVISION")
mark_as_advanced(Cjyx_FORCED_REVISION)
mark_as_superbuild(Cjyx_FORCED_REVISION)

set(Cjyx_REVISION_TYPE "CommitCount" CACHE STRING "How to set Cjyx_REVISION - from commit count or git hash." FORCE)
set_property(CACHE Cjyx_REVISION_TYPE PROPERTY STRINGS
  "CommitCount"
  "Hash"
  )
mark_as_advanced(Cjyx_REVISION_TYPE)
mark_as_superbuild(Cjyx_REVISION_TYPE)

# Default Cjyx_WC_COMMIT_COUNT_OFFSET is chosen to provide continuity of revisions when switching from SVN to git
# at SVN revision=28825 / git hash 47deb76d7556e40de4e25e585c4b24a63a153da5 in official Cjyx repository
# (https://github.com/Slicer/Slicer.git).
set(Cjyx_WC_COMMIT_COUNT_OFFSET "3037" CACHE STRING
  "This value is added to commit count to compute Cjyx_COMMIT_COUNT (that may be used to set Cjyx_REVISION).")
mark_as_advanced(Cjyx_WC_COMMIT_COUNT_OFFSET)
mark_as_superbuild(Cjyx_WC_COMMIT_COUNT_OFFSET)

option(Cjyx_USE_FOLDERS "Organize build targets into folders" ON)
mark_as_superbuild(Cjyx_USE_FOLDERS)
mark_as_advanced(Cjyx_USE_FOLDERS)

#-----------------------------------------------------------------------------
# Cjyx version number
#-----------------------------------------------------------------------------
include(CjyxVersion)

#-----------------------------------------------------------------------------
# External projects related options
#-----------------------------------------------------------------------------
CMAKE_DEPENDENT_OPTION(
  Cjyx_USE_PYTHONQT_WITH_OPENSSL "Enable PythonQt SSL support" ON
  "Cjyx_USE_PYTHONQT" OFF)
mark_as_superbuild(Cjyx_USE_PYTHONQT_WITH_OPENSSL)

CMAKE_DEPENDENT_OPTION(
  Cjyx_USE_NUMPY "Build Cjyx with NumPy" ON
  "Cjyx_USE_PYTHONQT" OFF)
mark_as_superbuild(Cjyx_USE_NUMPY)

CMAKE_DEPENDENT_OPTION(
  Cjyx_USE_SCIPY "Build Cjyx with SciPy" ON
  "Cjyx_USE_PYTHONQT;Cjyx_USE_NUMPY" OFF)
mark_as_superbuild(Cjyx_USE_SCIPY)

option(Cjyx_USE_CTKAPPLAUNCHER "Configure ctkAppLauncher." ON)
mark_as_advanced(Cjyx_USE_CTKAPPLAUNCHER)
mark_as_superbuild(Cjyx_USE_CTKAPPLAUNCHER)

option(Cjyx_USE_QtTesting    "Integrate QtTesting framework into Cjyx." ON)
mark_as_advanced(Cjyx_USE_QtTesting)
mark_as_superbuild(Cjyx_USE_QtTesting)

option(Cjyx_USE_SimpleITK "Build Cjyx with SimpleITK support" ON)
mark_as_superbuild(Cjyx_USE_SimpleITK)

option(Cjyx_BUILD_ITKPython "Build Python wrapping for ITK" OFF)
mark_as_superbuild(Cjyx_BUILD_ITKPython)
CMAKE_DEPENDENT_OPTION(
  Cjyx_INSTALL_ITKPython "Install Cjyx with Python wrapping for ITK" OFF
  "Cjyx_BUILD_ITKPython" OFF)
mark_as_superbuild(Cjyx_INSTALL_ITKPython)

option(Cjyx_BUILD_PARAMETERSERIALIZER_SUPPORT "Build Cjyx with parameter serializer support" OFF)
mark_as_superbuild(Cjyx_BUILD_PARAMETERSERIALIZER_SUPPORT)

set(_default_vtk "9")
set(Cjyx_VTK_VERSION_MAJOR ${_default_vtk} CACHE STRING "VTK major version (9)")
set_property(CACHE Cjyx_VTK_VERSION_MAJOR PROPERTY STRINGS "9")
if(NOT "${Cjyx_VTK_VERSION_MAJOR}" MATCHES "^(9)$")
  message(FATAL_ERROR "error: Cjyx_VTK_VERSION_MAJOR must be 9.")
endif()
mark_as_superbuild(Cjyx_VTK_VERSION_MAJOR)

#
# SimpleITK has large internal libraries, which take an extremely long
# time to link on windows when they are static. Creating shared
# SimpleITK internal libraries can reduce linking time. Also the size
# of the debug libraries are monstrous. Using shared libraries for
# debug, reduce disc requirements, and can improve linking
# times. However, these shared libraries take longer to load than the
# monolithic target from static libraries.
#
set( Cjyx_USE_SimpleITK_SHARED_DEFAULT OFF)
string(TOUPPER "${CMAKE_BUILD_TYPE}" _CMAKE_BUILD_TYPE)
if(MSVC OR _CMAKE_BUILD_TYPE MATCHES "DEBUG")
  set(Cjyx_USE_SimpleITK_SHARED_DEFAULT ON)
endif()
CMAKE_DEPENDENT_OPTION(Cjyx_USE_SimpleITK_SHARED "Build SimpleITK with shared libraries. Reduces linking time, increases run-time load time." ${Cjyx_USE_SimpleITK_SHARED_DEFAULT} "Cjyx_USE_SimpleITK" OFF )
mark_as_superbuild(Cjyx_USE_SimpleITK_SHARED)

#-----------------------------------------------------------------------------
# Install no development files by default, but allow the user to get
# them installed by setting Cjyx_INSTALL_DEVELOPMENT to true.
#-----------------------------------------------------------------------------
option(Cjyx_INSTALL_DEVELOPMENT "Install Cjyx extension development files." OFF)
mark_as_advanced(Cjyx_INSTALL_DEVELOPMENT)

if(NOT Cjyx_INSTALL_DEVELOPMENT)
  set(Cjyx_INSTALL_NO_DEVELOPMENT 1)
else()
  set(Cjyx_INSTALL_NO_DEVELOPMENT 0)
endif()

#-----------------------------------------------------------------------------
# Documentation
#-----------------------------------------------------------------------------
option(Cjyx_BUILD_DOCUMENTATION "Build documentation (Doxygen, sphinx, ...)" ON)
mark_as_advanced(Cjyx_BUILD_DOCUMENTATION)
mark_as_superbuild(Cjyx_BUILD_DOCUMENTATION)

if(Cjyx_BUILD_DOCUMENTATION)
  find_package(Doxygen QUIET)

  if(DOXYGEN_DOT_FOUND)
    mark_as_superbuild(DOXYGEN_EXECUTABLE)

    set(DOCUMENTATION_ARCHIVES_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
     CACHE PATH "Where documentation archives should be stored")
    mark_as_advanced(DOCUMENTATION_ARCHIVES_OUTPUT_DIRECTORY)
    mark_as_superbuild(DOCUMENTATION_ARCHIVES_OUTPUT_DIRECTORY)

    option(DOCUMENTATION_TARGET_IN_ALL "Include the custom target for building documentation in 'all'" OFF)
    mark_as_advanced(DOCUMENTATION_TARGET_IN_ALL)
    mark_as_superbuild(DOCUMENTATION_TARGET_IN_ALL)
  endif()
endif()

#-----------------------------------------------------------------------------
# CTestCustom
#-----------------------------------------------------------------------------
configure_file(
  CMake/CTestCustom.cmake.in
  ${CMAKE_BINARY_DIR}/CTestCustom.cmake
  COPYONLY
  )

#-----------------------------------------------------------------------------
# Additional CXX/C Flags
#-----------------------------------------------------------------------------
set(ADDITIONAL_C_FLAGS "" CACHE STRING "Additional C Flags")
mark_as_advanced(ADDITIONAL_C_FLAGS)
set(ADDITIONAL_CXX_FLAGS "" CACHE STRING "Additional CXX Flags")
mark_as_advanced(ADDITIONAL_CXX_FLAGS)

#-----------------------------------------------------------------------------
# CMake Function(s) and Macro(s)
#-----------------------------------------------------------------------------
include(CMakeParseArguments)
include(CjyxMacroBuildScriptedCLI)
include(CjyxMacroBuildScriptedModule)
include(CjyxMacroGetOperatingSystemArchitectureBitness)
if(Cjyx_BUILD_I18N_SUPPORT)
  include(CjyxMacroTranslation)
endif()
include(CjyxFunctionInstallLibrary)
include(CjyxFunctionAddPythonQtResources)

#-----------------------------------------------------------------------------
# Internationalization
#-----------------------------------------------------------------------------
if(Cjyx_BUILD_I18N_SUPPORT)
  set(Cjyx_LANGUAGES
    "fr"
    CACHE STRING "Semicolon separated list of supported Cjyx languages. Expected format : language[_country] (both en and en_US are supported)."
    )
  mark_as_advanced(Cjyx_LANGUAGES)
  mark_as_superbuild(Cjyx_LANGUAGES)
endif()

#-----------------------------------------------------------------------------
# Set Cjyx_{C,CXX}_REQUIRED_FLAGS variables
#-----------------------------------------------------------------------------
include(CjyxBlockCXXRequiredFlags)

#-----------------------------------------------------------------------------
# Cjyx directories
#-----------------------------------------------------------------------------
include(CjyxDirectories)

# CjyxExecutionModel output directories
if(Cjyx_SUPERBUILD)
  set(_sem_output_dir ${CMAKE_BINARY_DIR}/${Cjyx_BINARY_INNER_SUBDIR})
else()
  set(_sem_output_dir ${CMAKE_BINARY_DIR})
endif()
if(NOT DEFINED CjyxExecutionModel_DEFAULT_CLI_RUNTIME_OUTPUT_DIRECTORY)
  set(CjyxExecutionModel_DEFAULT_CLI_RUNTIME_OUTPUT_DIRECTORY ${_sem_output_dir}/${Cjyx_CLIMODULES_BIN_DIR})
endif()
if(NOT DEFINED CjyxExecutionModel_DEFAULT_CLI_LIBRARY_OUTPUT_DIRECTORY)
  set(CjyxExecutionModel_DEFAULT_CLI_LIBRARY_OUTPUT_DIRECTORY ${_sem_output_dir}/${Cjyx_CLIMODULES_LIB_DIR})
endif()
if(NOT DEFINED CjyxExecutionModel_DEFAULT_CLI_ARCHIVE_OUTPUT_DIRECTORY)
  set(CjyxExecutionModel_DEFAULT_CLI_ARCHIVE_OUTPUT_DIRECTORY ${_sem_output_dir}/${Cjyx_CLIMODULES_LIB_DIR})
endif()


#-----------------------------------------------------------------------------
# Extension directories
#-----------------------------------------------------------------------------
# NOTE: Make sure to update vtkCjyxApplicationLogic::IsEmbeddedModule if
#       the following variables are changed.
set(Cjyx_EXTENSIONS_DIRBASENAME "Extensions")
set(Cjyx_EXTENSIONS_DIRNAME "${Cjyx_EXTENSIONS_DIRBASENAME}-${Cjyx_REVISION}")

#-----------------------------------------------------------------------------
# Cjyx OpenGL Options
#-----------------------------------------------------------------------------
if(UNIX AND NOT APPLE)
  set(OpenGL_GL_PREFERENCE "LEGACY")
  mark_as_superbuild(OpenGL_GL_PREFERENCE:STRING)
endif()

#-----------------------------------------------------------------------------
# Cjyx VTK Options
#-----------------------------------------------------------------------------
set(_backend "OpenGL2")
set(Cjyx_VTK_RENDERING_BACKEND "${_backend}" CACHE STRING "Rendering backend." FORCE)
mark_as_superbuild(Cjyx_VTK_RENDERING_BACKEND)
set(Cjyx_VTK_RENDERING_USE_${Cjyx_VTK_RENDERING_BACKEND}_BACKEND 1)

# Cjyx build is only tested with Sequential and TBB. OpenMP might work.
# Use TBB by default only on Windows, as it has not been tested on other platforms.
if(WIN32)
  set(Cjyx_DEFAULT_VTK_SMP_IMPLEMENTATION_TYPE "TBB")
else()
  set(Cjyx_DEFAULT_VTK_SMP_IMPLEMENTATION_TYPE "Sequential")
endif()
set(Cjyx_VTK_SMP_IMPLEMENTATION_TYPE ${Cjyx_DEFAULT_VTK_SMP_IMPLEMENTATION_TYPE}
  CACHE STRING "Which multi-threaded parallelism implementation to use in VTK. Options are Sequential or TBB.")
set_property(CACHE Cjyx_VTK_SMP_IMPLEMENTATION_TYPE
  PROPERTY
    STRINGS Sequential TBB)
mark_as_superbuild(Cjyx_VTK_SMP_IMPLEMENTATION_TYPE)
if(${Cjyx_VTK_SMP_IMPLEMENTATION_TYPE} STREQUAL "TBB")
  set(Cjyx_USE_TBB TRUE)
else()
  set(Cjyx_USE_TBB FALSE)
endif()

message(STATUS "Configuring VTK")
message(STATUS "  Cjyx_VTK_RENDERING_BACKEND is ${Cjyx_VTK_RENDERING_BACKEND}")
message(STATUS "  Cjyx_VTK_SMP_IMPLEMENTATION_TYPE is ${Cjyx_VTK_SMP_IMPLEMENTATION_TYPE}")
message(STATUS "  Cjyx_VTK_VERSION_MAJOR is ${Cjyx_VTK_VERSION_MAJOR}")

#-----------------------------------------------------------------------------
# Cjyx_VTK_COMPONENTS
#-----------------------------------------------------------------------------
set(Cjyx_VTK_COMPONENTS
  FiltersExtraction
  FiltersFlowPaths
  FiltersGeometry
  FiltersParallel
  GUISupportQtSQL
  IOExport
  IOImage
  IOLegacy
  IOPLY
  IOXML
  ImagingMath
  ImagingMorphological
  ImagingStatistics
  ImagingStencil
  InteractionImage
  RenderingContext${Cjyx_VTK_RENDERING_BACKEND}
  RenderingQt
  RenderingVolume${Cjyx_VTK_RENDERING_BACKEND}
  TestingRendering
  ViewsQt
  zlib
  )

if(Cjyx_USE_PYTHONQT)
  list(APPEND Cjyx_VTK_COMPONENTS
    WrappingPythonCore
    )
endif()

#-----------------------------------------------------------------------------
# Qt - Cjyx_REQUIRED_QT_MODULES
#-----------------------------------------------------------------------------
# Module name should be specified as they appear in FindQt5.cmake.
# For example, the module name associated with the variable QT_USE_QTXML is QTXML.
# Note that the modules will be installed when packaging.
  set(Cjyx_REQUIRED_QT_MODULES
    Core Widgets
    Network OpenGL
    PrintSupport # Required by "Annotations" module
    UiTools #no dll
    Xml XmlPatterns
    Svg Sql
    )

  if(Cjyx_BUILD_MULTIMEDIA_SUPPORT)
    list(APPEND Cjyx_REQUIRED_QT_MODULES
      Multimedia
      MultimediaWidgets
      )
  endif()

  # Add the Qt5X11Extras component for Linux systems
  # This component is needed for building VTK
  if(UNIX AND NOT APPLE)
    list(APPEND Cjyx_REQUIRED_QT_MODULES X11Extras)
  endif()

  find_package(Qt5 COMPONENTS Core QUIET)
  if(Cjyx_BUILD_WEBENGINE_SUPPORT)
    list(APPEND Cjyx_REQUIRED_QT_MODULES
      WebEngine
      WebEngineWidgets
      WebChannel
      )
  endif()
  if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
    list(APPEND Cjyx_REQUIRED_QT_MODULES Script)
  endif()
  if(Cjyx_BUILD_I18N_SUPPORT)
    list(APPEND Cjyx_REQUIRED_QT_MODULES LinguistTools) # no dll
  endif()
  if(BUILD_TESTING)
    list(APPEND Cjyx_REQUIRED_QT_MODULES Test)
  endif()
list(APPEND Cjyx_REQUIRED_QT_MODULES ${Cjyx_ADDITIONAL_REQUIRED_QT_MODULES})

  set(QT_LIBRARIES)
  foreach(lib IN LISTS Cjyx_REQUIRED_QT_MODULES)
    if(lib MATCHES "^(LinguistTools)$")
      continue()
    endif()
    list(APPEND QT_LIBRARIES Qt5::${lib})
  endforeach()

if(DEFINED Cjyx_ADDITIONAL_REQUIRED_QT_MODULES)
  mark_as_superbuild(Cjyx_ADDITIONAL_REQUIRED_QT_MODULES:STRING)
endif()

mark_as_superbuild(Cjyx_USE_SYSTEM_QT:BOOL)

#-----------------------------------------------------------------------------
# Qt plugins (designer, imageformats, ...) relative directories
#-----------------------------------------------------------------------------
set(Cjyx_QtPlugins_DIR "lib/QtPlugins")
set(Cjyx_INSTALL_QtPlugins_DIR "${Cjyx_INSTALL_ROOT}${Cjyx_QtPlugins_DIR}")

#-----------------------------------------------------------------------------
# Qt
#-----------------------------------------------------------------------------
include(CjyxBlockFindQtAndCheckVersion)
  mark_as_superbuild(VARS Qt5_DIR LABELS "FIND_PACKAGE")

set(Cjyx_HAVE_QT5 1)
    set(Cjyx_HAVE_WEBKIT_SUPPORT 0)

#
# If qmake or Qt5Config are associated with a system location, explicitly mark Qt as such. Doing so
# will prevent system path from being prepended to PATH or (DY)LD_LIBRARY_PATH
# when generating the launcher settings and avoid system libraries symbols from
# conflicting with Cjyx version of these libraries.
#
# See https://issues.slicer.org/view.php?id=3574
#
  foreach(_path IN ITEMS
      "/usr/lib/"
      "/usr/lib32/"
      "/usr/lib64/"
      "/usr/local/lib/"
      # homebrew
      "/usr/local/Cellar/lib/"
      # macport
      "/opt/lib"
      "/opt/local/lib"
      )
    if("${Qt5_DIR}" MATCHES "^${_path}")
      set(Cjyx_USE_SYSTEM_QT ON)
      message(STATUS "")
      message(STATUS "Forcing Cjyx_USE_SYSTEM_QT to ON (Qt5_DIR [${Qt5_DIR}] associated with a system location: ${_path})")
      message(STATUS "")
      break()
    endif()
  endforeach()

# Always use QVTKOpenGLWidget (instead of QVTKWidget)
set(Cjyx_VTK_USE_QVTKOPENGLWIDGET 1)

#-----------------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------------
# NOTE: For CTEST_USE_LAUNCHER to work, it's required to include CTest at the superbuild level.
#       See https://www.kitware.com/blog/home/post/11

set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1) # Do not add CTest default targets
include(CTest)

# Add Experimental target - Adapted from CTestTargets.cmake
set(__conf_types "")
if(CMAKE_CONFIGURATION_TYPES)
  # We need to pass the configuration type on the test command line.
  set(__conf_types -C "${CMAKE_CFG_INTDIR}")
endif()
add_custom_target(Experimental ${CMAKE_CTEST_COMMAND} ${__conf_types} -D Experimental)

if(Cjyx_SUPERBUILD)
  set(_cjyx_dir ${CMAKE_BINARY_DIR}/${Cjyx_BINARY_INNER_SUBDIR})
else()
  set(_cjyx_dir ${Cjyx_BINARY_DIR})
endif()
set(Cjyx_LAUNCHER_EXECUTABLE ${_cjyx_dir}/${Cjyx_MAIN_PROJECT_APPLICATION_NAME}${CMAKE_EXECUTABLE_SUFFIX})
set(Cjyx_LAUNCH_COMMAND ${Cjyx_LAUNCHER_EXECUTABLE} --launch)

#-----------------------------------------------------------------------------
# ExternalData Object Stores configuration
#-----------------------------------------------------------------------------
set(ExternalData_OBJECT_STORES_DEFAULT "${Cjyx_BINARY_DIR}/ExternalData/Objects")
if(DEFINED "ENV{ExternalData_OBJECT_STORES}")
  set(ExternalData_OBJECT_STORES_DEFAULT $ENV{ExternalData_OBJECT_STORES})
endif()
if(NOT DEFINED ExternalData_OBJECT_STORES)
  set(ExternalData_OBJECT_STORES "${ExternalData_OBJECT_STORES_DEFAULT}")
endif()
message(STATUS "Setting ExternalData_OBJECT_STORES to '${ExternalData_OBJECT_STORES}'")

set(Cjyx_ExternalData_DATA_MANAGEMENT_TARGET ${PROJECT_NAME}Data)

#-----------------------------------------------------------------------------
# Get operating system, architecture and bitness
#-----------------------------------------------------------------------------
CjyxMacroGetOperatingSystemArchitectureBitness(VAR_PREFIX Cjyx)
message(STATUS "Configuring ${Cjyx_MAIN_PROJECT_APPLICATION_NAME} for [${Cjyx_OS}-${Cjyx_ARCHITECTURE}]")

#-----------------------------------------------------------------------------
# Superbuild script
#-----------------------------------------------------------------------------
if(Cjyx_SUPERBUILD)
  include("${CMAKE_CURRENT_SOURCE_DIR}/SuperBuild.cmake")
  return()
endif()

foreach(dep QT ${Cjyx_DEPENDENCIES})
  if(Cjyx_USE_SYSTEM_${dep})
    message(STATUS "Using system ${dep}")
  endif()
endforeach()

#-----------------------------------------------------------------------------
# Update CMake module path
#------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  ${vtkAddon_CMAKE_DIR}
  ${CMAKE_MODULE_PATH}
  )

#-----------------------------------------------------------------------------
# Folders
#-----------------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ${Cjyx_USE_FOLDERS})

#-----------------------------------------------------------------------------
# Clear CjyxTargets.cmake
#-----------------------------------------------------------------------------
file(WRITE "${Cjyx_BINARY_DIR}/CjyxTargets.cmake" "")

#-----------------------------------------------------------------------------
# Output directories.
#------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Cjyx_BINARY_DIR}/${Cjyx_BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Cjyx_BINARY_DIR}/${Cjyx_BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Cjyx_BINARY_DIR}/${Cjyx_LIB_DIR})

#-----------------------------------------------------------------------------
# Cjyx Home
#------------------------------------------------------------------------------
set(Cjyx_HOME "${Cjyx_BINARY_DIR}")

#-----------------------------------------------------------------------------
# Cjyx include and libraries subdirectory
#-----------------------------------------------------------------------------
set(Cjyx_Base_LIBRARIES CACHE INTERNAL "Cjyx Base libraries" FORCE)
set(Cjyx_Base_INCLUDE_DIRS CACHE INTERNAL "Cjyx Base includes" FORCE)
set(Cjyx_ModuleLogic_INCLUDE_DIRS CACHE INTERNAL "Cjyx Module logic includes" FORCE)
set(Cjyx_ModuleMRML_INCLUDE_DIRS CACHE INTERNAL "Cjyx Module MRML includes" FORCE)
set(Cjyx_ModuleWidgets_INCLUDE_DIRS CACHE INTERNAL "Cjyx Module Widgets includes" FORCE)

#-----------------------------------------------------------------------------
# ExternalData configuration for test data
#-----------------------------------------------------------------------------
list(APPEND ExternalData_URL_TEMPLATES "https://github.com/Slicer/SlicerTestingData/releases/download/%(algo)/%(hash)")
include(ExternalData)
set(Cjyx_ExternalData_OBJECT_STORES ${ExternalData_OBJECT_STORES})
set(Cjyx_ExternalData_URL_TEMPLATES ${ExternalData_URL_TEMPLATES})

#-----------------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------------
include(CjyxMacroConfigureModuleCxxTestDriver)
include(CjyxMacroSimpleTest)
include(CjyxMacroPythonTesting)
include(CjyxMacroConfigureGenericCxxModuleTests)
include(CjyxMacroConfigureGenericPythonModuleTests)

if(BUILD_TESTING)
  # The variable CXX_TEST_PATH should *NOT* be used when adding tests.
  # Instead the the $<TARGET_FILE:...> generator expression should be
  # considered. See r16586 for more details.
  set(CXX_TEST_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # XXX

  configure_file(
    ${Cjyx_SOURCE_DIR}/CTestConfig.cmake
    ${Cjyx_BINARY_DIR}/${Cjyx_BIN_DIR}/CTestConfig.cmake
    COPYONLY)

  add_subdirectory(CMake/Testing)

  if(Cjyx_BUILD_CLI_SUPPORT)
    set(SEM_LAUNCH_COMMAND ${Cjyx_LAUNCH_COMMAND})
    set(SEM_DATA_MANAGEMENT_TARGET ${Cjyx_ExternalData_DATA_MANAGEMENT_TARGET})
  endif()
endif()

#-----------------------------------------------------------------------------
# NOTE: How to include dependent packages ?
#-----------------------------------------------------------------------------

# Every Cjyx sub project, library or module is responsible
# to call find_package (optionally specifying a list of components)
# and (if it applies) include the associated "use file".
#
# This means that moving forward most of the find_package calls listed below
# will be removed.

#-----------------------------------------------------------------------------
# VTK
#-----------------------------------------------------------------------------
find_package(VTK REQUIRED)
set(VTK_LIBRARIES "")
find_package(VTK ${Cjyx_VTK_VERSION_MAJOR} COMPONENTS ${Cjyx_VTK_COMPONENTS} REQUIRED)
set(VTK_GUI_SUPPORT_QT_TARGET_NAME "VTK::GUISupportQt")

if(NOT TARGET ${VTK_GUI_SUPPORT_QT_TARGET_NAME})
  message(FATAL_ERROR "error: VTK was not configured to use Qt, you probably need "
                    "to recompile it with VTK_USE_GUISUPPORT ON, VTK_Group_Qt ON, "
                    "Note that Qt >= ${Cjyx_REQUIRED_QT_VERSION} is *required*")
endif()
set(VTK_RENDERING_BACKEND "${Cjyx_VTK_RENDERING_BACKEND}")

#-----------------------------------------------------------------------------
# CTK
#-----------------------------------------------------------------------------
find_package(CTK REQUIRED)
include(${CTK_USE_FILE})

#-----------------------------------------------------------------------------
# Update CMake module path
#-----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  ${CTK_CMAKE_DIR}
  ${CMAKE_MODULE_PATH}
  ${CTK_CMAKE_UTILITIES_DIR})

#-----------------------------------------------------------------------------
# CMake Function(s) and Macro(s) from CTK
#-----------------------------------------------------------------------------
include(ctkFunctionAddExecutableUtf8)

#-----------------------------------------------------------------------------
# Set Cjyx_PYTHON_VERSION and Cjyx_PYTHON_VERSION_DOT
#-----------------------------------------------------------------------------
if(Cjyx_USE_PYTHONQT)
  find_package(PythonLibs ${Cjyx_REQUIRED_PYTHON_VERSION_DOT} REQUIRED)
  find_package(PythonInterp ${Cjyx_REQUIRED_PYTHON_VERSION_DOT} REQUIRED)

  # Extract python lib path
  get_filename_component(PYTHON_DIR_PATH ${PYTHON_EXECUTABLE} PATH)
  set(PYTHON_LIBRARY_PATH ${PYTHON_DIR_PATH}/../lib)
  if(WIN32)
    set(PYTHON_LIBRARY_PATH ${PYTHON_DIR_PATH})
  endif()

  # Extract python version components
  ctkMacroSetPaths("${PYTHON_LIBRARY_PATH}")
  function(_python_sys_attribute attribute varname)
    execute_process(
      COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print(sys.${attribute})"
      OUTPUT_VARIABLE value
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(${varname} ${value} PARENT_SCOPE)
  endfunction()

  _python_sys_attribute("version_info.major" Cjyx_PYTHON_VERSION_MAJOR)
  _python_sys_attribute("version_info.minor" Cjyx_PYTHON_VERSION_MINOR)
  _python_sys_attribute("version_info.micro" Cjyx_PYTHON_VERSION_PATCH)
  _python_sys_attribute("abiflags" Cjyx_PYTHON_ABIFLAGS)

  set(Cjyx_PYTHON_VERSION_DOT "${Cjyx_PYTHON_VERSION_MAJOR}.${Cjyx_PYTHON_VERSION_MINOR}")
  set(Cjyx_PYTHON_VERSION "${Cjyx_PYTHON_VERSION_MAJOR}${Cjyx_PYTHON_VERSION_MINOR}")

  message(STATUS "Configuring ${Cjyx_MAIN_PROJECT_APPLICATION_NAME} with python ${Cjyx_PYTHON_VERSION_DOT}${Cjyx_PYTHON_ABIFLAGS}")

  set(CTK_COMPILE_PYTHON_SCRIPTS_GLOBAL_TARGET_NAME "Cjyx")
endif()

#-----------------------------------------------------------------------------
# PythonQt
#-----------------------------------------------------------------------------
if(Cjyx_USE_PYTHONQT)
  # Find PythonQt package so that PYTHONQT_USE_RELEASE_PYTHON_FALLBACK is defined
  # See https://github.com/commontk/PythonQt/issues/7
  # and https://github.com/commontk/PythonQt/issues/8
  find_package(PythonQt REQUIRED)
  link_directories(${PYTHONQT_INSTALL_DIR}/lib)
  include(ctkMacroWrapPythonQt)
  include(ctkMacroCompilePythonScript)

  # Enable qMRMLWidgets python wrapping
  set(MRMLWidgets_WRAP_PYTHON ON)

  # See vtkAddon/CMake/vtkMacroKitPythonWrap.cmake
  set(Cjyx_VTK_WRAP_HIERARCHY_DIR ${Cjyx_BINARY_DIR})
endif()

#-----------------------------------------------------------------------------
# Teem
#-----------------------------------------------------------------------------
# XXX Waiting teem provide a correct TeemConfig.cmake exporting targets, this is needed
#     to ensure the link_directories for teem is defined and avoid error like:
#     warning: libteem.so.1, needed by ../../../../bin/libvtkTeem.so, may conflict with libteem.so.2
find_package(Teem REQUIRED)
include(${Teem_USE_FILE})

#-----------------------------------------------------------------------------
# CjyxExecutionModel settings
#-----------------------------------------------------------------------------
set(CjyxExecutionModel_EXTRA_INCLUDE_DIRECTORIES "" CACHE INTERNAL "CjyxExecutionModel extra includes" FORCE)
set(CjyxExecutionModel_EXTRA_EXECUTABLE_TARGET_LIBRARIES "" CACHE INTERNAL "CjyxExecutionModel extra executable target libraries" FORCE)

#-----------------------------------------------------------------------------
# Set Cjyx builtin libraries *_DIR variables
#-----------------------------------------------------------------------------
set(GenerateLM_DIR ${Cjyx_BINARY_DIR}/Libs/GenerateLM)
set(vtkITK_DIR ${Cjyx_BINARY_DIR}/Libs/vtkITK)

#-----------------------------------------------------------------------------
# Set COVERAGE_{C,CXX}_FLAGS variables
#-----------------------------------------------------------------------------
include(CjyxBlockCXXCoverageFlags)

#-----------------------------------------------------------------------------
# Set ITK_REQUIRED_{C,CXX}_FLAGS variables
#-----------------------------------------------------------------------------
include(ITKPlatformSpecificChecks)

#-----------------------------------------------------------------------------
# Set CMAKE_{C,CXX}_FLAGS variables
#-----------------------------------------------------------------------------
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_INIT} ${Cjyx_REQUIRED_C_FLAGS} ${ITK_REQUIRED_C_FLAGS} ${COVERAGE_C_FLAGS} ${ADDITIONAL_C_FLAGS}" CACHE STRING "CMake C Flags" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} ${Cjyx_REQUIRED_CXX_FLAGS} ${ITK_REQUIRED_CXX_FLAGS} ${COVERAGE_CXX_FLAGS} ${ADDITIONAL_CXX_FLAGS}" CACHE STRING "CMake CXX Flags" FORCE)

#-----------------------------------------------------------------------------
# Set the header template which defines custom export/import macros
# for shared libraries
#-----------------------------------------------------------------------------
set(QMRML_EXPORT_HEADER_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/Libs/MRML/Widgets/CMake/qMRMLExport.h.in")

#-----------------------------------------------------------------------------
# Configure testing scripts
#-----------------------------------------------------------------------------
set(files
  Utilities/Scripts/computeCodeCoverageLocally.sh
  Utilities/Scripts/computeCodeCoverageForOneTest.sh
  Utilities/Scripts/runExperimentalOffScreen.sh
  Utilities/Scripts/setupCjyxEnvironment.sh
  )
foreach(f ${files})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${f}.in
    ${CMAKE_CURRENT_BINARY_DIR}/${f}
    )
endforeach()

#-----------------------------------------------------------------------------
# Includes
#-----------------------------------------------------------------------------
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

#-----------------------------------------------------------------------------
# Subdirectories
#-----------------------------------------------------------------------------

add_subdirectory(Libs)
add_subdirectory(Base)
add_subdirectory(Resources)
add_subdirectory(Utilities/Scripts)
if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  add_subdirectory(Utilities/Scripts/CjyxWizard)
endif()

#-----------------------------------------------------------------------------
# Macro useful to build ModuleLogic, ModuleMRML and ModuleWidgets libraries
#-----------------------------------------------------------------------------
include(CjyxMacroBuildModuleQtLibrary)
include(CjyxMacroBuildModuleVTKLibrary)
include(CjyxMacroPythonWrapModuleVTKLibrary)
include(CjyxMacroBuildModuleLogic)
include(CjyxMacroBuildModuleMRML)
include(CjyxMacroBuildModuleWidgets)

#-----------------------------------------------------------------------------
# Cjyx Core and GUI library
#-----------------------------------------------------------------------------
set(Cjyx_CORE_LIBRARY qCjyxBaseQTCore)
set(Cjyx_GUI_LIBRARY qCjyxBaseQTApp)

#-----------------------------------------------------------------------------
# Applications
#-----------------------------------------------------------------------------
add_subdirectory(Modules/Core)
add_subdirectory(Base/QTApp)
add_subdirectory(Modules)
add_subdirectory(
  ${Cjyx_APPLICATIONS_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/Applications
  )

#-----------------------------------------------------------------------------
# Templates
#-----------------------------------------------------------------------------
if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  add_subdirectory(Utilities/Templates)
endif()

#-----------------------------------------------------------------------------
set(Cjyx_USE_FILE ${Cjyx_BINARY_DIR}/UseCjyx.cmake)
configure_file(
  ${Cjyx_SOURCE_DIR}/CMake/UseCjyx.cmake.in
  ${Cjyx_USE_FILE} COPYONLY)

configure_file(
  ${Cjyx_SOURCE_DIR}/CMake/CjyxConfigVersion.cmake.in
  ${Cjyx_BINARY_DIR}/CjyxConfigVersion.cmake @ONLY)

set(Cjyx_EXTENSION_GENERATE_CONFIG ${Cjyx_SOURCE_DIR}/CMake/CjyxExtensionGenerateConfig.cmake)
set(Cjyx_EXTENSION_CPACK ${Cjyx_SOURCE_DIR}/CMake/CjyxExtensionCPack.cmake)

# --------------------------------------------------------------------------
# Bundle remote modules and extensions adding source directories.
# --------------------------------------------------------------------------

set(extensions_build_dir "${Cjyx_BINARY_DIR}/E")

# Configure a no-op CjyxConfig and CjyxConfigVersion for bundled projects
set(Cjyx_DIR ${extensions_build_dir})
configure_file(
  ${Cjyx_SOURCE_DIR}/CMake/CjyxConfig.cmake.in
  ${Cjyx_DIR}/CjyxConfig.cmake @ONLY)

configure_file(
  ${Cjyx_SOURCE_DIR}/CMake/CjyxConfigVersion.cmake.in
  ${Cjyx_DIR}/CjyxConfigVersion.cmake @ONLY)

#
# Directories can be set in three ways:
#
# (1) Automatically by specifying the label 'REMOTE_MODULE' when
#     calling 'Cjyx_Remote_Add' from SuperBuild.cmake.
#
# (2) Automatically by specifying the label 'REMOTE_EXTENSION' when
#     calling 'Cjyx_Remote_Add' from SuperBuild.cmake.
#
# (3) Explicitly by configuring the project using the following syntax:
#  cmake -DCjyx_EXTENSION_SOURCE_DIRS:STRING=/path/to/ExtensionA;/path/to/ExtensionB /path/to/source/Cjyx
#

#
# Support for "SuperBuild-type" extension:
#
# * An extension is considered to be of type "SuperBuild" if a directory
#   "<extension_dir>/SuperBuild" or "<extension_dir>/Superbuild" exists.
#   Corresponding directory is appended to EXTERNAL_PROJECT_ADDITIONAL_DIRS.
#
# * If variable "<extension_name>_EXTERNAL_PROJECT_EXCLUDE_ALL" is set to TRUE, corresponding SuperBuild directory
#   is not appended to EXTERNAL_PROJECT_ADDITIONAL_DIRS.
#
# * Associated external projects are globbed using expression of the form
#   "<extension_dir>/(SuperBuild|Superbuild)/External_*.cmake".
#
# * List of external project names is extracted from the "External_<projectName>.cmake"
#   files and appended to Cjyx_DEPENDENCIES. This ensures they are build before Cjyx inner build.
#   Setting variable "<extension_name>_EXTERNAL_PROJECT_DEPENDENCIES" to a list of <projectName> allows
#   to override the list of <projectName> appended to Cjyx_DEPENDENCIES.
#
# * Variable Cjyx_BUNDLED_EXTENSION_NAMES is updated with the names of external project
#   and passed to Cjyx inner build. It is then used in CjyxCPack. to package associated
#   external projects if the cache variable <extensionName>_CPACK_INSTALL_CMAKE_PROJECTS
#   was defined in the extension sources.
#
# Corresponding logic is implemented in SuperBuild.cmake
#

function(_add_extension_source_dir extension_source_dir what)
  get_filename_component(extension_source_dir ${extension_source_dir} REALPATH)
  get_filename_component(extension_source_dirname ${extension_source_dir} NAME_WE)
  message(STATUS "--------------------------------------------------")
  message(STATUS "Configuring ${what}: ${extension_source_dirname}")
  set(ExternalData_SOURCE_ROOT ${extension_source_dir})
  set(${extension_source_dirname}_SOURCE_DIR ${extension_source_dir})
  set(${extension_source_dirname}_BINARY_DIR ${extensions_build_dir}/${extension_source_dirname})
  add_subdirectory(
    ${${extension_source_dirname}_SOURCE_DIR}
    ${${extension_source_dirname}_BINARY_DIR}
    )
endfunction()

foreach(varname ${Cjyx_EP_LABEL_REMOTE_MODULE})
  _add_extension_source_dir(${${varname}} "remote module")
endforeach()
foreach(varname ${Cjyx_EP_LABEL_REMOTE_EXTENSION})
  _add_extension_source_dir(${${varname}} "remote extension")
endforeach()
foreach(extension_source_dir ${Cjyx_EXTENSION_SOURCE_DIRS})
  _add_extension_source_dir(${extension_source_dir} "extension directory")
endforeach()

unset(Cjyx_DIR)

# --------------------------------------------------------------------------
# Recover the QM output directories
# --------------------------------------------------------------------------
get_property(Cjyx_QM_OUTPUT_DIRS GLOBAL PROPERTY Cjyx_QM_OUTPUT_DIRS)

# --------------------------------------------------------------------------
# Configure and install headers
# --------------------------------------------------------------------------
set(files
  vtkCjyxConfigure.h
  vtkCjyxVersionConfigureInternal.h
  )
foreach(f ${files})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/${f}.in
    ${CMAKE_CURRENT_BINARY_DIR}/${f}
    )
endforeach()

# Add target configuring 'vtkCjyxVersionConfigure.h' at build time.
include(CjyxConfigureVersionHeaderTarget)
add_dependencies(CjyxBaseLogic CjyxConfigureVersionHeader)
add_dependencies(MRMLCore CjyxConfigureVersionHeader)

list(APPEND files vtkCjyxVersionConfigure.h)

if(NOT Cjyx_INSTALL_NO_DEVELOPMENT)
foreach(f ${files})
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${f}
    DESTINATION ${Cjyx_INSTALL_INCLUDE_DIR} COMPONENT Development
    )
endforeach()
endif()

#-----------------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()

# Install testing data
if(NOT Cjyx_INSTALL_NO_DEVELOPMENT)
  install(DIRECTORY
    Testing
    DESTINATION ${Cjyx_INSTALL_SHARE_DIR} COMPONENT Development
    PATTERN "CMakeLists.txt" EXCLUDE
    )
endif()

#-----------------------------------------------------------------------------
# Dynamic analysis
#-----------------------------------------------------------------------------
set(Cjyx_VALGRIND_SUPPRESSIONS_FILE "${Cjyx_SOURCE_DIR}/CMake/CjyxValgrind.supp")
get_property(Cjyx_MAIN_PROJECT_APPLICATION_EXECUTABLE GLOBAL PROPERTY "${Cjyx_MAIN_PROJECT_APPLICATION_NAME}_EXECUTABLE")
set(script "Utilities/Scripts/runValgrind.sh")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/${script}.in
  ${CMAKE_CURRENT_BINARY_DIR}/${script}
  @ONLY
  )

#-----------------------------------------------------------------------------
# Documentation
#-----------------------------------------------------------------------------
if(Cjyx_BUILD_DOCUMENTATION)
  # Automatically generate documentation at build time
  set(all_arg)
  if(DOCUMENTATION_TARGET_IN_ALL)
    set(all_arg "ALL")
  endif()
  add_custom_target(doc ${all_arg})
  add_custom_target(doc-tarballs ${all_arg})

  add_subdirectory(Utilities/Doxygen)
  add_subdirectory(Utilities/Scripts/CjyxWizard/doc)
endif()

#-----------------------------------------------------------------------------
# Install CMake modules
#-----------------------------------------------------------------------------
if(NOT Cjyx_INSTALL_NO_DEVELOPMENT)
  file(GLOB cmake_files "${CMAKE_CURRENT_SOURCE_DIR}/CMake/*.cmake")
  install(
    FILES ${cmake_files}
    DESTINATION ${Cjyx_INSTALL_LIB_DIR}/CMake COMPONENT Development
    )

  install(FILES
    ${Cjyx_BINARY_DIR}/UseCjyx.cmake
    ${Cjyx_BINARY_DIR}/CjyxConfig.cmake
    ${Cjyx_BINARY_DIR}/CjyxConfigVersion.cmake
    DESTINATION ${Cjyx_INSTALL_LIB_DIR}
    COMPONENT Development
    )
endif()

#-----------------------------------------------------------------------------
# Create target to download data from the <Cjyx_ExternalData_DATA_MANAGEMENT_TARGET> group.
# This must come after all tests have been added that reference the group, so we put it last.
ExternalData_Add_Target(${Cjyx_ExternalData_DATA_MANAGEMENT_TARGET})

#-----------------------------------------------------------------------------
# Create targets CopyCjyxPython{Resource, Script}Files, CompileCjyxPythonFiles
if(Cjyx_USE_PYTHONQT)
  cjyxFunctionAddPythonQtResourcesTargets(CjyxPythonResources)
  ctkFunctionAddCompilePythonScriptTargets(
    ${CTK_COMPILE_PYTHON_SCRIPTS_GLOBAL_TARGET_NAME}
    DEPENDS CjyxPythonResources
    )

  set(_python_dir ${Cjyx_SUPERBUILD_DIR}/python-install)
  if(NOT Cjyx_USE_SYSTEM_python OR NOT EXISTS ${_python_dir})
    # Custom target to ensure there are cached byte-code files for all the python
    # standard library and site-packages modules.
    #
    # Considering (1) some of the tests purposefully include invalid code
    # that would lead the compilation to fail and (2) test directories are
    # explicitly excluded from packages in "CMake/CjyxBlockInstallPython.cmake",
    # the corresponding files are excluded from the compilation passing the "-x"
    # argument.
    add_custom_command(
      COMMAND ${PYTHON_EXECUTABLE} -m compileall -q -x "[\/\\\\]test[s]?[\/\\\\]" ${_python_dir}/${PYTHON_STDLIB_SUBDIR}
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/python_compile_stdlib_and_sitepackages_complete
      COMMENT "Compiling python stdlib and site-packages modules: ${_python_dir}/${PYTHON_STDLIB_SUBDIR}"
      VERBATIM
      )
    add_custom_target(CompileStdLibAndSitePackagesPythonFiles ALL
      DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/python_compile_stdlib_and_sitepackages_complete
      )
  endif()
endif()

#-----------------------------------------------------------------------------
# The commands in this directory are intended to be executed as
# the end of the whole configuration process, as a "last step".
# This directory is typically the last add_subdirectory in the main CMakeLists.txt.
add_subdirectory(CMake/LastConfigureStep)
