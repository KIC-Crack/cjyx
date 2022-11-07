
if(Cjyx_USE_NUMPY AND PYTHON_EXECUTABLE)

  add_test(PythonCjyxSimpleNUMPYTest ${Cjyx_LAUNCH_COMMAND} ${PYTHON_EXECUTABLE} ${Cjyx_SOURCE_DIR}/Testing/SimpleNUMPYTest.py)

endif()
