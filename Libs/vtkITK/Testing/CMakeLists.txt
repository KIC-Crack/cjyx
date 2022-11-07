
############################################################################
# The test is a stand-alone executable.  However, the Cjyx
# launcher is needed to set up shared library paths correctly.
############################################################################

set(VTKITKTESTVECTORREADER_SOURCE VTKITKVectorReader.cxx)
ctk_add_executable_utf8(VTKITKVectorReader ${VTKITKTESTVECTORREADER_SOURCE})
target_link_libraries(VTKITKVectorReader
  vtkITK)

set_target_properties(VTKITKVectorReader PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})

ExternalData_add_test(${Cjyx_ExternalData_DATA_MANAGEMENT_TARGET}
  NAME VTKITKVectorReader
  COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:VTKITKVectorReader>
    DATA{${MRML_TEST_DATA_DIR}/fixed.nrrd}
  )

cjyx_add_python_unittest(SCRIPT vtkITKArchetypeDiffusionTensorReaderFile.py)
cjyx_add_python_unittest(SCRIPT vtkITKArchetypeScalarReaderFile.py)
