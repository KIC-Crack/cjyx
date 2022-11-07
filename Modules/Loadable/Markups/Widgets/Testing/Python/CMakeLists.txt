if(Cjyx_USE_QtTesting AND Cjyx_USE_PYTHONQT)
  cjyxMacroBuildScriptedModule(
    NAME MarkupsWidgetsSelfTest
    SCRIPTS MarkupsWidgetsSelfTest.py
    )
  cjyx_add_python_unittest(SCRIPT MarkupsWidgetsSelfTest.py)
endif()
