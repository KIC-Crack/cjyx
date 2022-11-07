project(qCjyx${MODULE_NAME}ModuleWidgets)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "Q_CJYX_MODULE_${MODULE_NAME_UPPER}_WIDGETS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkCjyx${MODULE_NAME}ModuleMRML_SOURCE_DIR}
  ${vtkCjyx${MODULE_NAME}ModuleMRML_BINARY_DIR}
  )

set(${KIT}_SRCS
  qCjyxCPURayCast${MODULE_NAME}PropertiesWidget.cxx
  qCjyxCPURayCast${MODULE_NAME}PropertiesWidget.h
  qCjyxGPURayCast${MODULE_NAME}PropertiesWidget.cxx
  qCjyxGPURayCast${MODULE_NAME}PropertiesWidget.h
  qCjyxMulti${MODULE_NAME}PropertiesWidget.cxx
  qCjyxMulti${MODULE_NAME}PropertiesWidget.h
  qCjyxPresetComboBox.cxx
  qCjyxPresetComboBox.h
  qCjyxPresetComboBox_p.h
  qCjyx${MODULE_NAME}ModuleWidget.cxx
  qCjyx${MODULE_NAME}ModuleWidget.h
  qCjyx${MODULE_NAME}PropertiesWidget.cxx
  qCjyx${MODULE_NAME}PropertiesWidget.h
  qCjyx${MODULE_NAME}PresetComboBox.cxx
  qCjyx${MODULE_NAME}PresetComboBox.h
  qMRMLVolumePropertyNodeWidget.cxx
  qMRMLVolumePropertyNodeWidget.h
  qCjyxGPUMemoryComboBox.cxx
  qCjyxGPUMemoryComboBox.h
  )

set(${KIT}_MOC_SRCS
  qCjyxCPURayCast${MODULE_NAME}PropertiesWidget.h
  qCjyxGPURayCast${MODULE_NAME}PropertiesWidget.h
  qCjyxMulti${MODULE_NAME}PropertiesWidget.h
  qCjyxPresetComboBox.h
  qCjyxPresetComboBox_p.h
  qCjyx${MODULE_NAME}ModuleWidget.h
  qCjyx${MODULE_NAME}PropertiesWidget.h
  qCjyx${MODULE_NAME}PresetComboBox.h
  qMRMLVolumePropertyNodeWidget.h
  qCjyxGPUMemoryComboBox.h
  )

set(${KIT}_UI_SRCS
  ../Resources/UI/qMRMLVolumePropertyNodeWidget.ui
  ../Resources/UI/qCjyxCPURayCast${MODULE_NAME}PropertiesWidget.ui
  ../Resources/UI/qCjyxGPURayCast${MODULE_NAME}PropertiesWidget.ui
  ../Resources/UI/qCjyxMulti${MODULE_NAME}PropertiesWidget.ui
  ../Resources/UI/qCjyx${MODULE_NAME}ModuleWidget.ui
  ../Resources/UI/qCjyx${MODULE_NAME}PresetComboBox.ui
  )

set(${KIT}_RESOURCES
  )

set(${KIT}_TARGET_LIBRARIES
  vtkCjyx${MODULE_NAME}ModuleLogic
  vtkCjyx${MODULE_NAME}ModuleMRML
  qCjyxAnnotationsModuleWidgets
  qCjyxMarkupsModuleWidgets
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
