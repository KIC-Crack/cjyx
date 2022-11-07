
#-----------------------------------------------------------------------------
set(MODULE_NAME VolumeRendering)
set(MODULE_TITLE "Volume Rendering")

string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)

#-----------------------------------------------------------------------------
add_subdirectory(MRML)
add_subdirectory(Logic)
add_subdirectory(MRMLDM)
add_subdirectory(Widgets)
add_subdirectory(SubjectHierarchyPlugins)

#-----------------------------------------------------------------------------
set(MODULE_EXPORT_DIRECTIVE "Q_CJYX_QTMODULES_${MODULE_NAME_UPPER}_EXPORT")

# Additional includes - Current_{source,binary} and Cjyx_{Libs,Base} already included
set(MODULE_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/MRML
  ${CMAKE_CURRENT_BINARY_DIR}/MRML
  ${CMAKE_CURRENT_SOURCE_DIR}/Logic
  ${CMAKE_CURRENT_BINARY_DIR}/Logic
  ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
  ${CMAKE_CURRENT_BINARY_DIR}/Widgets
  ${CMAKE_CURRENT_SOURCE_DIR}/SubjectHierarchyPlugins
  ${CMAKE_CURRENT_BINARY_DIR}/SubjectHierarchyPlugins
  ${qCjyxAnnotationsModuleWidgets_SOURCE_DIR}
  ${qCjyxAnnotationsModuleWidgets_BINARY_DIR}
  ${qCjyxSubjectHierarchyModuleWidgets_INCLUDE_DIRS}
  )

set(MODULE_SRCS
  qCjyx${MODULE_NAME}Module.cxx
  qCjyx${MODULE_NAME}Module.h
  qCjyx${MODULE_NAME}Reader.cxx
  qCjyx${MODULE_NAME}Reader.h
  qCjyxShaderPropertyReader.cxx
  qCjyxShaderPropertyReader.h
  qCjyx${MODULE_NAME}SettingsPanel.cxx
  qCjyx${MODULE_NAME}SettingsPanel.h
  )

set(MODULE_MOC_SRCS
  qCjyx${MODULE_NAME}Module.h
  qCjyx${MODULE_NAME}Reader.h
  qCjyxShaderPropertyReader.h
  qCjyx${MODULE_NAME}SettingsPanel.h
  )

set(MODULE_UI_SRCS
  Resources/UI/qCjyx${MODULE_NAME}SettingsPanel.ui
  )

set(MODULE_TARGET_LIBRARIES
  vtkCjyx${MODULE_NAME}ModuleLogic
  vtkCjyx${MODULE_NAME}ModuleMRML
  vtkCjyx${MODULE_NAME}ModuleMRMLDisplayableManager
  qCjyx${MODULE_NAME}ModuleWidgets
  qCjyx${MODULE_NAME}SubjectHierarchyPlugins

  vtkCjyxAnnotationsModuleMRML
  qCjyxAnnotationsModuleWidgets
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
