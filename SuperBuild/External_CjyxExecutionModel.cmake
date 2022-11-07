
set(proj CjyxExecutionModel)

# Set dependency list
set(${proj}_DEPENDENCIES ${ITK_EXTERNAL_NAME})

if(Cjyx_USE_TBB)
  list(APPEND ${proj}_DEPENDENCIES tbb)
endif()

if(Cjyx_BUILD_PARAMETERSERIALIZER_SUPPORT)
  set(${proj}_DEPENDENCIES ${${proj}_DEPENDENCIES} JsonCpp ParameterSerializer)
endif()

# Include dependent projects if any
ExternalProject_Include_Dependencies(${proj} PROJECT_VAR proj DEPENDS_VAR ${proj}_DEPENDENCIES)

if(Cjyx_USE_SYSTEM_${proj})
  message(FATAL_ERROR "Enabling Cjyx_USE_SYSTEM_${proj} is not supported !")
endif()

# Sanity checks
if(DEFINED CjyxExecutionModel_DIR AND NOT EXISTS ${CjyxExecutionModel_DIR})
  message(FATAL_ERROR "CjyxExecutionModel_DIR variable is defined but corresponds to nonexistent directory")
endif()

if(NOT DEFINED CjyxExecutionModel_DIR AND NOT Cjyx_USE_SYSTEM_${proj})

  set(EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS)
  set(EXTERNAL_PROJECT_OPTIONAL_CMAKE_ARGS)
  set(EXTERNAL_PROJECT_ADDITIONAL_FORWARD_PATHS_BUILD)
  set(EXTERNAL_PROJECT_ADDITIONAL_FORWARD_PATHS_INSTALL)

  if(APPLE)
    list(APPEND EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS
      -DCjyxExecutionModel_DEFAULT_CLI_EXECUTABLE_LINK_FLAGS:STRING=-Wl,-rpath,@loader_path/../../../
      )
  endif()

  if(Cjyx_BUILD_PARAMETERSERIALIZER_SUPPORT)
    list(APPEND EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS
      -DJsonCpp_INCLUDE_DIR:PATH=${JsonCpp_INCLUDE_DIR}
      -DParameterSerializer_DIR:PATH=${ParameterSerializer_DIR}
      )
    # JsoncCpp_LIBRARY needs to be added as a CMAKE_ARGS because it contains an
    # expression that needs to be evaluated
    list(APPEND EXTERNAL_PROJECT_OPTIONAL_CMAKE_ARGS
      -DJsonCpp_LIBRARY:PATH=${JsonCpp_LIBRARY}
      )
  endif()

  if(Cjyx_USE_PYTHONQT)
    list(APPEND EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS
      # Required by FindPython3 CMake module used by VTK
      -DPython3_ROOT_DIR:PATH=${Python3_ROOT_DIR}
      -DPython3_INCLUDE_DIR:PATH=${Python3_INCLUDE_DIR}
      -DPython3_LIBRARY:FILEPATH=${Python3_LIBRARY}
      -DPython3_LIBRARY_DEBUG:FILEPATH=${Python3_LIBRARY_DEBUG}
      -DPython3_LIBRARY_RELEASE:FILEPATH=${Python3_LIBRARY_RELEASE}
      -DPython3_EXECUTABLE:FILEPATH=${Python3_EXECUTABLE}
      )
  endif()

  if(Cjyx_USE_TBB)
    list(APPEND EXTERNAL_PROJECT_ADDITIONAL_FORWARD_PATHS_BUILD
      ${TBB_BIN_DIR}
      )
    list(APPEND EXTERNAL_PROJECT_ADDITIONAL_FORWARD_PATHS_INSTALL
      ${TBB_BIN_DIR}
      )
  endif()

  # If it applies, prepend "CMAKE_ARGS"
  if(NOT EXTERNAL_PROJECT_OPTIONAL_CMAKE_ARGS STREQUAL "")
    set(EXTERNAL_PROJECT_OPTIONAL_CMAKE_ARGS
      CMAKE_ARGS
      ${EXTERNAL_PROJECT_OPTIONAL_CMAKE_ARGS})
  endif()

  ExternalProject_SetIfNotDefined(
    Cjyx_${proj}_GIT_REPOSITORY
    "${EP_GIT_PROTOCOL}://github.com/yulaomao/CjyxExecutionModel.git"
    QUIET
    )

  ExternalProject_SetIfNotDefined(
    Cjyx_${proj}_GIT_TAG
    "ac58777835b7f336de35321792919f9678a0e47e"
    QUIET
    )

  set(EP_SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj})
  set(EP_BINARY_DIR ${CMAKE_BINARY_DIR}/${proj}-build)

  ExternalProject_Add(${proj}
    ${${proj}_EP_ARGS}
    GIT_REPOSITORY "${Cjyx_${proj}_GIT_REPOSITORY}"
    GIT_TAG "${Cjyx_${proj}_GIT_TAG}"
    SOURCE_DIR ${EP_SOURCE_DIR}
    BINARY_DIR ${EP_BINARY_DIR}
    CMAKE_CACHE_ARGS
      # Compiler settings
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${ep_common_cxx_flags}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_C_FLAGS:STRING=${ep_common_c_flags} # Unused
      -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}
      -DCMAKE_CXX_STANDARD_REQUIRED:BOOL=${CMAKE_CXX_STANDARD_REQUIRED}
      -DCMAKE_CXX_EXTENSIONS:BOOL=${CMAKE_CXX_EXTENSIONS}
      # Options
      -DBUILD_TESTING:BOOL=OFF
      -DCjyxExecutionModel_USE_UTF8:BOOL=ON
      -DITK_DIR:PATH=${ITK_DIR}
      -DCjyxExecutionModel_USE_SERIALIZER:BOOL=${Cjyx_BUILD_PARAMETERSERIALIZER_SUPPORT}
      -DCjyxExecutionModel_USE_JSONCPP:BOOL=${Cjyx_BUILD_PARAMETERSERIALIZER_SUPPORT}
      -DCjyxExecutionModel_LIBRARY_PROPERTIES:STRING=${Cjyx_LIBRARY_PROPERTIES}
      # Output directories
      -DCjyxExecutionModel_DEFAULT_CLI_RUNTIME_OUTPUT_DIRECTORY:PATH=${CjyxExecutionModel_DEFAULT_CLI_RUNTIME_OUTPUT_DIRECTORY}
      -DCjyxExecutionModel_DEFAULT_CLI_LIBRARY_OUTPUT_DIRECTORY:PATH=${CjyxExecutionModel_DEFAULT_CLI_LIBRARY_OUTPUT_DIRECTORY}
      -DCjyxExecutionModel_DEFAULT_CLI_ARCHIVE_OUTPUT_DIRECTORY:PATH=${CjyxExecutionModel_DEFAULT_CLI_ARCHIVE_OUTPUT_DIRECTORY}
      # Install directories
      -DCjyxExecutionModel_INSTALL_BIN_DIR:PATH=${Cjyx_INSTALL_LIB_DIR}
      -DCjyxExecutionModel_INSTALL_LIB_DIR:PATH=${Cjyx_INSTALL_LIB_DIR}
      #-DCjyxExecutionModel_INSTALL_SHARE_DIR:PATH=${Cjyx_INSTALL_ROOT}share/${CjyxExecutionModel}
      -DCjyxExecutionModel_DEFAULT_CLI_INSTALL_RUNTIME_DESTINATION:STRING=${Cjyx_INSTALL_CLIMODULES_BIN_DIR}
      -DCjyxExecutionModel_DEFAULT_CLI_INSTALL_LIBRARY_DESTINATION:STRING=${Cjyx_INSTALL_CLIMODULES_LIB_DIR}
      -DCjyxExecutionModel_DEFAULT_CLI_INSTALL_ARCHIVE_DESTINATION:STRING=${Cjyx_INSTALL_CLIMODULES_LIB_DIR}
      # Options
      -DCjyxExecutionModel_INSTALL_NO_DEVELOPMENT:BOOL=${Cjyx_INSTALL_NO_DEVELOPMENT}
      -DCjyxExecutionModel_DEFAULT_CLI_TARGETS_FOLDER_PREFIX:STRING=Module-
      -DCjyxExecutionModel_ADDITIONAL_FORWARD_PATHS_BUILD:PATH=${EXTERNAL_PROJECT_ADDITIONAL_FORWARD_PATHS_BUILD}
      -DCjyxExecutionModel_ADDITIONAL_FORWARD_PATHS_INSTALL:PATH=${EXTERNAL_PROJECT_ADDITIONAL_FORWARD_PATHS_INSTALL}
      ${EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS}
    ${EXTERNAL_PROJECT_OPTIONAL_CMAKE_ARGS}
    INSTALL_COMMAND ""
    DEPENDS
      ${${proj}_DEPENDENCIES}
    )

  ExternalProject_GenerateProjectDescription_Step(${proj})

  set(CjyxExecutionModel_DIR ${EP_BINARY_DIR})

  #-----------------------------------------------------------------------------
  # Launcher setting specific to build tree

  set(${proj}_LIBRARY_PATHS_LAUNCHER_BUILD ${CjyxExecutionModel_DIR}/ModuleDescriptionParser/bin/<CMAKE_CFG_INTDIR>)
  mark_as_superbuild(
    VARS ${proj}_LIBRARY_PATHS_LAUNCHER_BUILD
    LABELS "LIBRARY_PATHS_LAUNCHER_BUILD"
    )

else()
  ExternalProject_Add_Empty(${proj} DEPENDS ${${proj}_DEPENDENCIES})
endif()

mark_as_superbuild(
  VARS CjyxExecutionModel_DIR:PATH
  LABELS "FIND_PACKAGE"
  )
