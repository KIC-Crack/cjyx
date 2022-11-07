#-----------------------------------------------------------------------------
set(EXTENSION_TEST_PYTHON_SCRIPTS
  SubjectHierarchyFoldersTest1.py
  )

set(EXTENSION_TEST_PYTHON_RESOURCES
  )

if(Cjyx_USE_QtTesting AND Cjyx_USE_PYTHONQT)
  cjyxMacroBuildScriptedModule(
    NAME SubjectHierarchyCorePluginsSelfTest
    SCRIPTS SubjectHierarchyCorePluginsSelfTest.py
    )
  cjyx_add_python_unittest(SCRIPT SubjectHierarchyCorePluginsSelfTest.py)

  cjyxMacroBuildScriptedModule(
    NAME SubjectHierarchyGenericSelfTest
    SCRIPTS SubjectHierarchyGenericSelfTest.py
    )
  cjyx_add_python_unittest(SCRIPT SubjectHierarchyGenericSelfTest.py)
endif()

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
