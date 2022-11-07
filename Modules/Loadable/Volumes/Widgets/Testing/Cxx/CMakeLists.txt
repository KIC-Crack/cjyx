set(KIT ${PROJECT_NAME})

#-----------------------------------------------------------------------------
set(INPUT "${MRMLCore_SOURCE_DIR}/Testing/TestData")

#-----------------------------------------------------------------------------
set(KIT_TEST_SRCS
  qCjyxDiffusionTensorVolumeDisplayWidgetTest1.cxx
  qCjyxDTISliceDisplayWidgetTest1.cxx
  qCjyxDTISliceDisplayWidgetTest2.cxx
  )

#-----------------------------------------------------------------------------
cjyxMacroConfigureModuleCxxTestDriver(
  NAME ${KIT}
  SOURCES ${KIT_TEST_SRCS}
  TARGET_LIBRARIES vtkCjyxVolumesModuleLogic
  INCLUDE_DIRECTORIES
    ${vtkCjyxVolumesModuleLogic_SOURCE_DIR}
    ${vtkCjyxVolumesModuleLogic_BINARY_DIR}
  WITH_VTK_DEBUG_LEAKS_CHECK
  WITH_VTK_ERROR_OUTPUT_CHECK
  )

#-----------------------------------------------------------------------------
simple_test(qCjyxDiffusionTensorVolumeDisplayWidgetTest1)
simple_test(qCjyxDTISliceDisplayWidgetTest1)
simple_test(qCjyxDTISliceDisplayWidgetTest2 DATA{${INPUT}/helix-DTI.nhdr})
