# Deploy abstract scripted plugin and init file that
# imports all deployed scripted plugins deployed by modules
set(SubjectHierarchyWidgets_PYTHON_SCRIPTS
  __init__
  AbstractScriptedSubjectHierarchyPlugin
  )

set(SubjectHierarchyWidgets_PYTHON_RESOURCES
  )

ctkMacroCompilePythonScript(
  TARGET_NAME SubjectHierarchyWidgets
  SCRIPTS "${SubjectHierarchyWidgets_PYTHON_SCRIPTS}"
  RESOURCES "${SubjectHierarchyWidgets_PYTHON_RESOURCES}"
  DESTINATION_DIR ${Cjyx_BINARY_DIR}/${Cjyx_QTSCRIPTEDMODULES_LIB_DIR}/SubjectHierarchyPlugins
  INSTALL_DIR ${Cjyx_INSTALL_QTSCRIPTEDMODULES_LIB_DIR}/SubjectHierarchyPlugins
  NO_INSTALL_SUBDIR
  )
