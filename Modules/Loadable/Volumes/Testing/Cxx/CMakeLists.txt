set(KIT qCjyx${MODULE_NAME}Module)

#-----------------------------------------------------------------------------

#
# ITK
#
set(${KIT}Testing_ITK_COMPONENTS
  ITKCommon
  )
find_package(ITK 4.6 COMPONENTS ${${KIT}Testing_ITK_COMPONENTS} REQUIRED)
if(ITK_VERSION VERSION_GREATER_EQUAL "5.3")
  foreach(factory_uc IN ITEMS "IMAGEIO" "MESHIO" "TRANSFORMIO")
    set(ITK_NO_${factory_uc}_FACTORY_REGISTER_MANAGER 1)
  endforeach()
else()
  set(ITK_NO_IO_FACTORY_REGISTER_MANAGER 1) # See Libs/ITKFactoryRegistration/CMakeLists.txt
endif()
list(APPEND ITK_LIBRARIES ITKFactoryRegistration)
list(APPEND ITK_INCLUDE_DIRS
  ${ITKFactoryRegistration_INCLUDE_DIRS}
  )
include(${ITK_USE_FILE})

#-----------------------------------------------------------------------------
set(TEMP ${Cjyx_BINARY_DIR}/Testing/Temporary)
set(MRML_CORE_INPUT ${MRMLCore_SOURCE_DIR}/Testing/TestData/)
set(CJYXAPP_INPUT ${Cjyx_SOURCE_DIR}/Applications/CjyxApp/Data/Input/)

#-----------------------------------------------------------------------------
set(KIT_TEST_SRCS
  qCjyx${MODULE_NAME}IOOptionsWidgetTest1.cxx
  qCjyx${MODULE_NAME}ModuleWidgetTest1.cxx
  vtkCjyx${MODULE_NAME}LogicTest1.cxx
  )

#-----------------------------------------------------------------------------
cjyxMacroConfigureModuleCxxTestDriver(
  NAME ${KIT}
  SOURCES ${KIT_TEST_SRCS}
  TARGET_LIBRARIES vtkCjyxVolumesModuleLogic
  WITH_VTK_DEBUG_LEAKS_CHECK
  WITH_VTK_ERROR_OUTPUT_CHECK
  )

#-----------------------------------------------------------------------------
simple_test(qCjyxVolumesIOOptionsWidgetTest1)
simple_test(qCjyxVolumesModuleWidgetTest1 DATA{${MRML_CORE_INPUT}/fixed.nrrd})
simple_test(vtkCjyxVolumesLogicTest1 DATA{${MRML_CORE_INPUT}/fixed.nrrd})
simple_test(vtkCjyxVolumesLogicTest1_TestNAN
  DRIVER_TESTNAME vtkCjyx${MODULE_NAME}LogicTest1 DATA{${CJYXAPP_INPUT}/testNANInVolume.nrrd}
  )

#-----------------------------------------------------------------------------
ctk_add_executable_utf8(vtkCjyx${MODULE_NAME}LogicTest2 vtkCjyx${MODULE_NAME}LogicTest2.cxx)
target_link_libraries(vtkCjyx${MODULE_NAME}LogicTest2 ${KIT})
set_target_properties(vtkCjyx${MODULE_NAME}LogicTest2 PROPERTIES FOLDER "Module-${MODULE_NAME}")

add_test(NAME vtkCjyx${MODULE_NAME}LogicTest2
  COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:vtkCjyx${MODULE_NAME}LogicTest2>
  vtkCjyx${MODULE_NAME}LogicTest2 ${TEMP}
  )
