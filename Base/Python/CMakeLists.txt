
set(Cjyx_PYTHON_SCRIPTS
  cjyx/__init__
  cjyx/logic
  cjyx/ScriptedLoadableModule
  cjyx/cjyxqt
  cjyx/testing
  cjyx/util
  mrml
  vtkAddon
  vtkITK
  vtkSegmentationCore
  vtkTeem
  )

set(Cjyx_PYTHON_MODULES_CONFIG "
# cjyx libs
'mrml',
'vtkAddon',
'vtkSegmentationCore',
'qMRMLWidgetsPythonQt',
# cjyx base libs
'logic',
'qCjyxBaseQTCorePythonQt',
'qCjyxBaseQTGUIPythonQt',
'qCjyxBaseQTAppPythonQt'"
  )

if(Cjyx_BUILD_CLI_SUPPORT)
  list(APPEND Cjyx_PYTHON_SCRIPTS
    cjyx/cli
    )
  set(Cjyx_PYTHON_MODULES_CONFIG "${Cjyx_PYTHON_MODULES_CONFIG},
# CLI logic (Cjyx_BUILD_CLI_SUPPORT:ON)
'qCjyxBaseQTCLIPython',
'qCjyxBaseQTCLIPythonQt',
"
  )
endif()

configure_file(
  cjyx/kits.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/cjyx/kits.py
  @ONLY
  )

list(APPEND Cjyx_PYTHON_SCRIPTS ${CMAKE_CURRENT_BINARY_DIR}/cjyx/kits.py)

set(Cjyx_PYTHON_RESOURCES
  )

if(Cjyx_USE_SimpleITK)
  list(APPEND Cjyx_PYTHON_SCRIPTS
    sitkUtils
    )
endif()

ctkMacroCompilePythonScript(
  TARGET_NAME CjyxBase
  SCRIPTS "${Cjyx_PYTHON_SCRIPTS}"
  RESOURCES "${Cjyx_PYTHON_RESOURCES}"
  DESTINATION_DIR ${Cjyx_BINARY_DIR}/bin/Python
  INSTALL_DIR ${Cjyx_INSTALL_BIN_DIR}
  )
