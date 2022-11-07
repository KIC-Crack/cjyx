if(Cjyx_USE_QtTesting AND Cjyx_USE_PYTHONQT)

  # add tests
  cjyx_add_python_unittest(SCRIPT VolumeRenderingSceneClose.py)
  cjyx_add_python_unittest(SCRIPT VolumeRenderingThreeDOnlyLayout.py)
  set_tests_properties(
    py_VolumeRenderingThreeDOnlyLayout
    PROPERTIES FAIL_REGULAR_EXPRESSION "OpenGL errors detected"
    )
  set(KIT_PYTHON_SCRIPTS
    VolumeRenderingSceneClose.py
     )
  ctkMacroCompilePythonScript(
    TARGET_NAME ApplicationSelfTests
    SCRIPTS "${KIT_PYTHON_SCRIPTS}"
    DESTINATION_DIR ${Cjyx_BINARY_DIR}/${Cjyx_QTSCRIPTEDMODULES_LIB_DIR}
    INSTALL_DIR ${Cjyx_INSTALL_QTSCRIPTEDMODULES_LIB_DIR}
    NO_INSTALL_SUBDIR
    )

endif()
