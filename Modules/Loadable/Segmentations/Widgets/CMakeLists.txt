project(qCjyx${MODULE_NAME}ModuleWidgets)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "Q_CJYX_MODULE_${MODULE_NAME_UPPER}_WIDGETS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkCjyxSegmentationsModuleMRML_INCLUDE_DIRS}
  ${vtkCjyxSegmentationsModuleLogic_INCLUDE_DIRS}
  ${qCjyxSegmentationsEditorEffects_INCLUDE_DIRS}
  ${qCjyxTerminologiesModuleWidgets_INCLUDE_DIRS}
  )

set(${KIT}_SRCS
  qMRMLDoubleSpinBoxDelegate.h
  qMRMLDoubleSpinBoxDelegate.cxx
  qMRMLSegmentsModel.cxx
  qMRMLSegmentsModel.h
  qMRMLSegmentsModel_p.h
  qMRMLSegmentsTableView.cxx
  qMRMLSegmentsTableView.h
  qMRMLSegmentationConversionParametersWidget.cxx
  qMRMLSegmentationConversionParametersWidget.h
  qMRMLSegmentationDisplayNodeWidget.cxx
  qMRMLSegmentationDisplayNodeWidget.h
  qMRMLSegmentationFileExportWidget.cxx
  qMRMLSegmentationFileExportWidget.h
  qMRMLSegmentationGeometryWidget.cxx
  qMRMLSegmentationGeometryWidget.h
  qMRMLSegmentationGeometryDialog.cxx
  qMRMLSegmentationGeometryDialog.h
  qMRMLSegmentationRepresentationsListView.cxx
  qMRMLSegmentationRepresentationsListView.h
  qMRMLSegmentationShow3DButton.h
  qMRMLSegmentationShow3DButton.cxx
  qMRMLSegmentEditorWidget.cxx
  qMRMLSegmentEditorWidget.h
  qMRMLSortFilterSegmentsProxyModel.cxx
  qMRMLSortFilterSegmentsProxyModel.h
  )

set(${KIT}_MOC_SRCS
  qMRMLSegmentsTableView.h
  qMRMLSegmentsModel.h
  qMRMLSegmentationConversionParametersWidget.h
  qMRMLSegmentationDisplayNodeWidget.h
  qMRMLSegmentationFileExportWidget.h
  qMRMLSegmentationGeometryWidget.h
  qMRMLSegmentationGeometryDialog.h
  qMRMLSegmentationRepresentationsListView.h
  qMRMLSegmentationShow3DButton.h
  qMRMLSegmentEditorWidget.h
  qMRMLSortFilterSegmentsProxyModel.h
  qMRMLDoubleSpinBoxDelegate.h
)

set(${KIT}_UI_SRCS
  Resources/UI/qMRMLSegmentationRepresentationsListView.ui
  Resources/UI/qMRMLSegmentationConversionParametersWidget.ui
  Resources/UI/qMRMLSegmentEditorWidget.ui
  Resources/UI/qMRMLSegmentationDisplayNodeWidget.ui
  Resources/UI/qMRMLSegmentationFileExportWidget.ui
  Resources/UI/qMRMLSegmentationGeometryWidget.ui
  Resources/UI/qMRMLSegmentsTableView.ui
  )

set(${KIT}_RESOURCES
  Resources/${KIT}.qrc
  )

set(${KIT}_TARGET_LIBRARIES
  qCjyxSegmentationsEditorEffects
  qCjyxTerminologiesModuleWidgets
  vtkCjyxSegmentationsModuleMRML
  vtkCjyxSegmentationsModuleLogic
  )

#-----------------------------------------------------------------------------
CjyxMacroBuildModuleWidgets(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  UI_SRCS ${${KIT}_UI_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  RESOURCES ${${KIT}_RESOURCES}
  WRAP_PYTHONQT
  )

#-----------------------------------------------------------------------------
if(Cjyx_BUILD_QT_DESIGNER_PLUGINS)
  add_subdirectory(DesignerPlugins)
endif()

#-----------------------------------------------------------------------------
if(BUILD_TESTING)
  #add_subdirectory(Testing)
endif()
