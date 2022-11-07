#-----------------------------------------------------------------------------
set(EXTENSION_TEST_PYTHON_SCRIPTS
  SegmentationsModuleTest1.py
  SegmentationsModuleTest2.py
  SegmentationWidgetsTest1.py
  )

set(EXTENSION_TEST_PYTHON_RESOURCES
  )

#-----------------------------------------------------------------------------
if(CMAKE_CONFIGURATION_TYPES)
  set(MODULE_BUILD_DIR "")
  foreach(config ${CMAKE_CONFIGURATION_TYPES})
    list(APPEND MODULE_BUILD_DIR "${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_LIB_DIR}/${config}")
  endforeach()
else()
  set(MODULE_BUILD_DIR "${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_LIB_DIR}")
endif()

foreach(scriptName ${EXTENSION_TEST_PYTHON_SCRIPTS})
  cjyx_add_python_unittest(
    SCRIPT ${scriptName}
    CJYX_ARGS --disable-cli-modules
                --no-main-window
                --additional-module-paths
                  ${MODULE_BUILD_DIR}
                  ${CMAKE_BINARY_DIR}/${Cjyx_QTSCRIPTEDMODULES_LIB_DIR}
    TESTNAME_PREFIX nomainwindow_
    )
endforeach()
