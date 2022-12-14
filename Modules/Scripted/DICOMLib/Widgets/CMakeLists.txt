project(qCjyx${MODULE_NAME}ModuleWidgets)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "Q_CJYX_MODULE_${MODULE_NAME_UPPER}_WIDGETS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${qCjyxSubjectHierarchyModuleWidgets_INCLUDE_DIRS}
  ${vtkCjyxDICOMLibModuleLogic_INCLUDE_DIRS}
  )

set(${KIT}_SRCS
  qCjyxDICOMExportDialog.cxx
  qCjyxDICOMExportDialog.h
  qCjyxDICOMLoadable.cxx
  qCjyxDICOMLoadable.h
  qCjyxDICOMExportable.cxx
  qCjyxDICOMExportable.h
  qCjyxDICOMTagEditorWidget.cxx
  qCjyxDICOMTagEditorWidget.h
  )

set(${KIT}_MOC_SRCS
  qCjyxDICOMExportDialog.h
  qCjyxDICOMLoadable.h
  qCjyxDICOMExportable.h
  qCjyxDICOMTagEditorWidget.h
  )

set(${KIT}_UI_SRCS
  Resources/UI/qCjyxDICOMExportDialog.ui
  )

set(${KIT}_RESOURCES
  #Resources/${KIT}.qrc
  )

#-----------------------------------------------------------------------------
set(${KIT}_TARGET_LIBRARIES
  qCjyxSubjectHierarchyModuleWidgets
  vtkCjyxDICOMLibModuleLogic
  MRMLCore
  MRMLLogic
  ${QT_LIBRARIES}
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
