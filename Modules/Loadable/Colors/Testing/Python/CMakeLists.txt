include(CjyxMacroBuildScriptedModule)
set(COLORS_PYTHON_RESOURCES
  )

# Test vtkCjyxScalarBarActor class
cjyxMacroBuildScriptedModule(
  NAME ColorLegendSelfTest
  SCRIPTS ColorLegendSelfTest.py
  RESOURCES ${MARKUPS_PYTHON_RESOURCES}
  )
cjyx_add_python_unittest(SCRIPT ColorLegendSelfTest.py
                           CJYX_ARGS --disable-cli-modules)

# Test restoring scene view with a custom color node
cjyx_add_python_test(SCRIPT CustomColorTableSceneViewRestoreTestBug3992.py
                       CJYX_ARGS --disable-cli-modules)
