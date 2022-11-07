project(vtkCjyx${MODULE_NAME}ModuleVTKWidgets)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_CJYX_${MODULE_NAME_UPPER}_MODULE_VTKWIDGETS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkCjyx${MODULE_NAME}ModuleMRML_SOURCE_DIR}
  ${vtkCjyx${MODULE_NAME}ModuleMRML_BINARY_DIR}
  )

set(${KIT}_SRCS
  vtk${MODULE_NAME}GlyphSource2D.cxx
  vtk${MODULE_NAME}GlyphSource2D.h
  vtkFastSelectVisiblePoints.cxx
  vtkFastSelectVisiblePoints.h
  vtkCjyxMarkupsWidgetRepresentation.cxx
  vtkCjyxMarkupsWidgetRepresentation.h
  vtkCjyxMarkupsWidgetRepresentation3D.cxx
  vtkCjyxMarkupsWidgetRepresentation3D.h
  vtkCjyxMarkupsWidgetRepresentation2D.cxx
  vtkCjyxMarkupsWidgetRepresentation2D.h
  vtkCjyxMarkupsWidget.cxx
  vtkCjyxMarkupsWidget.h
  vtkCjyxAngleRepresentation2D.cxx
  vtkCjyxAngleRepresentation2D.h
  vtkCjyxAngleRepresentation3D.cxx
  vtkCjyxAngleRepresentation3D.h
  vtkCjyxAngleWidget.cxx
  vtkCjyxAngleWidget.h
  vtkCjyxCurveRepresentation2D.cxx
  vtkCjyxCurveRepresentation2D.h
  vtkCjyxCurveRepresentation3D.cxx
  vtkCjyxCurveRepresentation3D.h
  vtkCjyxCurveWidget.cxx
  vtkCjyxCurveWidget.h
  vtkCjyxLineRepresentation3D.cxx
  vtkCjyxLineRepresentation3D.h
  vtkCjyxLineRepresentation2D.cxx
  vtkCjyxLineRepresentation2D.h
  vtkCjyxLineWidget.cxx
  vtkCjyxLineWidget.h
  vtkCjyxPointsRepresentation3D.cxx
  vtkCjyxPointsRepresentation3D.h
  vtkCjyxPointsRepresentation2D.cxx
  vtkCjyxPointsRepresentation2D.h
  vtkCjyxPointsWidget.cxx
  vtkCjyxPointsWidget.h
  vtkCjyxPlaneRepresentation2D.cxx
  vtkCjyxPlaneRepresentation2D.h
  vtkCjyxPlaneRepresentation3D.cxx
  vtkCjyxPlaneRepresentation3D.h
  vtkCjyxPlaneWidget.cxx
  vtkCjyxPlaneWidget.h
  vtkCjyxROIRepresentation2D.cxx
  vtkCjyxROIRepresentation2D.h
  vtkCjyxROIRepresentation3D.cxx
  vtkCjyxROIRepresentation3D.h
  vtkCjyxROIWidget.cxx
  vtkCjyxROIWidget.h
  )

set(${KIT}_TARGET_LIBRARIES
  vtkCjyx${MODULE_NAME}ModuleMRML
  )

#-----------------------------------------------------------------------------
CjyxMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
