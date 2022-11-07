project(qCjyx${MODULE_NAME}ModuleWidgets)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "Q_CJYX_QTMODULES_${MODULE_NAME_UPPER}_WIDGETS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  )

set(${KIT}_SRCS
  qCjyxDiffusionTensorVolumeDisplayWidget.cxx
  qCjyxDiffusionTensorVolumeDisplayWidget.h
  qCjyxDiffusionWeightedVolumeDisplayWidget.cxx
  qCjyxDiffusionWeightedVolumeDisplayWidget.h
  qCjyxLabelMapVolumeDisplayWidget.cxx
  qCjyxLabelMapVolumeDisplayWidget.h
  qCjyxScalarVolumeDisplayWidget.cxx
  qCjyxScalarVolumeDisplayWidget.h
  qCjyxVolumeDisplayWidget.cxx
  qCjyxVolumeDisplayWidget.h
  qCjyxDTISliceDisplayWidget.cxx
  qCjyxDTISliceDisplayWidget.h
  )

set(${KIT}_MOC_SRCS
  qCjyxDiffusionTensorVolumeDisplayWidget.h
  qCjyxDiffusionWeightedVolumeDisplayWidget.h
  qCjyxLabelMapVolumeDisplayWidget.h
  qCjyxScalarVolumeDisplayWidget.h
  qCjyxVolumeDisplayWidget.h
  qCjyxDTISliceDisplayWidget.h
  )

set(${KIT}_UI_SRCS
  Resources/UI/qCjyxDiffusionTensorVolumeDisplayWidget.ui
  Resources/UI/qCjyxDiffusionWeightedVolumeDisplayWidget.ui
  Resources/UI/qCjyxDTISliceDisplayWidget.ui
  Resources/UI/qCjyxLabelMapVolumeDisplayWidget.ui
  Resources/UI/qCjyxScalarVolumeDisplayWidget.ui
  )

set(${KIT}_RESOURCES
  Resources/qCjyx${MODULE_NAME}WidgetsModule.qrc
  )

set(${KIT}_TARGET_LIBRARIES
  vtkCjyxVolumesModuleLogic
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
  add_subdirectory(Testing)
endif()
