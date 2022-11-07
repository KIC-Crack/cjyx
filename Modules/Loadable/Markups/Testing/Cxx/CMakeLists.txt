set(KIT qCjyx${MODULE_NAME}Module)

#-----------------------------------------------------------------------------
set(TEMP ${Cjyx_BINARY_DIR}/Testing/Temporary)
set(INPUT ${CMAKE_CURRENT_SOURCE_DIR}/../Data/Input)

#-----------------------------------------------------------------------------
set(KIT_TEST_SRCS
  vtkMRMLMarkupsDisplayNodeTest1.cxx
  vtkMRMLMarkupsFiducialNodeTest1.cxx
  vtkMRMLMarkupsNodeTest1.cxx
  vtkMRMLMarkupsNodeTest2.cxx
  vtkMRMLMarkupsNodeTest3.cxx
  vtkMRMLMarkupsNodeTest4.cxx
  vtkMRMLMarkupsNodeTest5.cxx
  vtkMRMLMarkupsNodeTest6.cxx
  vtkMRMLMarkupsFiducialStorageNodeTest2.cxx
  vtkMRMLMarkupsFiducialStorageNodeTest3.cxx
  vtkMRMLMarkupsStorageNodeTest1.cxx
  vtkMRMLMarkupsStorageNodeTest2.cxx
  vtkCjyxMarkupsLogicTest1.cxx
  vtkCjyxMarkupsLogicTest2.cxx
  vtkCjyxMarkupsLogicTest3.cxx
  vtkCjyxMarkupsLogicTest4.cxx
  vtkMarkupsAnnotationSceneTest.cxx
  vtkMRMLMarkupsNodeEventsTest.cxx
  )

#-----------------------------------------------------------------------------
cjyxMacroConfigureModuleCxxTestDriver(
  NAME ${KIT}
  SOURCES ${KIT_TEST_SRCS}
  TARGET_LIBRARIES
    vtkCjyxAnnotationsModuleLogic
  WITH_VTK_DEBUG_LEAKS_CHECK
  WITH_VTK_ERROR_OUTPUT_CHECK
  )

SIMPLE_TEST( vtkMRMLMarkupsDisplayNodeTest1 )
SIMPLE_TEST( vtkMRMLMarkupsFiducialNodeTest1 )
SIMPLE_TEST( vtkMRMLMarkupsNodeTest1 )
SIMPLE_TEST( vtkMRMLMarkupsNodeTest2 )
SIMPLE_TEST( vtkMRMLMarkupsNodeTest3 )
SIMPLE_TEST( vtkMRMLMarkupsNodeTest4 )
SIMPLE_TEST( vtkMRMLMarkupsNodeTest5 )
SIMPLE_TEST( vtkMRMLMarkupsNodeTest6 )
SIMPLE_TEST( vtkMRMLMarkupsNodeEventsTest )

# test legacy Cjyx3 fcsv file
SIMPLE_TEST( vtkMRMLMarkupsFiducialStorageNodeTest2 ${INPUT}/cjyx3.fcsv )

# test Cjyx4 annotation acsv file
SIMPLE_TEST( vtkMRMLMarkupsFiducialStorageNodeTest3 ${INPUT}/cjyx4.acsv )

SIMPLE_TEST( vtkMRMLMarkupsStorageNodeTest1 )
SIMPLE_TEST( vtkMRMLMarkupsStorageNodeTest2 ${TEMP} )

# logic tests
SIMPLE_TEST( vtkCjyxMarkupsLogicTest1 )
SIMPLE_TEST( vtkCjyxMarkupsLogicTest2 ${TEMP} )
SIMPLE_TEST( vtkCjyxMarkupsLogicTest3 )
SIMPLE_TEST( vtkCjyxMarkupsLogicTest4 )

# test Cjyx4 annotation fiducials in a mrml file
# TODO: remove this after annotation fiducials have been removed
SIMPLE_TEST( vtkMarkupsAnnotationSceneTest ${INPUT}/AnnotationTest/AnnotationFiducialsTest.mrml )
