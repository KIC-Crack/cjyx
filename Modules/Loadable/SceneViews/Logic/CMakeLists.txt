project(vtkCjyx${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_CJYX_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/../GUI
  ${CMAKE_CURRENT_BINARY_DIR}/..
  )

set(${KIT}_SRCS
  vtkCjyx${MODULE_NAME}ModuleLogic.cxx
  vtkCjyx${MODULE_NAME}ModuleLogic.h
  )

set(${KIT}_TARGET_LIBRARIES
  )

#-----------------------------------------------------------------------------
CjyxMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
