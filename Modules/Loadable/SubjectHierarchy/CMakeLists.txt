#-----------------------------------------------------------------------------
set(MODULE_NAME "SubjectHierarchy")
set(MODULE_TITLE "Subject Hierarchy")

string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)

#-----------------------------------------------------------------------------
add_subdirectory(Logic)
add_subdirectory(Widgets)

#-----------------------------------------------------------------------------
set(MODULE_EXPORT_DIRECTIVE "Q_CJYX_QTMODULES_${MODULE_NAME_UPPER}_EXPORT")

set(MODULE_INCLUDE_DIRECTORIES
  ${Cjyx_ModuleWidgets_INCLUDE_DIRS}
  ${qCjyxSubjectHierarchyModuleWidgets_INCLUDE_DIRS}
  )

  set(MODULE_SRCS
  qCjyx${MODULE_NAME}ModuleWidget.cxx
  qCjyx${MODULE_NAME}ModuleWidget.h
  qCjyx${MODULE_NAME}Module.cxx
  qCjyx${MODULE_NAME}Module.h
  qCjyx${MODULE_NAME}SettingsPanel.cxx
  qCjyx${MODULE_NAME}SettingsPanel.h
  )

set(MODULE_MOC_SRCS
  qCjyx${MODULE_NAME}ModuleWidget.h
  qCjyx${MODULE_NAME}Module.h
  qCjyx${MODULE_NAME}SettingsPanel.h
  )

set(MODULE_UI_SRCS
  Resources/UI/qCjyx${MODULE_NAME}Module.ui
  Resources/UI/qCjyx${MODULE_NAME}SettingsPanel.ui
  )

set(MODULE_TARGET_LIBRARIES
  vtkCjyx${MODULE_NAME}ModuleLogic
  qCjyx${MODULE_NAME}ModuleWidgets
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
