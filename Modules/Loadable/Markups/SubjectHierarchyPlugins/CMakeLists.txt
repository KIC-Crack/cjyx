project(qCjyx${MODULE_NAME}SubjectHierarchyPlugins)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "Q_CJYX_${MODULE_NAME_UPPER}_SUBJECT_HIERARCHY_PLUGINS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkCjyxMarkupsModuleMRML_INCLUDE_DIRS}
  ${vtkCjyxMarkupsModuleLogic_INCLUDE_DIRS}
  ${vtkCjyxMarkupsModuleVTKWidgets_INCLUDE_DIRS}
  ${qCjyxSubjectHierarchyModuleWidgets_INCLUDE_DIRS}
  ${vtkCjyxSubjectHierarchyModuleLogic_INCLUDE_DIRS}
  ${qCjyxTerminologiesModuleWidgets_INCLUDE_DIRS}
  ${vtkCjyxTerminologiesModuleLogic_INCLUDE_DIRS}
  ${vtkSegmentationCore_INCLUDE_DIRS}
  ${qMRMLWidgets_INCLUDE_DIRS}
  ${MRMLLogic_INCLUDE_DIRS}
  ${MRMLCore_INCLUDE_DIRS}
  ${MRMLDisplayableManager_INCLUDE_DIRS}
  )

set(${KIT}_SRCS
  qCjyxSubjectHierarchyMarkupsPlugin.cxx
  qCjyxSubjectHierarchyMarkupsPlugin.h
  )

set(${KIT}_MOC_SRCS
  qCjyxSubjectHierarchyMarkupsPlugin.h
  )

set(${KIT}_UI_SRCS
  )

#-----------------------------------------------------------------------------
set(${KIT}_TARGET_LIBRARIES
  vtkCjyx${MODULE_NAME}ModuleMRML
  vtkCjyx${MODULE_NAME}ModuleVTKWidgets
  vtkCjyx${MODULE_NAME}ModuleLogic
  qCjyxSubjectHierarchyModuleWidgets
  vtkCjyxSubjectHierarchyModuleLogic
  qCjyxTerminologiesModuleWidgets
  qMRMLWidgets
  MRMLLogic
  MRMLCore
  MRMLDisplayableManager
  )

#-----------------------------------------------------------------------------
CjyxMacroBuildModuleQtLibrary(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  FOLDER "Module-${MODULE_NAME}"
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  UI_SRCS ${${KIT}_UI_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  RESOURCES ${${KIT}_RESOURCES}
  WRAP_PYTHONQT
  )
