
set(KIT ${PROJECT_NAME}Plugins)

set(${KIT}_EXPORT "Q_CJYX_MODULE_${MODULE_NAME_UPPER}_WIDGETS_PLUGINS_EXPORT")

set(${KIT}_SRCS
  qCjyx${MODULE_NAME}ModuleWidgetsAbstractPlugin.cxx
  qCjyx${MODULE_NAME}ModuleWidgetsAbstractPlugin.h
  qCjyx${MODULE_NAME}ModuleWidgetsPlugin.cxx
  qCjyx${MODULE_NAME}ModuleWidgetsPlugin.h
  qMRML${MODULE_NAME}DisplayNodeWidgetPlugin.cxx
  qMRML${MODULE_NAME}DisplayNodeWidgetPlugin.h
  qMRML${MODULE_NAME}FiducialProjectionPropertyWidgetPlugin.cxx
  qMRML${MODULE_NAME}FiducialProjectionPropertyWidgetPlugin.h
  qMRML${MODULE_NAME}InteractionHandleWidgetPlugin.cxx
  qMRML${MODULE_NAME}InteractionHandleWidgetPlugin.h
  qMRML${MODULE_NAME}ROIWidgetPlugin.cxx
  qMRML${MODULE_NAME}ROIWidgetPlugin.h
  qCjyx${MODULE_NAME}PlaceWidgetPlugin.cxx
  qCjyx${MODULE_NAME}PlaceWidgetPlugin.h
  qCjyxSimpleMarkupsWidgetPlugin.cxx
  qCjyxSimpleMarkupsWidgetPlugin.h
  )

set(${KIT}_MOC_SRCS
  qCjyx${MODULE_NAME}ModuleWidgetsPlugin.h
  qMRML${MODULE_NAME}DisplayNodeWidgetPlugin.h
  qMRML${MODULE_NAME}FiducialProjectionPropertyWidgetPlugin.h
  qMRML${MODULE_NAME}InteractionHandleWidgetPlugin.h
  qMRML${MODULE_NAME}ROIWidgetPlugin.h
  qCjyxMarkupsPlaceWidgetPlugin.h
  qCjyxSimpleMarkupsWidgetPlugin.h
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
