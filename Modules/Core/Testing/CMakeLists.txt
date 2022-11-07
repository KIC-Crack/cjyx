set(KIT ${PROJECT_NAME})

set(KIT_TEST_SRCS)
set(KIT_TEST_NAMES)
set(KIT_TEST_NAMES_CXX)
set(modulenames
  EventBroker
  )
add_definitions(-DGENERIC_CXX_MODULE_TEST_SKIP_DEPENDENCIES)
CjyxMacroConfigureGenericCxxModuleTests("${modulenames}" KIT_TEST_SRCS KIT_TEST_NAMES KIT_TEST_NAMES_CXX)

set(CMAKE_TESTDRIVER_BEFORE_TESTMAIN "DEBUG_LEAKS_ENABLE_EXIT_ERROR();" )
create_test_sourcelist(Tests ${KIT}CxxTests.cxx
  ${KIT_TEST_NAMES_CXX}
  qCjyxCoreModuleTest1.cxx
  qCjyxCoreModuleFactoryTest1.cxx
  qCjyxModulePanelTest2.cxx
  EXTRA_INCLUDE vtkMRMLDebugLeaksMacro.h
  )

list(REMOVE_ITEM Tests ${KIT_TEST_NAMES_CXX})
list(APPEND Tests ${KIT_TEST_SRCS})

set(LIBRARY_NAME ${PROJECT_NAME})

ctk_add_executable_utf8(${KIT}CxxTests ${Tests})
target_link_libraries(${KIT}CxxTests ${KIT})
set_target_properties(${KIT}CxxTests PROPERTIES LABELS ${KIT})
set_target_properties(${KIT}CxxTests PROPERTIES FOLDER "Core-Base")

foreach(testname ${KIT_TEST_NAMES})
  simple_test( ${testname} )
endforeach()
simple_test( qCjyxCoreModuleTest1 )
simple_test( qCjyxCoreModuleFactoryTest1 )
simple_test( qCjyxModulePanelTest2 )
