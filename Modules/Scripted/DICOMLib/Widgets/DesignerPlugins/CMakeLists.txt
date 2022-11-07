
set(KIT ${PROJECT_NAME}Plugins)

set(${KIT}_EXPORT "Q_CJYX_MODULE_${MODULE_NAME_UPPER}_WIDGETS_PLUGINS_EXPORT")

set(${KIT}_SRCS
  qCjyxDICOMLibModuleWidgetsAbstractPlugin.cxx
  qCjyxDICOMLibModuleWidgetsAbstractPlugin.h
  qCjyxDICOMLibModuleWidgetsPlugin.cxx
  qCjyxDICOMLibModuleWidgetsPlugin.h
  qCjyxDICOMTagEditorWidgetPlugin.cxx
  qCjyxDICOMTagEditorWidgetPlugin.h
  )

set(${KIT}_MOC_SRCS
  qCjyxDICOMLibModuleWidgetsPlugin.h
  qCjyxDICOMTagEditorWidgetPlugin.h
  )

set(${KIT}_TARGET_LIBRARIES
  qCjyxDICOMLibModuleWidgets
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
