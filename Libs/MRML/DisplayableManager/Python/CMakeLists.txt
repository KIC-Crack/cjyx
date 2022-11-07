

set(displayableManager_PYTHON_SCRIPTS
  mrmlDisplayableManager/vtkScriptedExampleDisplayableManager
  )

ctkMacroCompilePythonScript(
  TARGET_NAME mrmlDisplayableManager
  SCRIPTS "${displayableManager_PYTHON_SCRIPTS}"
  DESTINATION_DIR ${Cjyx_BINARY_DIR}/bin/Python
  INSTALL_DIR ${Cjyx_INSTALL_BIN_DIR}
  )
