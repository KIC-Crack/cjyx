set(KIT qCjyx${MODULE_NAME}Module)

#-----------------------------------------------------------------------------
set(KIT_TEST_SRCS
  qCjyxCamerasModuleWidgetTest1.cxx
  vtkCjyxCamerasModuleLogicCopyImportedCamerasTest.cxx
  )

#-----------------------------------------------------------------------------
cjyxMacroConfigureModuleCxxTestDriver(
  NAME ${KIT}
  SOURCES ${KIT_TEST_SRCS}
  WITH_VTK_DEBUG_LEAKS_CHECK
  WITH_VTK_ERROR_OUTPUT_CHECK
  )

#-----------------------------------------------------------------------------
simple_test(qCjyxCamerasModuleWidgetTest1)
simple_test(vtkCjyxCamerasModuleLogicCopyImportedCamerasTest)
