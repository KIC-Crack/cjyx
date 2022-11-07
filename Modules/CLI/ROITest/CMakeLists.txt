
#-----------------------------------------------------------------------------
set(MODULE_NAME CLIROITest)

#-----------------------------------------------------------------------------

#
# CjyxExecutionModel
#
find_package(CjyxExecutionModel REQUIRED)
include(${CjyxExecutionModel_USE_FILE})

#-----------------------------------------------------------------------------
SEMMacroBuildCLI(
  NAME ${MODULE_NAME}
  LOGO_HEADER ${Cjyx_SOURCE_DIR}/Resources/NAMICLogo.h
  TARGET_LIBRARIES
    # ${VTK_LIBRARIES} # Not needed: All dependencies are transitively satisfied by other targets
    ModuleDescriptionParser ${ITK_LIBRARIES}
    vtkTeem
    MRMLCore
    CjyxBaseCLI
  INCLUDE_DIRECTORIES
    ${vtkTeem_INCLUDE_DIRS}
    ${MRMLCore_INCLUDE_DIRS}
    ${vtkITK_INCLUDE_DIRS}
    ${CjyxBaseCLI_SOURCE_DIR} ${CjyxBaseCLI_BINARY_DIR}
  NO_INSTALL
  )

