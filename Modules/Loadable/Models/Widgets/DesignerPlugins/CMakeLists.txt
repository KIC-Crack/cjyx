
set(KIT ${PROJECT_NAME}Plugins)

set(${KIT}_EXPORT "Q_CJYX_QTMODULES_${MODULE_NAME_UPPER}_WIDGETS_PLUGINS_EXPORT")

set(${KIT}_SRCS
  qCjyx${MODULE_NAME}WidgetsAbstractPlugin.cxx
  qCjyx${MODULE_NAME}WidgetsAbstractPlugin.h
  qCjyx${MODULE_NAME}WidgetsPlugin.cxx
  qCjyx${MODULE_NAME}WidgetsPlugin.h

  qMRMLModelDisplayNodeWidgetPlugin.cxx
  qMRMLModelDisplayNodeWidgetPlugin.h
  )

set(${KIT}_MOC_SRCS
  qCjyx${MODULE_NAME}WidgetsPlugin.h

  qMRMLModelDisplayNodeWidgetPlugin.h
  )

set(${KIT}_TARGET_LIBRARIES
  qCjyx${MODULE_NAME}ModuleWidgets
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
