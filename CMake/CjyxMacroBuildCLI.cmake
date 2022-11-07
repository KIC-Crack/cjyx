# CjyxExecutionModel
find_package(CjyxExecutionModel NO_MODULE REQUIRED GenerateCLP)
include(${CjyxExecutionModel_USE_FILE})

macro(cjyxMacroBuildCLI)
  set(options
    EXECUTABLE_ONLY
    NO_INSTALL VERBOSE
  )
  set(oneValueArgs  NAME LOGO_HEADER
  )
  set(multiValueArgs
    ADDITIONAL_SRCS
    TARGET_LIBRARIES
    LINK_DIRECTORIES
    INCLUDE_DIRECTORIES
  )
  cmake_parse_arguments(MY_CJYX
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
    )

  if(${MY_CJYX_EXECUTABLE_ONLY})
    set(PASS_EXECUTABLE_ONLY EXECUTABLE_ONLY)
  endif()
  if(${MY_CJYX_NO_INSTALL})
    set(PASS_NO_INSTALL NO_INSTALL)
  endif()
  if(${MY_CJYX_VERBOSE})
    set(PASS_VERBOSE VERBOSE)
  endif()

  message(WARNING "Macro 'cjyxMacroBuildCLI' is *DEPRECATED* - Use 'SEMMacroBuildCLI' instead.")

  SEMMacroBuildCLI(
    ${PASS_EXECUTABLE_ONLY}
    ${PASS_NO_INSTALL}
    ${PASS_VERBOSE}
    ADDITIONAL_SRCS                 ${MY_CJYX_ADDITIONAL_SRCS}
    TARGET_LIBRARIES                ${MY_CJYX_TARGET_LIBRARIES}
    LINK_DIRECTORIES                ${MY_CJYX_LINK_DIRECTORIES}
    INCLUDE_DIRECTORIES             "${MY_CJYX_INCLUDE_DIRECTORIES}"
    NAME                            ${MY_CJYX_NAME}
    LOGO_HEADER                     ${MY_CJYX_LOGO_HEADER}
    RUNTIME_OUTPUT_DIRECTORY        "${CMAKE_BINARY_DIR}/${Cjyx_CLIMODULES_BIN_DIR}"
    LIBRARY_OUTPUT_DIRECTORY        "${CMAKE_BINARY_DIR}/${Cjyx_CLIMODULES_LIB_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY        "${CMAKE_BINARY_DIR}/${Cjyx_CLIMODULES_LIB_DIR}"
    INSTALL_RUNTIME_DESTINATION     ${Cjyx_INSTALL_CLIMODULES_BIN_DIR}
    INSTALL_LIBRARY_DESTINATION     ${Cjyx_INSTALL_CLIMODULES_LIB_DIR}
    INSTALL_ARCHIVE_DESTINATION     ${Cjyx_INSTALL_CLIMODULES_LIB_DIR}
    )
endmacro()
