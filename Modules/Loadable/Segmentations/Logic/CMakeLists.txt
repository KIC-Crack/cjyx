project(vtkCjyx${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_CJYX_${MODULE_NAME_UPPER}_LOGIC_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkCjyxSegmentationsModuleMRML_INCLUDE_DIRS}
  ${vtkCjyxTerminologiesModuleLogic_INCLUDE_DIRS}
  ${vtkCjyxMarkupsModuleMRML_INCLUDE_DIRS}
  )

set(${KIT}_SRCS
  vtkCjyx${MODULE_NAME}ModuleLogic.cxx
  vtkCjyx${MODULE_NAME}ModuleLogic.h
  vtkCjyxSegmentationGeometryLogic.cxx
  vtkCjyxSegmentationGeometryLogic.h
  vtkImageGrowCutSegment.cxx
  vtkImageGrowCutSegment.h
  FibHeap.cxx
  )

set(${KIT}_TARGET_LIBRARIES
  vtkCjyx${MODULE_NAME}ModuleMRML
  vtkCjyxTerminologiesModuleLogic
  vtkCjyxAnnotationsModuleMRML
  vtkCjyxMarkupsModuleMRML
  )

#-----------------------------------------------------------------------------
CjyxMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
