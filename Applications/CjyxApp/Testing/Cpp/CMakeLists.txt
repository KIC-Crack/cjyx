set(KIT ${APPLIB_NAME})

set(CMAKE_TESTDRIVER_BEFORE_TESTMAIN "DEBUG_LEAKS_ENABLE_EXIT_ERROR();" )
set(KIT_TEST_SRCS
  qCjyxAppMainWindowTest1.cxx
  qCjyxModuleFactoryManagerTest1.cxx
  )
create_test_sourcelist(Tests ${KIT}CxxTests.cxx
  ${KIT_TEST_SRCS}
  EXTRA_INCLUDE vtkMRMLDebugLeaksMacro.h
  )

set(target_libraries ${KIT})

ctk_add_executable_utf8(${KIT}CxxTests ${Tests})
target_link_libraries(${KIT}CxxTests  ${target_libraries})
set_target_properties(${KIT}CxxTests PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})

#
# Add Tests
#
simple_test( qCjyxAppMainWindowTest1 )
simple_test( qCjyxModuleFactoryManagerTest1 )

#
# Application tests
#

if(Cjyx_HAS_CONSOLE_IO_SUPPORT)
  add_test(
    NAME cjyx_nomainwindow_DisableModulesCommandLineOptionsTest
    COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
    ${ARGN} --testing --verbose-module-discovery --disable-modules
    )
  add_test(
    NAME cjyx_nomainwindow_DisableModulesCommandLineOptionsTest2
    COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
    ${ARGN} --testing --verbose-module-discovery --disable-cli-modules --disable-loadable-modules --disable-scripted-loadable-modules
    )
  set_tests_properties(
    cjyx_nomainwindow_DisableModulesCommandLineOptionsTest
    cjyx_nomainwindow_DisableModulesCommandLineOptionsTest2
    PROPERTIES PASS_REGULAR_EXPRESSION "Number of loaded modules: 1"
    )

  add_test(
    NAME cjyx_nomainwindow_NoApplicationInformationOptionTest
    COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
    --disable-modules --ignore-cjyxrc --exit-after-startup
    )
  set_tests_properties(
    cjyx_nomainwindow_NoApplicationInformationOptionTest
    PROPERTIES FAIL_REGULAR_EXPRESSION "Session start time"
    )
  add_test(
    NAME cjyx_nomainwindow_ApplicationInformationOptionTest
    COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
    --disable-modules --ignore-cjyxrc --exit-after-startup --application-information
    )
  set_tests_properties(
    cjyx_nomainwindow_ApplicationInformationOptionTest
    PROPERTIES PASS_REGULAR_EXPRESSION "Session start time"
    )
endif()

