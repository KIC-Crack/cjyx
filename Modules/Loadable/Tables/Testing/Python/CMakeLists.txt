if(Cjyx_USE_QtTesting AND Cjyx_USE_PYTHONQT)
  cjyxMacroBuildScriptedModule(
    NAME TablesSelfTest
    SCRIPTS TablesSelfTest.py
    )
  cjyx_add_python_unittest(SCRIPT TablesSelfTest.py)
endif()
