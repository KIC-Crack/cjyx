if(Cjyx_USE_QtTesting AND Cjyx_USE_PYTHONQT)
  cjyxMacroBuildScriptedModule(
    NAME PlotsSelfTest
    SCRIPTS PlotsSelfTest.py
    )
  cjyx_add_python_unittest(SCRIPT PlotsSelfTest.py)
endif()
