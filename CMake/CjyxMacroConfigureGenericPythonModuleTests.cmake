
#
# CjyxMacroConfigureGenericPythonModuleTests
#

macro(CjyxMacroConfigureGenericPythonModuleTests MODULENAMES TEST_SCRIPTS_OUTPUT_VAR)
  # Sanity checks
  if("${MODULENAMES}" STREQUAL "")
    message(FATAL_ERROR "error: Variable MODULENAMES is empty !")
  endif()
  if("${TEST_SCRIPTS_OUTPUT_VAR}" STREQUAL "")
    message(FATAL_ERROR "error: Variable TEST_SCRIPTS_OUTPUT_VAR is empty !")
  endif()
  if("${Cjyx_PYTHON_MODULE_TEST_TEMPLATES_DIR}" STREQUAL "")
    message(FATAL_ERROR "error: Variable Cjyx_PYTHON_MODULE_TEST_TEMPLATES_DIR is empty !")
  endif()

  foreach(MODULENAME ${MODULENAMES})

    # Note: The variables MODULENAME and MODULENAME_LC are used to
    #       configure the different test scripts.
    string(TOLOWER ${MODULENAME} MODULENAME_LC)
    set(configured_test_src ${CMAKE_CURRENT_BINARY_DIR}/qCjyx${MODULENAME}ModuleGenericTest.py)
    configure_file(
      ${Cjyx_PYTHON_MODULE_TEST_TEMPLATES_DIR}/qCjyxModuleGenericTest.py.in
      ${configured_test_src}
      @ONLY
      )

    get_filename_component(script_filename ${configured_test_src} NAME)
    list(APPEND ${TEST_SCRIPTS_OUTPUT_VAR} ${script_filename})

  endforeach()

endmacro()

