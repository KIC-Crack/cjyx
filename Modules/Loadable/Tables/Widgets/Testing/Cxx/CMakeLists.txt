set(KIT ${PROJECT_NAME})

#-----------------------------------------------------------------------------
set(INPUT ${MRMLCore_SOURCE_DIR}/Testing)

#-----------------------------------------------------------------------------
set(KIT_TEST_SRCS
  qCjyxTableColumnPropertiesWidgetTest1.cxx
  )

#-----------------------------------------------------------------------------
cjyxMacroConfigureModuleCxxTestDriver(
  NAME ${KIT}
  SOURCES ${KIT_TEST_SRCS}
  WITH_VTK_DEBUG_LEAKS_CHECK
  WITH_VTK_ERROR_OUTPUT_CHECK
  )

#-----------------------------------------------------------------------------
simple_test( qCjyxTableColumnPropertiesWidgetTest1 )
