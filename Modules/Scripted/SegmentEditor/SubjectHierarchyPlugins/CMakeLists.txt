set(${MODULE_NAME}SubjectHierarchyPlugins_PYTHON_SCRIPTS
  ${MODULE_NAME}SubjectHierarchyPlugin
  )

set(${MODULE_NAME}SubjectHierarchyPlugins_PYTHON_RESOURCES
  )

ctkMacroCompilePythonScript(
  TARGET_NAME ${MODULE_NAME}SubjectHierarchyPlugins
  SCRIPTS "${${MODULE_NAME}SubjectHierarchyPlugins_PYTHON_SCRIPTS}"
  RESOURCES "${${MODULE_NAME}SubjectHierarchyPlugins_PYTHON_RESOURCES}"
  DESTINATION_DIR ${Cjyx_BINARY_DIR}/${Cjyx_QTSCRIPTEDMODULES_LIB_DIR}/SubjectHierarchyPlugins
  INSTALL_DIR ${Cjyx_INSTALL_QTSCRIPTEDMODULES_LIB_DIR}/SubjectHierarchyPlugins
  NO_INSTALL_SUBDIR
  )
