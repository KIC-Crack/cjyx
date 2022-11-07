
#-----------------------------------------------------------------------------
set(MODULE_NAME "Annotations")
set(MODULE_TITLE "${MODULE_NAME}")

string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)

#-----------------------------------------------------------------------------
add_subdirectory(MRML)
add_subdirectory(VTKWidgets)
add_subdirectory(Logic)
add_subdirectory(MRMLDM)
add_subdirectory(Widgets)
if(Cjyx_USE_PYTHONQT)
  add_subdirectory(SubjectHierarchyPlugins)
endif()

#-----------------------------------------------------------------------------
set(MODULE_EXPORT_DIRECTIVE "Q_CJYX_QTMODULES_${MODULE_NAME_UPPER}_EXPORT")

# Additional includes - Current_{source,binary} and Cjyx_{Libs,Base} already included
set(MODULE_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Logic
  ${CMAKE_CURRENT_BINARY_DIR}/Logic
  ${CMAKE_CURRENT_SOURCE_DIR}/MRML
  ${CMAKE_CURRENT_BINARY_DIR}/MRML
  ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
  ${CMAKE_CURRENT_BINARY_DIR}/Widgets
  )

set(GUI_SRCS
  GUI/qCjyxAnnotationModuleWidget.cxx
  GUI/qCjyxAnnotationModuleWidget.h
  GUI/qCjyxAnnotationModulePropertyDialog.cxx
  GUI/qCjyxAnnotationModulePropertyDialog.h
  GUI/qCjyxAnnotationModuleReportDialog.cxx
  GUI/qCjyxAnnotationModuleReportDialog.h
  GUI/qCjyxAnnotationModuleSnapShotDialog.cxx
  GUI/qCjyxAnnotationModuleSnapShotDialog.h
  )

set(MODULE_SRCS
  ${GUI_SRCS}
  qCjyxAnnotationsIOOptionsWidget.cxx
  qCjyxAnnotationsIOOptionsWidget.h
  qCjyxAnnotationsModule.cxx
  qCjyxAnnotationsModule.h
  qCjyxAnnotationsReader.cxx
  qCjyxAnnotationsReader.h
  )

set(MODULE_MOC_SRCS
  qCjyxAnnotationsIOOptionsWidget.h
  qCjyxAnnotationsModule.h
  qCjyxAnnotationsReader.h
  GUI/qCjyxAnnotationModuleWidget.h
  GUI/qCjyxAnnotationModulePropertyDialog.h
  GUI/qCjyxAnnotationModuleReportDialog.h
  GUI/qCjyxAnnotationModuleSnapShotDialog.h
  )

set(MODULE_UI_SRCS
  Resources/UI/qCjyxAnnotationModuleWidget.ui
  Resources/UI/qCjyxAnnotationModuleIOOptionsWidget.ui
  Resources/UI/qCjyxAnnotationModulePropertyDialog.ui
  Resources/UI/qCjyxAnnotationModuleReportDialog.ui
  )

set(MODULE_RESOURCES
  Resources/qCjyxAnnotationModule.qrc
  )

set(MODULE_TARGET_LIBRARIES
  qCjyxBaseQTCore
  qMRMLWidgets
  vtkCjyx${MODULE_NAME}ModuleMRML
  vtkCjyx${MODULE_NAME}ModuleVTKWidgets
  vtkCjyx${MODULE_NAME}ModuleMRMLDisplayableManager
  vtkCjyx${MODULE_NAME}ModuleLogic
  qCjyx${MODULE_NAME}ModuleWidgets
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
  add_subdirectory(Testing/Cxx)
  if(Cjyx_USE_PYTHONQT)
    add_subdirectory(Testing/Python)
  endif()
endif()
