set(KIT ${PROJECT_NAME}Plugins)

set(${KIT}_EXPORT "Q_CJYX_MODULE_${MODULE_NAME_UPPER}_WIDGETS_PLUGINS_EXPORT")

set(${KIT}_SRCS
  qCjyxTerminologiesModuleWidgetsAbstractPlugin.cxx
  qCjyxTerminologiesModuleWidgetsAbstractPlugin.h
  qCjyxTerminologiesModuleWidgetsPlugin.cxx
  qCjyxTerminologiesModuleWidgetsPlugin.h
  qCjyxTerminologyNavigatorWidgetPlugin.cxx
  qCjyxTerminologyNavigatorWidgetPlugin.h
  )

set(${KIT}_MOC_SRCS
  qCjyxTerminologiesModuleWidgetsPlugin.h
  qCjyxTerminologyNavigatorWidgetPlugin.h
  )

set(${KIT}_TARGET_LIBRARIES
  qCjyxTerminologiesModuleWidgets
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
