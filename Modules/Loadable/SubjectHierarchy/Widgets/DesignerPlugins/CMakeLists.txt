
set(KIT ${PROJECT_NAME}Plugins)

set(${KIT}_EXPORT "Q_CJYX_MODULE_${MODULE_NAME_UPPER}_WIDGETS_PLUGINS_EXPORT")

set(${KIT}_SRCS
  qCjyxSubjectHierarchyModuleWidgetsAbstractPlugin.cxx
  qCjyxSubjectHierarchyModuleWidgetsAbstractPlugin.h
  qCjyxSubjectHierarchyModuleWidgetsPlugin.cxx
  qCjyxSubjectHierarchyModuleWidgetsPlugin.h
  qMRMLSubjectHierarchyTreeViewPlugin.cxx
  qMRMLSubjectHierarchyTreeViewPlugin.h
  qMRMLSubjectHierarchyComboBoxPlugin.cxx
  qMRMLSubjectHierarchyComboBoxPlugin.h
  )

set(${KIT}_MOC_SRCS
  qCjyxSubjectHierarchyModuleWidgetsPlugin.h
  qMRMLSubjectHierarchyTreeViewPlugin.h
  qMRMLSubjectHierarchyComboBoxPlugin.h
  )

set(${KIT}_TARGET_LIBRARIES
  qCjyxSubjectHierarchyModuleWidgets
  )

#-----------------------------------------------------------------------------
ctkMacroBuildQtDesignerPlugin(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT}
  FOLDER "Module-${MODULE_NAME}"
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
