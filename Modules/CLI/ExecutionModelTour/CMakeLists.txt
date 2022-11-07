
#-----------------------------------------------------------------------------
set(MODULE_NAME ExecutionModelTour)

#-----------------------------------------------------------------------------

#
# CjyxExecutionModel
#
find_package(CjyxExecutionModel REQUIRED)
include(${CjyxExecutionModel_USE_FILE})

#-----------------------------------------------------------------------------
SEMMacroBuildCLI(
  NAME ${MODULE_NAME}
  LOGO_HEADER ${Cjyx_SOURCE_DIR}/Resources/BillsLogo.h
  TARGET_LIBRARIES
    # ${VTK_LIBRARIES} # Not needed: All dependencies are transitively satisfied by other targets
    MRMLCore
    vtkCjyxMarkupsModuleMRML
  INCLUDE_DIRECTORIES
    ${MRMLCore_INCLUDE_DIRS}
    ${vtkCjyxMarkupsModuleMRML_INCLUDE_DIRS}
  )

#-----------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
