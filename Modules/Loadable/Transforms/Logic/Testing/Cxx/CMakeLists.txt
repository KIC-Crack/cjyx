set(KIT ${PROJECT_NAME})

set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Data)

#-----------------------------------------------------------------------------
set(KIT_TEST_SRCS
  vtkCjyxTransformLogicTest1.cxx
  vtkCjyxTransformLogicTest2.cxx
  vtkCjyxTransformLogicTest3.cxx
  )

#-----------------------------------------------------------------------------
cjyxMacroConfigureModuleCxxTestDriver(
  NAME ${KIT}
  SOURCES ${KIT_TEST_SRCS}
  WITH_VTK_DEBUG_LEAKS_CHECK
  WITH_VTK_ERROR_OUTPUT_CHECK
  )

#-----------------------------------------------------------------------------
simple_test( vtkCjyxTransformLogicTest1 ${DATA_DIR}/affineTransform.txt)
simple_test( vtkCjyxTransformLogicTest2 ${DATA_DIR}/cube.vtk)
simple_test( vtkCjyxTransformLogicTest3 ${DATA_DIR}/cube.vtk ${DATA_DIR}/transformedCube.vtk)
