set(KIT ${PROJECT_NAME})

#-----------------------------------------------------------------------------
set(KIT_TEST_SRCS
  vtkCjyxVolumeRenderingLogicTest.cxx
  vtkCjyxVolumeRenderingLogicAddFromFileTest.cxx
  )

#-----------------------------------------------------------------------------
cjyxMacroConfigureModuleCxxTestDriver(
  NAME ${KIT}
  SOURCES ${KIT_TEST_SRCS}
  WITH_VTK_DEBUG_LEAKS_CHECK
  WITH_VTK_ERROR_OUTPUT_CHECK
  )

#-----------------------------------------------------------------------------
simple_test(vtkCjyxVolumeRenderingLogicTest ${Cjyx_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME})
simple_test(vtkCjyxVolumeRenderingLogicAddFromFileTest ${Cjyx_BINARY_DIR}/Testing/Temporary/)
