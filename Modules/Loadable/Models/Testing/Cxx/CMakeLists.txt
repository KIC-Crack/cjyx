set(KIT qCjyx${MODULE_NAME}Module)

#-----------------------------------------------------------------------------
set(MODEL_FILE "${MRMLCore_SOURCE_DIR}/Testing/TestData/cube.vtk")
set(MODEL_SCENE "${MRMLCore_SOURCE_DIR}/Testing/modelHierarchy.mrml")

#-----------------------------------------------------------------------------
set(KIT_TEST_SRCS
  qCjyxModelsModuleWidgetTest.cxx
  qCjyxModelsModuleWidgetTest1.cxx
  qCjyxModelsModuleWidgetTestScene.cxx
  vtkCjyxModelsLogicAddFileTest.cxx
  )

#-----------------------------------------------------------------------------
set(KIT_TEST_GENERATE_MOC_SRCS
  qCjyx${MODULE_NAME}ModuleWidgetTest.cxx
  )
  QT5_GENERATE_MOCS(${KIT_TEST_GENERATE_MOC_SRCS})

include_directories( ${CMAKE_CURRENT_BINARY_DIR})

#-----------------------------------------------------------------------------
cjyxMacroConfigureModuleCxxTestDriver(
  NAME ${KIT}
  SOURCES ${KIT_TEST_SRCS}
  WITH_VTK_DEBUG_LEAKS_CHECK
  WITH_VTK_ERROR_OUTPUT_CHECK
  )

#-----------------------------------------------------------------------------
simple_test( qCjyxModelsModuleWidgetTest1 ${MODEL_FILE} )
simple_test( qCjyxModelsModuleWidgetTestScene ${MODEL_SCENE} )
simple_test(vtkCjyxModelsLogicAddFileTest ${MODEL_FILE})
simple_test( qCjyxModelsModuleWidgetTest )
