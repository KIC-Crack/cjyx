
set(KIT ${PROJECT_NAME}Plugins)

set(${KIT}_EXPORT "Q_CJYX_MODULE_${MODULE_NAME_UPPER}_WIDGETS_PLUGINS_EXPORT")

set(${KIT}_SRCS
  qCjyxAnnotationModuleWidgetsAbstractPlugin.cxx
  qCjyxAnnotationModuleWidgetsAbstractPlugin.h
  qCjyxAnnotationModuleWidgetsPlugin.cxx
  qCjyxAnnotationModuleWidgetsPlugin.h

  qMRMLAnnotationFiducialProjectionPropertyWidgetPlugin.cxx
  qMRMLAnnotationFiducialProjectionPropertyWidgetPlugin.h
  qMRMLAnnotationROIWidgetPlugin.cxx
  qMRMLAnnotationROIWidgetPlugin.h
  qMRMLAnnotationRulerProjectionPropertyWidgetPlugin.cxx
  qMRMLAnnotationRulerProjectionPropertyWidgetPlugin.h
  qMRMLAnnotationTreeViewPlugin.cxx
  qMRMLAnnotationTreeViewPlugin.h
  )

set(${KIT}_MOC_SRCS
  qCjyxAnnotationModuleWidgetsPlugin.h

  qMRMLAnnotationFiducialProjectionPropertyWidgetPlugin.h
  qMRMLAnnotationROIWidgetPlugin.h
  qMRMLAnnotationRulerProjectionPropertyWidgetPlugin.h
  qMRMLAnnotationTreeViewPlugin.h
  )

set(${KIT}_TARGET_LIBRARIES
  qCjyxAnnotationsModuleWidgets
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
