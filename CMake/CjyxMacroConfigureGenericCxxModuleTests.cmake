
#
# CjyxMacroConfigureGenericCxxModuleTests
#

macro(CjyxMacroConfigureGenericCxxModuleTests MODULENAMES TEST_SRCS_OUTPUT_VAR TEST_NAMES_OUTPUT_VAR TEST_NAMES_CXX_OUTPUT_VAR)
  # Sanity checks
  if("${MODULENAMES}" STREQUAL "")
    message(FATAL_ERROR "error: Variable MODULENAMES is empty !")
  endif()
  if("${TEST_SRCS_OUTPUT_VAR}" STREQUAL "")
    message(FATAL_ERROR "error: Variable TEST_SRCS_OUTPUT_VAR is empty !")
  endif()
  if("${TEST_NAMES_OUTPUT_VAR}" STREQUAL "")
    message(FATAL_ERROR "error: Variable TEST_NAMES_OUTPUT_VAR is empty !")
  endif()
  if("${TEST_NAMES_CXX_OUTPUT_VAR}" STREQUAL "")
    message(FATAL_ERROR "error: Variable TEST_NAMES_CXX_OUTPUT_VAR is empty !")
  endif()
  if("${Cjyx_CXX_MODULE_TEST_TEMPLATES_DIR}" STREQUAL "")
    message(FATAL_ERROR "error: Variable Cjyx_CXX_MODULE_TEST_TEMPLATES_DIR is empty !")
  endif()

  set(MODULEPATH_WITHOUT_INTDIR ${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_BIN_DIR})

  foreach(MODULENAME ${MODULENAMES})
    set(MODULETESTS ModuleGenericTest ModuleWidgetGenericTest)
    foreach(MODULETEST ${MODULETESTS})

      # Note: the variable MODULENAME is used to configure the different tests.
      set(configured_test_src ${CMAKE_CURRENT_BINARY_DIR}/qCjyx${MODULENAME}${MODULETEST}.cxx)

      configure_file(
        ${Cjyx_CXX_MODULE_TEST_TEMPLATES_DIR}/qCjyx${MODULETEST}.cxx.in
        ${configured_test_src}
        @ONLY
        )

      list(APPEND ${TEST_SRCS_OUTPUT_VAR} ${configured_test_src})

      get_filename_component(test_name ${configured_test_src} NAME_WE)
      list(APPEND ${TEST_NAMES_OUTPUT_VAR} ${test_name})

      get_filename_component(test_filename ${configured_test_src} NAME)
      list(APPEND ${TEST_NAMES_CXX_OUTPUT_VAR} ${test_filename})
    endforeach()
  endforeach()

endmacro()

