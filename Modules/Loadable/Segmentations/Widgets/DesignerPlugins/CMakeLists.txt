set(KIT ${PROJECT_NAME}Plugins)

set(${KIT}_EXPORT "Q_CJYX_MODULE_${MODULE_NAME_UPPER}_WIDGETS_PLUGINS_EXPORT")

set(${KIT}_SRCS
  qCjyxSegmentationsModuleWidgetsAbstractPlugin.cxx
  qCjyxSegmentationsModuleWidgetsAbstractPlugin.h
  qCjyxSegmentationsModuleWidgetsPlugin.cxx
  qCjyxSegmentationsModuleWidgetsPlugin.h
  qMRMLSegmentsTableViewPlugin.cxx
  qMRMLSegmentsTableViewPlugin.h
  qMRMLSegmentationRepresentationsListViewPlugin.cxx
  qMRMLSegmentationRepresentationsListViewPlugin.h
  qMRMLSegmentationConversionParametersWidgetPlugin.cxx
  qMRMLSegmentationConversionParametersWidgetPlugin.h
  qMRMLSegmentationShow3DButtonPlugin.cxx
  qMRMLSegmentationShow3DButtonPlugin.h
  qMRMLSegmentSelectorWidgetPlugin.cxx
  qMRMLSegmentSelectorWidgetPlugin.h
  qMRMLSegmentEditorWidgetPlugin.cxx
  qMRMLSegmentEditorWidgetPlugin.h
  qMRMLSegmentationDisplayNodeWidgetPlugin.cxx
  qMRMLSegmentationDisplayNodeWidgetPlugin.h
  qMRMLSegmentationFileExportWidgetPlugin.cxx
  qMRMLSegmentationFileExportWidgetPlugin.h
  )

set(${KIT}_MOC_SRCS
  qCjyxSegmentationsModuleWidgetsPlugin.h
  qMRMLSegmentsTableViewPlugin.h
  qMRMLSegmentationRepresentationsListViewPlugin.h
  qMRMLSegmentationConversionParametersWidgetPlugin.h
  qMRMLSegmentSelectorWidgetPlugin.h
  qMRMLSegmentEditorWidgetPlugin.h
  qMRMLSegmentationDisplayNodeWidgetPlugin.h
  qMRMLSegmentationFileExportWidgetPlugin.h
  qMRMLSegmentationShow3DButtonPlugin.h
  )

set(${KIT}_TARGET_LIBRARIES
  qCjyxSegmentationsModuleWidgets
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
