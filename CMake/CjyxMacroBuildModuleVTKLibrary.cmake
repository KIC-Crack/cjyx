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
# CjyxMacroBuildModuleVTKLibrary
#

macro(CjyxMacroBuildModuleVTKLibrary)
  set(options
    DISABLE_WRAP_PYTHON
    NO_INSTALL
    )
  set(oneValueArgs
    NAME
    EXPORT_DIRECTIVE
    FOLDER
    )
  set(multiValueArgs
    SRCS
    INCLUDE_DIRECTORIES
    TARGET_LIBRARIES
    )
  cmake_parse_arguments(MODULEVTKLIBRARY
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
    )

  # --------------------------------------------------------------------------
  # Sanity checks
  # --------------------------------------------------------------------------
  if(MODULEVTKLIBRARY_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Unknown keywords given to CjyxMacroBuildModuleVTKLibrary(): \"${MODULEVTKLIBRARY_UNPARSED_ARGUMENTS}\"")
  endif()

  set(expected_nonempty_vars NAME EXPORT_DIRECTIVE SRCS)
  foreach(var ${expected_nonempty_vars})
    if("${MODULEVTKLIBRARY_${var}}" STREQUAL "")
      message(FATAL_ERROR "error: ${var} CMake variable is empty !")
    endif()
  endforeach()

  # --------------------------------------------------------------------------
  # Define library name
  # --------------------------------------------------------------------------
  set(lib_name ${MODULEVTKLIBRARY_NAME})

  # --------------------------------------------------------------------------
  # Set <MODULEVTKLIBRARY_NAME>_INCLUDE_DIRS
  # --------------------------------------------------------------------------
  set(_include_dirs
    ${${MODULEVTKLIBRARY_NAME}_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )
  # Since module developer may have already set the variable to some
  # specific values in the module CMakeLists.txt, we make sure to
  # consider the already set variable and remove duplicates.
  list(REMOVE_DUPLICATES _include_dirs)
  set(${MODULEVTKLIBRARY_NAME}_INCLUDE_DIRS
    ${_include_dirs}
    CACHE INTERNAL "${MODULEVTKLIBRARY_NAME} include directories" FORCE)

  # --------------------------------------------------------------------------
  # Include dirs
  # --------------------------------------------------------------------------
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${MODULEVTKLIBRARY_INCLUDE_DIRECTORIES}
    )

  #-----------------------------------------------------------------------------
  # Configure export header
  #-----------------------------------------------------------------------------
  set(MY_LIBRARY_EXPORT_DIRECTIVE ${MODULEVTKLIBRARY_EXPORT_DIRECTIVE})
  set(MY_EXPORT_HEADER_PREFIX ${MODULEVTKLIBRARY_NAME})
  set(MY_LIBNAME ${lib_name})

  # Sanity checks
  if(NOT EXISTS ${Cjyx_EXPORT_HEADER_TEMPLATE})
    message(FATAL_ERROR "error: Cjyx_EXPORT_HEADER_TEMPLATE doesn't exist: ${Cjyx_EXPORT_HEADER_TEMPLATE}")
  endif()

  configure_file(
    ${Cjyx_EXPORT_HEADER_TEMPLATE}
    ${CMAKE_CURRENT_BINARY_DIR}/${MY_EXPORT_HEADER_PREFIX}Export.h
    )
  set(dynamicHeaders
    "${dynamicHeaders};${CMAKE_CURRENT_BINARY_DIR}/${MY_EXPORT_HEADER_PREFIX}Export.h")

  #-----------------------------------------------------------------------------
  # Source groups
  # --------------------------------------------------------------------------
  source_group("Generated" FILES
    ${dynamicHeaders}
    )

  # --------------------------------------------------------------------------
  # Build library
  # --------------------------------------------------------------------------
  add_library(${lib_name}
    ${MODULEVTKLIBRARY_SRCS}
    )

  # Set loadable modules output
  set_target_properties(${lib_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_BIN_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_LIB_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_LIB_DIR}"
    )

  target_link_libraries(${lib_name}
    ${MODULEVTKLIBRARY_TARGET_LIBRARIES}
    )

  # Apply user-defined properties to the library target.
  if(Cjyx_LIBRARY_PROPERTIES)
    set_target_properties(${lib_name} PROPERTIES ${Cjyx_LIBRARY_PROPERTIES})
  endif()

  if(NOT "${MODULEVTKLIBRARY_FOLDER}" STREQUAL "")
    set_target_properties(${lib_name} PROPERTIES FOLDER ${MODULEVTKLIBRARY_FOLDER})
  endif()

  # --------------------------------------------------------------------------
  # Install library
  # --------------------------------------------------------------------------
  if(NOT MY_NO_INSTALL)
    install(TARGETS ${lib_name}
      RUNTIME DESTINATION ${Cjyx_INSTALL_QTLOADABLEMODULES_BIN_DIR} COMPONENT RuntimeLibraries
      LIBRARY DESTINATION ${Cjyx_INSTALL_QTLOADABLEMODULES_LIB_DIR} COMPONENT RuntimeLibraries
      ARCHIVE DESTINATION ${Cjyx_INSTALL_QTLOADABLEMODULES_LIB_DIR} COMPONENT Development
      )
  endif()

  # --------------------------------------------------------------------------
  # Install headers
  # --------------------------------------------------------------------------
  if(DEFINED Cjyx_DEVELOPMENT_INSTALL)
    if(NOT DEFINED ${MODULEVTKLIBRARY_NAME}_DEVELOPMENT_INSTALL)
      set(${MODULEVTKLIBRARY_NAME}_DEVELOPMENT_INSTALL ${Cjyx_DEVELOPMENT_INSTALL})
    endif()
  else()
    if(NOT DEFINED ${MODULEVTKLIBRARY_NAME}_DEVELOPMENT_INSTALL)
      set(${MODULEVTKLIBRARY_NAME}_DEVELOPMENT_INSTALL OFF)
    endif()
  endif()

  if(NOT MY_NO_INSTALL AND ${MODULEVTKLIBRARY_NAME}_DEVELOPMENT_INSTALL)
    file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
    install(FILES
      ${headers}
      ${dynamicHeaders}
      DESTINATION ${Cjyx_INSTALL_QTLOADABLEMODULES_INCLUDE_DIR}/${MODULEVTKLIBRARY_NAME} COMPONENT Development
      )
  endif()

  # --------------------------------------------------------------------------
  # Export target
  # --------------------------------------------------------------------------
  set_property(GLOBAL APPEND PROPERTY Cjyx_TARGETS ${MODULEVTKLIBRARY_NAME})

endmacro()
