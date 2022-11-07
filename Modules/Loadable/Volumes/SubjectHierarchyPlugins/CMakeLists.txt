project(qCjyx${MODULE_NAME}SubjectHierarchyPlugins)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "Q_CJYX_${MODULE_NAME_UPPER}_SUBJECT_HIERARCHY_PLUGINS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${qCjyxSubjectHierarchyModuleWidgets_INCLUDE_DIRS}
  ${vtkCjyxSubjectHierarchyModuleLogic_INCLUDE_DIRS}
  ${qMRMLWidgets_INCLUDE_DIRS}
  ${MRMLLogic_INCLUDE_DIRS}
  ${MRMLCore_INCLUDE_DIRS}
  ${vtkCjyxVolumesModuleLogic_INCLUDE_DIRS}
  ${vtkCjyxColorsModuleLogic_INCLUDE_DIRS}
  ${vtkCjyxColorsModuleMRML_INCLUDE_DIRS}
  )

set(${KIT}_SRCS
  qCjyxSubjectHierarchyVolumesPlugin.cxx
  qCjyxSubjectHierarchyVolumesPlugin.h
  qCjyxSubjectHierarchyLabelMapsPlugin.cxx
  qCjyxSubjectHierarchyLabelMapsPlugin.h
  qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin.cxx
  qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin.h
  )

set(${KIT}_MOC_SRCS
  qCjyxSubjectHierarchyVolumesPlugin.h
  qCjyxSubjectHierarchyLabelMapsPlugin.h
  qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin.h
  )

set(${KIT}_UI_SRCS
  )

set(${KIT}_RESOURCES
  Resources/${KIT}.qrc
  )

#-----------------------------------------------------------------------------
set(${KIT}_TARGET_LIBRARIES
  qCjyxSubjectHierarchyModuleWidgets
  vtkCjyxSubjectHierarchyModuleLogic
  vtkCjyxColorsModuleLogic
  qMRMLWidgets
  MRMLLogic
  MRMLCore
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
