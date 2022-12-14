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
# CjyxMacroBuildBaseQtLibrary
#

#
# Parameters:
#
#   NAME .................: Name of the library
#
#   EXPORT_DIRECTIVE .....: Export directive that should be used to export symbol
#
#   SRCS .................: List of source files
#
#   MOC_SRCS .............: Optional list of headers to run through the meta object compiler (moc)
#                           using Qt(4|5)_WRAP_CPP CMake macro
#
#   UI_SRCS ..............: Optional list of UI file to run through UI compiler (uic) using
#                           Qt(4|5)_WRAP_UI CMake macro
#
#   INCLUDE_DIRECTORIES ..: Optional list of extra folder that should be included. See implementation
#                           for the list of folder included by default.
#
#   TARGET_LIBRARIES .....: Optional list of target libraries that should be used with TARGET_LINK_LIBRARIES
#                           CMake macro. See implementation for the list of libraries added by default.
#
#   RESOURCES ............: Optional list of files that should be converted into resource header
#                           using Qt(4|5)_ADD_RESOURCES
#
# Options:
#
#   WRAP_PYTHONQT ........: If specified, the sources (SRCS) will be 'PythonQt' wrapped and a static
#                           library named <NAME>PythonQt will be built.
#

macro(CjyxMacroBuildBaseQtLibrary)
  set(options
    WRAP_PYTHONQT
    )
  set(oneValueArgs
    NAME
    EXPORT_DIRECTIVE
    )
  set(multiValueArgs
    SRCS
    MOC_SRCS
    UI_SRCS
    INCLUDE_DIRECTORIES
    TARGET_LIBRARIES
    RESOURCES
    )
  cmake_parse_arguments(CJYXQTBASELIB
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
    )

  message(STATUS "Configuring ${Cjyx_MAIN_PROJECT_APPLICATION_NAME} Qt base library: ${CJYXQTBASELIB_NAME}")
  # --------------------------------------------------------------------------
  # Sanity checks
  # --------------------------------------------------------------------------
  if(CJYXQTBASELIB_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Unknown keywords given to CjyxMacroBuildBaseQtLibrary(): \"${CJYXQTBASELIB_UNPARSED_ARGUMENTS}\"")
  endif()

  set(expected_defined_vars NAME EXPORT_DIRECTIVE)
  foreach(var ${expected_defined_vars})
    if(NOT DEFINED CJYXQTBASELIB_${var})
      message(FATAL_ERROR "${var} is mandatory")
    endif()
  endforeach()

  if(NOT DEFINED Cjyx_INSTALL_NO_DEVELOPMENT)
    message(SEND_ERROR "Cjyx_INSTALL_NO_DEVELOPMENT is mandatory")
  endif()

  # --------------------------------------------------------------------------
  # Define library name
  # --------------------------------------------------------------------------
  set(lib_name ${CJYXQTBASELIB_NAME})

  # --------------------------------------------------------------------------
  # Include dirs
  # --------------------------------------------------------------------------

  set(include_dirs
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Cjyx_Base_INCLUDE_DIRS}
    ${Cjyx_Libs_INCLUDE_DIRS}
    ${CJYXQTBASELIB_INCLUDE_DIRECTORIES}
    )

  include_directories(${include_dirs})

  #-----------------------------------------------------------------------------
  # Update Cjyx_Base_INCLUDE_DIRS
  #-----------------------------------------------------------------------------
  set(Cjyx_Base_INCLUDE_DIRS ${Cjyx_Base_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    CACHE INTERNAL "Cjyx Base includes" FORCE)

  #-----------------------------------------------------------------------------
  # Configure
  # --------------------------------------------------------------------------
  set(MY_LIBRARY_EXPORT_DIRECTIVE ${CJYXQTBASELIB_EXPORT_DIRECTIVE})
  set(MY_EXPORT_HEADER_PREFIX ${CJYXQTBASELIB_NAME})
  set(MY_LIBNAME ${lib_name})

  configure_file(
    ${Cjyx_SOURCE_DIR}/CMake/qCjyxExport.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/${MY_EXPORT_HEADER_PREFIX}Export.h
    )
  set(dynamicHeaders
    "${dynamicHeaders};${CMAKE_CURRENT_BINARY_DIR}/${MY_EXPORT_HEADER_PREFIX}Export.h")

  #-----------------------------------------------------------------------------
  # Sources
  # --------------------------------------------------------------------------
    set(_moc_options OPTIONS -DCjyx_HAVE_QT5)
    QT5_WRAP_CPP(CJYXQTBASELIB_MOC_OUTPUT ${CJYXQTBASELIB_MOC_SRCS} ${_moc_options})
    QT5_WRAP_UI(CJYXQTBASELIB_UI_CXX ${CJYXQTBASELIB_UI_SRCS})
    if(DEFINED CJYXQTBASELIB_RESOURCES)
      QT5_ADD_RESOURCES(CJYXQTBASELIB_QRC_SRCS ${CJYXQTBASELIB_RESOURCES})
    endif()

    QT5_ADD_RESOURCES(CJYXQTBASELIB_QRC_SRCS ${Cjyx_SOURCE_DIR}/Resources/qCjyx.qrc)

  set_source_files_properties(
    ${CJYXQTBASELIB_UI_CXX}
    ${CJYXQTBASELIB_MOC_OUTPUT}
    ${CJYXQTBASELIB_QRC_SRCS}
    WRAP_EXCLUDE
    )

  # --------------------------------------------------------------------------
  # Source groups
  # --------------------------------------------------------------------------
  source_group("Resources" FILES
    ${CJYXQTBASELIB_UI_SRCS}
    ${Cjyx_SOURCE_DIR}/Resources/qCjyx.qrc
    ${CJYXQTBASELIB_RESOURCES}
  )

  source_group("Generated" FILES
    ${CJYXQTBASELIB_UI_CXX}
    ${CJYXQTBASELIB_MOC_OUTPUT}
    ${CJYXQTBASELIB_QRC_SRCS}
    ${dynamicHeaders}
  )

  # --------------------------------------------------------------------------
  # Translation
  # --------------------------------------------------------------------------
  if(Cjyx_BUILD_I18N_SUPPORT)
    set(TS_DIR
      "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Translations/"
    )

    include(CjyxMacroTranslation)
    CjyxMacroTranslation(
      SRCS ${CJYXQTBASELIB_SRCS}
      UI_SRCS ${CJYXQTBASELIB_UI_SRCS}
      TS_DIR ${TS_DIR}
      TS_BASEFILENAME ${CJYXQTBASELIB_NAME}
      TS_LANGUAGES ${Cjyx_LANGUAGES}
      QM_OUTPUT_DIR_VAR QM_OUTPUT_DIR
      QM_OUTPUT_FILES_VAR QM_OUTPUT_FILES
      )

    set_property(GLOBAL APPEND PROPERTY Cjyx_QM_OUTPUT_DIRS ${QM_OUTPUT_DIR})
  else()
    set(QM_OUTPUT_FILES )
  endif()

  # --------------------------------------------------------------------------
  # Build the library
  # --------------------------------------------------------------------------
  add_library(${lib_name}
    ${CJYXQTBASELIB_SRCS}
    ${CJYXQTBASELIB_MOC_OUTPUT}
    ${CJYXQTBASELIB_UI_CXX}
    ${CJYXQTBASELIB_QRC_SRCS}
    ${QM_OUTPUT_FILES}
    )
  set_target_properties(${lib_name} PROPERTIES LABELS ${lib_name})

  # Apply user-defined properties to the library target.
  if(Cjyx_LIBRARY_PROPERTIES)
    set_target_properties(${lib_name} PROPERTIES ${Cjyx_LIBRARY_PROPERTIES})
  endif()

  target_link_libraries(${lib_name}
    ${CJYXQTBASELIB_TARGET_LIBRARIES}
    )

  # Folder
  set_target_properties(${lib_name} PROPERTIES FOLDER "Core-Base")

  #-----------------------------------------------------------------------------
  # Install library
  #-----------------------------------------------------------------------------
  install(TARGETS ${lib_name}
    RUNTIME DESTINATION ${Cjyx_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${Cjyx_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${Cjyx_INSTALL_LIB_DIR} COMPONENT Development
  )

  # --------------------------------------------------------------------------
  # Install headers
  # --------------------------------------------------------------------------
  if(NOT Cjyx_INSTALL_NO_DEVELOPMENT)
    # Install headers
    file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
    install(FILES
      ${headers}
      ${dynamicHeaders}
      DESTINATION ${Cjyx_INSTALL_INCLUDE_DIR}/${PROJECT_NAME} COMPONENT Development
      )
  endif()

  # --------------------------------------------------------------------------
  # PythonQt wrapping
  # --------------------------------------------------------------------------
  if(Cjyx_USE_PYTHONQT AND CJYXQTBASELIB_WRAP_PYTHONQT)
    ctkMacroBuildLibWrapper(
      NAMESPACE "osb" # Use "osb" instead of "org.cjyx.base" to avoid build error on windows
      TARGET ${lib_name}
      SRCS "${CJYXQTBASELIB_SRCS}"
      INSTALL_BIN_DIR ${Cjyx_INSTALL_BIN_DIR}
      INSTALL_LIB_DIR ${Cjyx_INSTALL_LIB_DIR}
      )
    set_target_properties(${lib_name}PythonQt PROPERTIES FOLDER "Core-Base")
  endif()

  # --------------------------------------------------------------------------
  # Export target
  # --------------------------------------------------------------------------
  set_property(GLOBAL APPEND PROPERTY Cjyx_TARGETS ${CJYXQTBASELIB_NAME})

endmacro()
