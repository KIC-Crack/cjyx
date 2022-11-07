#-----------------------------------------------------------------------------
set(MODULE_NAME "Segmentations")
set(MODULE_TITLE ${MODULE_NAME})

string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)

#-----------------------------------------------------------------------------
add_subdirectory(MRML)
add_subdirectory(Logic)
add_subdirectory(MRMLDM)
add_subdirectory(EditorEffects)
add_subdirectory(Widgets)
add_subdirectory(SubjectHierarchyPlugins)

#-----------------------------------------------------------------------------
set(MODULE_EXPORT_DIRECTIVE "Q_CJYX_QTMODULES_${MODULE_NAME_UPPER}_EXPORT")

set(MODULE_INCLUDE_DIRECTORIES
  ${vtkCjyxSegmentationsModuleMRML_INCLUDE_DIRS}
  ${vtkCjyxSegmentationsModuleLogic_INCLUDE_DIRS}
  ${vtkCjyxSegmentationsModuleMRMLDisplayableManager_INCLUDE_DIRS}
  ${qCjyxSegmentationsModuleWidgets_INCLUDE_DIRS}
  ${qCjyxSegmentationsEditorEffects_INCLUDE_DIRS}
  ${qCjyxSegmentationsSubjectHierarchyPlugins_INCLUDE_DIRS}
  ${qCjyxSubjectHierarchyModuleWidgets_INCLUDE_DIRS}
  ${qCjyxTerminologiesModuleWidgets_INCLUDE_DIRS}
  ${vtkCjyxTerminologiesModuleLogic_INCLUDE_DIRS}
  )

set(MODULE_SRCS
  qCjyx${MODULE_NAME}IOOptionsWidget.cxx
  qCjyx${MODULE_NAME}IOOptionsWidget.h
  qCjyx${MODULE_NAME}Module.cxx
  qCjyx${MODULE_NAME}Module.h
  qCjyx${MODULE_NAME}ModuleWidget.cxx
  qCjyx${MODULE_NAME}ModuleWidget.h
  qCjyx${MODULE_NAME}NodeWriter.cxx
  qCjyx${MODULE_NAME}NodeWriter.h
  qCjyx${MODULE_NAME}NodeWriterOptionsWidget.cxx
  qCjyx${MODULE_NAME}NodeWriterOptionsWidget.h
  qCjyx${MODULE_NAME}Reader.cxx
  qCjyx${MODULE_NAME}Reader.h
  qCjyx${MODULE_NAME}SettingsPanel.cxx
  qCjyx${MODULE_NAME}SettingsPanel.h
  )

set(MODULE_MOC_SRCS
  qCjyx${MODULE_NAME}IOOptionsWidget.h
  qCjyx${MODULE_NAME}Module.h
  qCjyx${MODULE_NAME}ModuleWidget.h
  qCjyx${MODULE_NAME}NodeWriter.h
  qCjyx${MODULE_NAME}NodeWriterOptionsWidget.h
  qCjyx${MODULE_NAME}Reader.h
  qCjyx${MODULE_NAME}SettingsPanel.h
  )

set(MODULE_UI_SRCS
  Resources/UI/qCjyx${MODULE_NAME}IOOptionsWidget.ui
  Resources/UI/qCjyx${MODULE_NAME}Module.ui
  Resources/UI/qCjyx${MODULE_NAME}SettingsPanel.ui
  )

set(MODULE_TARGET_LIBRARIES
  vtkCjyx${MODULE_NAME}ModuleMRML
  vtkCjyx${MODULE_NAME}ModuleLogic
  vtkCjyx${MODULE_NAME}ModuleMRMLDisplayableManager
  qCjyx${MODULE_NAME}ModuleWidgets
  qCjyx${MODULE_NAME}SubjectHierarchyPlugins
  qCjyx${MODULE_NAME}EditorEffects
  qCjyxSubjectHierarchyModuleWidgets
  qCjyxTerminologiesModuleWidgets
  vtkCjyxTerminologiesModuleLogic
  qCjyxBaseQTGUI
  )

set(MODULE_RESOURCES
  Resources/qCjyx${MODULE_NAME}Module.qrc
  )

#-----------------------------------------------------------------------------
cjyxMacroBuildLoadableModule(
  NAME ${MODULE_NAME}
  TITLE ${MODULE_TITLE}
  EXPORT_DIRECTIVE ${MODULE_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
  SRCS ${MODULE_SRCS}
  MOC_SRCS ${MODULE_MOC_SRCS}
  UI_SRCS ${MODULE_UI_SRCS}
  TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES}
  RESOURCES ${MODULE_RESOURCES}
  WITH_GENERIC_TESTS
  )

#-----------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
