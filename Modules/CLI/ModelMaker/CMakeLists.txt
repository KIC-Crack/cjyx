
#-----------------------------------------------------------------------------
set(MODULE_NAME ModelMaker)

#-----------------------------------------------------------------------------

#
# CjyxExecutionModel
#
find_package(CjyxExecutionModel REQUIRED)
include(${CjyxExecutionModel_USE_FILE})

#-----------------------------------------------------------------------------
# TODO: Remove CjyxBaseLogic once pass in color node information
SEMMacroBuildCLI(
  NAME ${MODULE_NAME}
  LOGO_HEADER ${Cjyx_SOURCE_DIR}/Resources/NAMICLogo.h
  TARGET_LIBRARIES vtkITK ModuleDescriptionParser MRMLCore CjyxBaseLogic CjyxBaseCLI ${VTK_LIBRARIES}
  INCLUDE_DIRECTORIES
    ${vtkITK_INCLUDE_DIRS}
    ${MRMLCore_INCLUDE_DIRS}
    ${CjyxBaseLogic_SOURCE_DIR} ${CjyxBaseLogic_BINARY_DIR}
  )

#-----------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
