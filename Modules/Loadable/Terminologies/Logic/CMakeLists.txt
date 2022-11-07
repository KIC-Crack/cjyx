project(vtkCjyx${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

find_package(RapidJSON REQUIRED)

set(${KIT}_EXPORT_DIRECTIVE "VTK_CJYX_${MODULE_NAME_UPPER}_LOGIC_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${RapidJSON_INCLUDE_DIR}
  )

set(${KIT}_SRCS
  vtkCjyx${MODULE_NAME}ModuleLogic.cxx
  vtkCjyx${MODULE_NAME}ModuleLogic.h
  vtkCjyxTerminologyCategory.cxx
  vtkCjyxTerminologyCategory.h
  vtkCjyxTerminologyType.cxx
  vtkCjyxTerminologyType.h
  vtkCjyxTerminologyEntry.cxx
  vtkCjyxTerminologyEntry.h
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

#-----------------------------------------------------------------------------
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/SegmentationCategoryTypeModifier-DICOM-Master.json
  ${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME}/SegmentationCategoryTypeModifier-DICOM-Master.term.json
  COPYONLY)
install(
  FILES ${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME}/SegmentationCategoryTypeModifier-DICOM-Master.term.json
  DESTINATION ${Cjyx_INSTALL_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME} COMPONENT Runtime)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/SegmentationCategoryTypeModifier-CjyxGeneralAnatomy.json
  ${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME}/SegmentationCategoryTypeModifier-CjyxGeneralAnatomy.term.json
  COPYONLY)
install(
  FILES ${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME}/SegmentationCategoryTypeModifier-CjyxGeneralAnatomy.term.json
  DESTINATION ${Cjyx_INSTALL_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME} COMPONENT Runtime)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/AnatomicRegionAndModifier-DICOM-Master.json
  ${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME}/AnatomicRegionAndModifier-DICOM-Master.term.json
  COPYONLY)
install(
  FILES ${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME}/AnatomicRegionAndModifier-DICOM-Master.term.json
  DESTINATION ${Cjyx_INSTALL_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME} COMPONENT Runtime)
