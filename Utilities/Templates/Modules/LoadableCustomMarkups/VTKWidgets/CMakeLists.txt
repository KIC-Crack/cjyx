project(vtkCjyx${MODULE_NAME}ModuleVTKWidgets)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_CJYX_${MODULE_NAME_UPPER}_MODULE_VTKWIDGETS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkCjyx${MODULE_NAME}ModuleMRML_SOURCE_DIR}
  ${vtkCjyx${MODULE_NAME}ModuleMRML_BINARY_DIR}
  ${vtkCjyxMarkupsModuleVTKWidgets_INCLUDE_DIRS}
  ${vtkCjyxMarkupsModuleVTKWidgets_INCLUDE_DIRS}
  )

set(${KIT}_SRCS
  vtkCjyxTestLineWidget.h
  vtkCjyxTestLineWidget.cxx
  vtkCjyxTestLineRepresentation3D.h
  vtkCjyxTestLineRepresentation3D.cxx
  vtkCjyxTestLineRepresentation2D.h
  vtkCjyxTestLineRepresentation2D.cxx
  )

set(${KIT}_TARGET_LIBRARIES
  vtkCjyx${MODULE_NAME}ModuleMRML
  vtkCjyxMarkupsModuleVTKWidgets
  )

#-----------------------------------------------------------------------------
CjyxMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
