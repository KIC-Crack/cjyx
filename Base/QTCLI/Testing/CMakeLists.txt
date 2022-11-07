
#-----------------------------------------------------------------------------
set(KIT ${PROJECT_NAME})

#-----------------------------------------------------------------------------

#
# CjyxExecutionModel
#
find_package(CjyxExecutionModel REQUIRED)
include(${CjyxExecutionModel_USE_FILE})

#-----------------------------------------------------------------------------
SEMMacroBuildCLI(
  NAME CLIModule4Test
  FOLDER "Core-Base"
  LOGO_HEADER ${Cjyx_SOURCE_DIR}/Resources/ITKLogo.h
  NO_INSTALL
  )

#-----------------------------------------------------------------------------
CjyxMacroBuildScriptedCLI(
  NAME PyCLIModule4Test
  FOLDER "Core-Base"
  NO_INSTALL
  )

#-----------------------------------------------------------------------------
set(KIT_TEST_SRCS
  qCjyxCLIExecutableModuleFactoryTest1.cxx
  qCjyxCLILoadableModuleFactoryTest1.cxx
  qCjyxCLIModuleTest1.cxx
  )
if(Cjyx_USE_PYTHONQT)
  list(APPEND KIT_TEST_SRCS
    qCjyxPyCLIModuleTest1.cxx
    )
endif()

#-----------------------------------------------------------------------------
set(CMAKE_TESTDRIVER_BEFORE_TESTMAIN "DEBUG_LEAKS_ENABLE_EXIT_ERROR();" )
create_test_sourcelist(Tests ${KIT}CxxTests.cxx
  ${KIT_TEST_SRCS}
  EXTRA_INCLUDE vtkMRMLDebugLeaksMacro.h
  )

ctk_add_executable_utf8(${KIT}CxxTests ${Tests})
target_link_libraries(${KIT}CxxTests ${KIT})
set_target_properties(${KIT}CxxTests PROPERTIES LABELS ${KIT})
set_target_properties(${KIT}CxxTests PROPERTIES FOLDER "Core-Base")

#
# Add Tests
#

simple_test( qCjyxCLIExecutableModuleFactoryTest1 )
simple_test( qCjyxCLILoadableModuleFactoryTest1 )
simple_test( qCjyxCLIModuleTest1 )
if(Cjyx_USE_PYTHONQT)
  simple_test( qCjyxPyCLIModuleTest1 )
endif()
