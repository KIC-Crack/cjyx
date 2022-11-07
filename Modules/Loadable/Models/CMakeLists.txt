
#-----------------------------------------------------------------------------
set(MODULE_NAME Models)
set(MODULE_TITLE "${MODULE_NAME}")

string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)

#-----------------------------------------------------------------------------
add_subdirectory(Logic)
add_subdirectory(SubjectHierarchyPlugins)
add_subdirectory(Widgets)

#-----------------------------------------------------------------------------
set(MODULE_EXPORT_DIRECTIVE "Q_CJYX_QTMODULES_${MODULE_NAME_UPPER}_EXPORT")

# Additional includes - Current_{source,binary} and Cjyx_{Libs,Base} already included
# Is used by the Plugins and Testing directories
set(MODULE_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/Logic
  ${CMAKE_CURRENT_BINARY_DIR}/Logic
  ${CMAKE_CURRENT_SOURCE_DIR}/SubjectHierarchyPlugins
  ${CMAKE_CURRENT_BINARY_DIR}/SubjectHierarchyPlugins
  ${qCjyxSubjectHierarchyModuleWidgets_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
  ${CMAKE_CURRENT_BINARY_DIR}/Widgets
  ${qCjyxColorsModuleWidgets_INCLUDE_DIRS}
  ${vtkCjyxColorsModuleMRML_INCLUDE_DIRS}
  ${vtkCjyxColorsModuleLogic_INCLUDE_DIRS}
  )

set(MODULE_SRCS
  qCjyx${MODULE_NAME}Module.cxx
  qCjyx${MODULE_NAME}Module.h
  qCjyx${MODULE_NAME}ModuleWidget.cxx
  qCjyx${MODULE_NAME}ModuleWidget.h
  qCjyx${MODULE_NAME}Reader.cxx
  qCjyx${MODULE_NAME}Reader.h
  qCjyx${MODULE_NAME}IOOptionsWidget.cxx
  qCjyx${MODULE_NAME}IOOptionsWidget.h
  )

set(MODULE_MOC_SRCS
  qCjyx${MODULE_NAME}Module.h
  qCjyx${MODULE_NAME}ModuleWidget.h
  qCjyx${MODULE_NAME}Reader.h
  qCjyx${MODULE_NAME}IOOptionsWidget.h
  )

set(MODULE_UI_SRCS
  Resources/UI/qCjyx${MODULE_NAME}ModuleWidget.ui
  Resources/UI/qCjyx${MODULE_NAME}IOOptionsWidget.ui
  )

set(MODULE_TARGET_LIBRARIES
  qCjyx${MODULE_NAME}ModuleWidgets
  vtkCjyx${MODULE_NAME}ModuleLogic
  qCjyx${MODULE_NAME}SubjectHierarchyPlugins
  qCjyxColorsModuleWidgets
  vtkCjyxColorsModuleLogic
  vtkCjyxColorsModuleMRML
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
