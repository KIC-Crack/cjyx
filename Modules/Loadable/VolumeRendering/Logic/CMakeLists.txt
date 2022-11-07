project(vtkCjyx${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_CJYX_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkCjyx${MODULE_NAME}ModuleMRML_SOURCE_DIR}
  ${vtkCjyx${MODULE_NAME}ModuleMRML_BINARY_DIR}
  )

set(${KIT}_SRCS
  vtkCjyx${MODULE_NAME}Logic.cxx
  vtkCjyx${MODULE_NAME}Logic.h
  )

set(${KIT}_TARGET_LIBRARIES
  vtkCjyx${MODULE_NAME}ModuleMRML
  vtkCjyxAnnotationsModuleMRML
  )

if(Cjyx_VTK_RENDERING_USE_OpenGL_BACKEND)
  find_package(OpenGL REQUIRED)
  list(APPEND ${KIT}_TARGET_LIBRARIES
    ${OPENGL_LIBRARIES}
    )
endif()

#-----------------------------------------------------------------------------
CjyxMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/presets.xml
  ${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME}/presets.xml
  COPYONLY)

install(
  FILES ${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME}/presets.xml
  DESTINATION ${Cjyx_INSTALL_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME} COMPONENT Runtime)

if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
