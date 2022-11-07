if(Cjyx_USE_QtTesting AND Cjyx_USE_PYTHONQT)
  cjyxMacroBuildScriptedModule(
    NAME CropVolumeSelfTest
    SCRIPTS CropVolumeSelfTest.py
    )
  cjyx_add_python_unittest(SCRIPT CropVolumeSelfTest.py)
endif()
