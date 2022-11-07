project(CjyxBaseCLI)

# --------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------

#
# ModuleDescriptionParser
#
find_package(CjyxExecutionModel REQUIRED ModuleDescriptionParser)


#
# ITK - Import ITK targets required by ModuleDescriptionParser
#
set(${PROJECT_NAME}_ITK_COMPONENTS
  ${ModuleDescriptionParser_ITK_COMPONENTS}
  )
find_package(ITK 4.6 COMPONENTS ${${PROJECT_NAME}_ITK_COMPONENTS} REQUIRED)

# --------------------------------------------------------------------------
# Include dirs
# --------------------------------------------------------------------------
set(Cjyx_BaseCLI_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Testing
  )
set(include_dirs
  ${Cjyx_BaseCLI_INCLUDE_DIRS}
  ${ModuleDescriptionParser_INCLUDE_DIRS}
  )
include_directories(${include_dirs})

# --------------------------------------------------------------------------
# Update CjyxExecutionModel_EXTRA_INCLUDE_DIRECTORIES
# --------------------------------------------------------------------------
set(CjyxExecutionModel_EXTRA_INCLUDE_DIRECTORIES
  ${CjyxExecutionModel_EXTRA_INCLUDE_DIRECTORIES}
  ${Cjyx_BaseCLI_INCLUDE_DIRS}
  ${ITKFactoryRegistration_INCLUDE_DIRS}
  CACHE INTERNAL "CjyxExecutionModel extra includes" FORCE
  )
set(CjyxExecutionModel_CLI_LIBRARY_WRAPPER_CXX
  ${CMAKE_CURRENT_BINARY_DIR}/SEMCommandLineLibraryWrapper.cxx
  CACHE INTERNAL "CjyxExecutionModel extra includes" FORCE
  )
configure_file(
  SEMCommandLineLibraryWrapper.cxx.in
  ${CjyxExecutionModel_CLI_LIBRARY_WRAPPER_CXX}
  )

# --------------------------------------------------------------------------
# Update Cjyx_Base_INCLUDE_DIRS
# --------------------------------------------------------------------------
set(Cjyx_Base_INCLUDE_DIRS ${Cjyx_Base_INCLUDE_DIRS}
  ${Cjyx_BaseCLI_INCLUDE_DIRS} CACHE INTERNAL "Cjyx Base includes" FORCE)

# --------------------------------------------------------------------------
# Configure headers
# --------------------------------------------------------------------------
set(configure_header_file vtkCjyxBaseCLIConfigure.h)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/${configure_header_file}.in
  ${CMAKE_CURRENT_BINARY_DIR}/${configure_header_file}
  )

# --------------------------------------------------------------------------
# Install headers
# --------------------------------------------------------------------------
if(NOT DEFINED ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
  set(${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT ON)
endif()
if(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
  file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  install(
    FILES ${headers} ${CMAKE_CURRENT_BINARY_DIR}/${configure_header_file}
    DESTINATION include/${PROJECT_NAME} COMPONENT Development)
endif()

# --------------------------------------------------------------------------
# Sources
# --------------------------------------------------------------------------
# Single library target. Down the road, we may want to have one library that
# only depends on ITK and a second library that only depends on VTK

set(CjyxBaseCLI_SRCS
  )
set(CjyxBaseCLI_LIBS
  ModuleDescriptionParser ${ITK_LIBRARIES}
  )

#find_package(VTK)
if(VTK_FOUND)
  #include(${VTK_USE_FILE})
  list(APPEND CjyxBaseCLI_SRCS vtkPluginFilterWatcher.cxx)
  list(APPEND CjyxBaseCLI_LIBS ${VTK_LIBRARIES})
endif()

if(CjyxBaseCLI_SRCS)
  # --------------------------------------------------------------------------
  # Build library
  # --------------------------------------------------------------------------
  set(lib_name "CjyxBaseCLI")

  add_library(${lib_name} ${CjyxBaseCLI_SRCS})
  target_link_libraries(${lib_name} ${CjyxBaseCLI_LIBS})
  set_target_properties(${lib_name} PROPERTIES LABELS ${lib_name})
  set_target_properties(${lib_name} PROPERTIES FOLDER "Core-Base")

  # Apply user-defined properties to the library target.
  if(Cjyx_LIBRARY_PROPERTIES)
    set_target_properties(${lib_name} PROPERTIES ${Cjyx_LIBRARY_PROPERTIES})
  endif()

  # --------------------------------------------------------------------------
  # Install library
  # --------------------------------------------------------------------------
  install(TARGETS ${lib_name}
    RUNTIME DESTINATION ${Cjyx_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${Cjyx_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${Cjyx_INSTALL_LIB_DIR} COMPONENT Development
    )
endif()
