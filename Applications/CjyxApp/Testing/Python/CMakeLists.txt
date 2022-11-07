
#-----------------------------------------------------------------------------
set(INPUT ${CMAKE_CURRENT_SOURCE_DIR}/../../Data/Input)

#
# Check if '--python-code' / 'cjyx.testing' works as expected
#

add_test(
  NAME py_nomainwindow_CjyxPythonCodeExitSuccessTest
  COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
  --disable-modules --ignore-cjyxrc --no-main-window
  --python-code "exit(cjyx.util.EXIT_SUCCESS)"
  )
add_test(
  NAME py_nomainwindow_testing_CjyxPythonCodeExitSuccessTest
  COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
  --testing --disable-modules --no-main-window
  --python-code "exit(cjyx.util.EXIT_SUCCESS)"
  )

add_test(
  NAME py_nomainwindow_CjyxPythonCodeExitFailureTest
  COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
  --disable-modules --ignore-cjyxr --no-main-windowc
  --python-code "exit(cjyx.util.EXIT_FAILURE)"
  )
add_test(
  NAME py_nomainwindow_testing_CjyxPythonCodeExitFailureTest
  COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
  --testing --disable-modules --no-main-window
  --python-code "exit(cjyx.util.EXIT_FAILURE)"
  )
set_tests_properties(
    py_nomainwindow_CjyxPythonCodeExitFailureTest
    py_nomainwindow_testing_CjyxPythonCodeExitFailureTest
  PROPERTIES WILL_FAIL TRUE
  )

add_test(
  NAME py_nomainwindow_CjyxPythonCodeTest3
  COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
  --testing --disable-modules --no-main-window
  --python-code "import cjyx.testing; cjyx.testing.exitSuccess()"
  )

add_test(
  NAME py_nomainwindow_CjyxPythonCodeTest4
  COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
  --testing --disable-modules --no-main-window
  --python-code "import cjyx.testing; cjyx.testing.exitFailure('Should fail !')"
  )
set_tests_properties(py_nomainwindow_CjyxPythonCodeTest4 PROPERTIES WILL_FAIL TRUE)

add_test(
  NAME py_nomainwindow_CjyxPythonCodeTest5
  COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
  --testing --disable-modules --no-main-window
  --python-code "raise Exception('Should fail !')"
  )
set_tests_properties(py_nomainwindow_CjyxPythonCodeTest5 PROPERTIES WILL_FAIL TRUE)

#
# Check if '--python-scripts' / 'testing' works as expected
#

cjyx_add_python_test(
  SCRIPT CjyxTestingExitSuccessTest.py
  SCRIPT_ARGS --no-main-window TESTNAME_PREFIX nomainwindow_
  )

if (WIN32)
  # Test script execution with native path separator character (backslash)
  string(REPLACE "/" "\\\\" nativeFilePath "${CMAKE_CURRENT_SOURCE_DIR}/bCjyxTestingExitSuccessTest.py")
  cjyx_add_python_test(
    SCRIPT ${nativeFilePath}
    SCRIPT_ARGS --no-main-window TESTNAME_PREFIX nomainwindow_nativePath_
    )
endif()

cjyx_add_python_test(
  SCRIPT CjyxTestingExitFailureTest.py
  SCRIPT_ARGS --no-main-window TESTNAME_PREFIX nomainwindow_
  )
set_tests_properties(py_nomainwindow_CjyxTestingExitFailureTest PROPERTIES WILL_FAIL TRUE)

cjyx_add_python_test(SCRIPT CjyxTestingExitSuccessTest.py)

cjyx_add_python_test(SCRIPT CjyxTestingExitFailureTest.py)
set_tests_properties(py_CjyxTestingExitFailureTest PROPERTIES WILL_FAIL TRUE)

#
# Check if 'cjyx.testing.runUnitTest()' works as expected
#

cjyx_add_python_unittest(
  SCRIPT CjyxUnitTestTest.py
  CJYX_ARGS --no-main-window --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )

cjyx_add_python_unittest(
  SCRIPT CjyxUnitTestWithErrorsTest.py
  CJYX_ARGS --no-main-window --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )
set_tests_properties(py_nomainwindow_CjyxUnitTestWithErrorsTest PROPERTIES WILL_FAIL TRUE)

#
# Exercise different Cjyx command line option and check that no warnings are displayed.
#
macro(nowarning_test test_name)
  add_test(
    NAME py_nowarning_${test_name}Test
    COMMAND ${Cjyx_LAUNCH_COMMAND} $<TARGET_FILE:${APP_TARGET_NAME}>
    ${ARGN} --testing --python-code "exit(cjyx.util.EXIT_SUCCESS)"
    )
  set(fail_regexp)
  list(APPEND fail_regexp "Failed to obtain reference to")
  list(APPEND fail_regexp "Exception AttributeError: .* instance has no attribute")
  set_tests_properties(py_nowarning_${test_name}Test PROPERTIES FAIL_REGULAR_EXPRESSION "${fail_regexp}")
endmacro()
nowarning_test(mainwindow --no-splash)
nowarning_test(mainwindow_application_info --application-information)
nowarning_test(mainwindow_python_interactor --show-python-interactor)
nowarning_test(mainwindow_nocli --disable-cli-modules)
nowarning_test(mainwindow_noloadable --disable-loadable-modules)
nowarning_test(mainwindow_noscripted --disable-scripted-loadable-modules)
nowarning_test(mainwindow_nocli_noloadable --disable-cli-modules --disable-loadable-modules)
nowarning_test(mainwindow_nocli_noscripted --disable-cli-modules --disable-scripted-loadable-modules)
nowarning_test(mainwindow_noloadable_noscripted --disable-loadable-modules --disable-scripted-loadable-modules)
nowarning_test(mainwindow_nocli_noloadable_noscripted --disable-cli-modules --disable-loadable-modules --disable-scripted-loadable-modules)
nowarning_test(mainwindow_nomodules --disable-modules)
nowarning_test(nomainwindow --no-main-window)
nowarning_test(nomainwindow_application_info --no-main-window --application-information)
nowarning_test(nomainwindow_python_interactor --no-main-window --show-python-interactor)
nowarning_test(nomainwindow_nocli --no-main-window --disable-cli-modules)
nowarning_test(nomainwindow_noloadable --no-main-window --disable-loadable-modules)
nowarning_test(nomainwindow_noscripted --no-main-window --disable-scripted-loadable-modules)
nowarning_test(nomainwindow_nocli_noloadable --no-main-window --disable-cli-modules --disable-loadable-modules)
nowarning_test(nomainwindow_nocli_noscripted --no-main-window --disable-cli-modules --disable-scripted-loadable-modules)
nowarning_test(nomainwindow_noloadable_noscripted --no-main-window --disable-loadable-modules --disable-scripted-loadable-modules)
nowarning_test(nomainwindow_nocli_noloadable_noscripted --no-main-window --disable-cli-modules --disable-loadable-modules --disable-scripted-loadable-modules)
nowarning_test(nomainwindow_nomodules --no-main-window --disable-modules)
nowarning_test(nomainwindow_ignorecjyxrc --no-main-window --ignore-cjyxrc)

#
# Test Cjyx command line options
#

add_test(
  NAME py_nomainwindow_CjyxOptionDisableSettingsTest
  COMMAND ${PYTHON_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/CjyxOptionDisableSettingsTest.py
    ${Cjyx_LAUNCHER_EXECUTABLE}
  )

add_test(
  NAME py_nomainwindow_CjyxOptionIgnoreCjyxRCTest
  COMMAND ${PYTHON_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/CjyxOptionIgnoreCjyxRCTest.py
    ${Cjyx_LAUNCHER_EXECUTABLE}
  )

if(UNIX)
  add_test(
    NAME py_nomainwindow_CjyxOptionModulesToIgnoreTest
    COMMAND ${PYTHON_EXECUTABLE}
      ${CMAKE_CURRENT_SOURCE_DIR}/CjyxOptionModulesToIgnoreTest.py
      ${Cjyx_LAUNCHER_EXECUTABLE}
      ${Cjyx_BINARY_DIR}/bin/cjyxExtensionWizard
    )
endif()

if(Cjyx_BUILD_PARAMETERSERIALIZER_SUPPORT)
  ExternalData_add_test(${Cjyx_ExternalData_DATA_MANAGEMENT_TARGET}
    NAME py_nomainwindow_CLISerializationTest
    COMMAND ${PYTHON_EXECUTABLE}
      ${CMAKE_CURRENT_SOURCE_DIR}/CLISerializationTest.py
      ${Cjyx_LAUNCHER_EXECUTABLE}
      ${Cjyx_SOURCE_DIR}/Testing/Data/Input
      DATA{${INPUT}/MRHeadResampled.nhdr,MRHeadResampled.raw.gz}
      DATA{${INPUT}/CTHeadAxial.nhdr,CTHeadAxial.raw.gz}
      ${Cjyx_BINARY_DIR}/Testing/Temporary
    )
endif()

#
# Check if scripted module import works as expected
#

set(testname ScriptedModuleDiscoveryTest)
cjyx_add_python_test(
  SCRIPT ${testname}.py
  CJYX_ARGS --no-main-window --disable-builtin-modules --additional-module-path ${CMAKE_CURRENT_SOURCE_DIR}/ScriptedModuleDiscoveryTest
  TESTNAME_PREFIX nomainwindow_
  )
if(UNIX)
set_tests_properties(py_nomainwindow_${testname} PROPERTIES
  PASS_REGULAR_EXPRESSION "ModuleAWidget finalized"
  )
endif()

#
# Check that an exception raised during scripted module cleanup sets exit code
#
set(testname ScriptedModuleCleanupTest_with_testing)
add_test(
  NAME py_${testname}
  COMMAND ${PYTHON_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/ScriptedModuleCleanupTest.py ${Cjyx_LAUNCHER_EXECUTABLE} --with-testing
  )

set(testname ScriptedModuleCleanupTest_without_testing)
add_test(
  NAME py_${testname}
  COMMAND ${PYTHON_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/ScriptedModuleCleanupTest.py ${Cjyx_LAUNCHER_EXECUTABLE}
  )

#
# Application tests
#
cjyx_add_python_unittest(
  SCRIPT CjyxSceneObserverTest.py
  CJYX_ARGS --no-main-window --disable-modules --ignore-cjyxrc
  TESTNAME_PREFIX nomainwindow_
  )

if(VTK_DEBUG_LEAKS AND Cjyx_HAS_CONSOLE_IO_SUPPORT)
  set(testname MRMLCreateNodeByClassWithoutSetReferenceCount)
  cjyx_add_python_test(
    SCRIPT ${testname}.py
    SCRIPT_ARGS --no-main-window --disable-modules
    TESTNAME_PREFIX nomainwindow_
    )
  set_tests_properties(py_nomainwindow_${testname} PROPERTIES
    PASS_REGULAR_EXPRESSION "instances? still around"
    )
endif()

set(testname MRMLCreateNodeByClassWithSetReferenceCountMinusOne)
cjyx_add_python_test(
  SCRIPT ${testname}.py
  SCRIPT_ARGS --no-main-window --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )

set(testname MRMLSceneImportAndExport)
cjyx_add_python_test(
  SCRIPT ${testname}.py
  TESTNAME_PREFIX mainwindow_
  )

if(Cjyx_USE_SimpleITK)
  cjyx_add_python_unittest(
    SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/tests/test_sitkUtils.py
    CJYX_ARGS --no-main-window
    TESTNAME_PREFIX nomainwindow_
    )
endif()

cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/tests/test_PythonManager.py
  CJYX_ARGS --no-main-window --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )

cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/cjyx/tests/test_cjyx_util_without_modules.py
  CJYX_ARGS --no-main-window  --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )

cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/cjyx/tests/test_cjyx_util_chdir.py
  CJYX_ARGS --no-main-window --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )

cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/cjyx/tests/test_cjyx_util_save.py
  CJYX_ARGS --no-main-window --disable-cli-modules --disable-scripted-loadable-modules DATA{${INPUT}/MR-head.nrrd}
  TESTNAME_PREFIX nomainwindow_
  )

cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/cjyx/tests/test_cjyx_util_getNodes.py
  CJYX_ARGS --no-main-window --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )

## Test reading MGH file format types.
cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/cjyx/tests/test_cjyx_mgh.py
  CJYX_ARGS --no-main-window --disable-cli-modules --disable-scripted-loadable-modules
              DATA{${INPUT}/T1.mgz}
              DATA{${INPUT}/T1_longname.mgh.gz}
              DATA{${INPUT}/T1_uncompressed.mgh}
  TESTNAME_PREFIX nomainwindow_
  )

## Test reading MINC file format types.
cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/cjyx/tests/test_cjyx_minc.py
  CJYX_ARGS --no-main-window --disable-cli-modules --disable-scripted-loadable-modules
              DATA{${INPUT}/pd_z-_float_xyz.mnc}
  TESTNAME_PREFIX nomainwindow_
  )

cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/cjyx/tests/test_cjyx_python_sqlite3.py
  CJYX_ARGS --no-main-window  --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )

cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/cjyx/tests/test_cjyx_python_lzma.py
  CJYX_ARGS --no-main-window  --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )

cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/cjyx/tests/test_cjyx_util_VTKObservationMixin.py
  CJYX_ARGS --no-main-window --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )

cjyx_add_python_unittest(
  SCRIPT ${Cjyx_SOURCE_DIR}/Base/Python/cjyx/tests/test_cjyx_environment.py
  CJYX_ARGS --no-main-window --disable-modules
  TESTNAME_PREFIX nomainwindow_
  )

# Test multi-shell DWI I/O
cjyx_add_python_test(
  SCRIPT DWMRIMultishellIOTests.py
  CJYX_ARGS --no-main-window --disable-modules --testing
  SCRIPT_ARGS
    ${Cjyx_SOURCE_DIR}/
    ${Cjyx_BINARY_DIR}/Testing/Temporary/
  TESTNAME_PREFIX nomainwindow_
  )

# DCMTKPrivateDictionary test
cjyx_add_python_test(
  SCRIPT DCMTKPrivateDictTest.py
  CJYX_ARGS --no-main-window --disable-modules
  SCRIPT_ARGS ${Cjyx_SOURCE_DIR}/Testing/Data/Input/philips_ingenia_4_1_3_dwi.dcm
  TESTNAME_PREFIX nomainwindow_
  )

if(Cjyx_USE_PYTHONQT_WITH_OPENSSL)
  add_test(
    NAME py_CjyxSslTest
    COMMAND ${Cjyx_LAUNCHER_EXECUTABLE}
    --ignore-cjyxrc -c "from urllib.request import urlopen; urlopen('https://www.eff.org/https-everywhere')"
    )
  add_test(
    NAME py_PythonSslTest
    COMMAND ${PYTHON_EXECUTABLE}
    -c "from urllib.request import urlopen; urlopen('https://www.eff.org/https-everywhere')"
    )
endif()

# Check that python standard library modules can be imported
foreach(stdlib_module IN ITEMS
    "bz2"
    )
  add_test(
    NAME py_test_stdlib_import_${stdlib_module}_using_cjyx
    COMMAND ${Cjyx_LAUNCHER_EXECUTABLE}
    -c "import ${stdlib_module}" --no-splash --ignore-cjyxrc --disable-settings
    )
  add_test(
    NAME py_test_stdlib_import_${stdlib_module}_using_python
    COMMAND ${PYTHON_EXECUTABLE}
    -c "import ${stdlib_module}"
    )
endforeach()

# Check that application signal "startupCompleted()" is emitted
add_test(
  NAME py_startupcompleted_signal_emitted
  COMMAND ${PYTHON_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/CjyxStartupCompletedTest.py
    ${Cjyx_LAUNCHER_EXECUTABLE}
  )

# Check that stand-alone Python interpreter, without PythonQt support, can `import cjyx` safely (#945)
add_test(
  NAME py_standalonepython_import_cjyx
  COMMAND ${Cjyx_LAUNCHER_EXECUTABLE} --launch
    ${PYTHON} ${CMAKE_CURRENT_SOURCE_DIR}/StandalonePythonImportCjyxTest.py
  )

#
# SelfTests
# see https://wiki.slicer.org/slicerWiki/index.php/Documentation/Nightly/Developers/Tutorials/SelfTestModule
#

if(Cjyx_USE_QtTesting AND Cjyx_USE_PYTHONQT)

  # add as unit test for use at build/test time
  cjyx_add_python_unittest(SCRIPT AtlasTests.py)
  cjyx_add_python_unittest(SCRIPT DICOMReaders.py)
  cjyx_add_python_unittest(SCRIPT KneeAtlasTest.py)
  cjyx_add_python_unittest(SCRIPT sceneImport2428.py)
  cjyx_add_python_unittest(SCRIPT CjyxMRBMultipleSaveRestoreLoopTest.py)
  cjyx_add_python_unittest(SCRIPT CjyxMRBMultipleSaveRestoreTest.py)
  cjyx_add_python_unittest(SCRIPT CjyxMRBSaveRestoreCheckPathsTest.py)
  cjyx_add_python_unittest(SCRIPT Cjyx4Minute.py)
  cjyx_add_python_unittest(SCRIPT CjyxBoundsTest.py)
  if(Cjyx_BUILD_WEBENGINE_SUPPORT)
    cjyx_add_python_unittest(SCRIPT WebEngine.py)
  endif()
  cjyx_add_python_unittest(SCRIPT SliceLinkLogic.py)
  cjyx_add_python_unittest(SCRIPT ScenePerformance.py)
  cjyx_add_python_unittest(SCRIPT CjyxCreateRulerCrashIssue4199.py)
  cjyx_add_python_unittest(SCRIPT CjyxRestoreSceneViewCrashIssue3445.py)
  cjyx_add_python_unittest(SCRIPT RSNAVisTutorial.py)
  cjyx_add_python_unittest(SCRIPT RSNAQuantTutorial.py)
  cjyx_add_python_unittest(SCRIPT CjyxOrientationSelectorTest.py)
  cjyx_add_python_unittest(SCRIPT CjyxTransformInteractionTest1.py)
  cjyx_add_python_unittest(SCRIPT UtilTest.py)
  cjyx_add_python_unittest(SCRIPT ViewControllersSliceInterpolationBug1926.py)
  cjyx_add_python_unittest(SCRIPT RSNA2012ProstateDemo.py)
  cjyx_add_python_unittest(SCRIPT JRC2013Vis.py)
  cjyx_add_python_unittest(SCRIPT FiducialLayoutSwitchBug1914.py)
  cjyx_add_python_unittest(SCRIPT ShaderProperties.py)
  cjyx_add_python_unittest(SCRIPT CjyxScriptedFileReaderWriterTest.py)

  # add as hidden module for use at run time
  set(KIT_PYTHON_SCRIPTS
    AtlasTests.py
    sceneImport2428.py
    CjyxMRBMultipleSaveRestoreLoopTest.py
    CjyxMRBMultipleSaveRestoreTest.py
    CjyxMRBSaveRestoreCheckPathsTest.py
    Cjyx4Minute.py
    CjyxBoundsTest.py
    WebEngine.py
    SliceLinkLogic.py
    ScenePerformance.py
    RSNAVisTutorial.py
    RSNAQuantTutorial.py
    CjyxOrientationSelectorTest.py
    CjyxTransformInteractionTest1.py
    ViewControllersSliceInterpolationBug1926.py
    RSNA2012ProstateDemo.py
    JRC2013Vis.py
    FiducialLayoutSwitchBug1914.py
    ShaderProperties.py
    UtilTest.py
    CjyxScriptedFileReaderWriterTest.py
    )

  if(Cjyx_BUILD_CLI_SUPPORT)
    cjyx_add_python_unittest(SCRIPT CLIEventTest.py CJYX_ARGS --no-main-window)
    cjyx_add_python_unittest(SCRIPT TwoCLIsInARowTest.py)
    cjyx_add_python_unittest(SCRIPT TwoCLIsInParallelTest.py)

    if(Cjyx_BUILD_BRAINSTOOLS)
      cjyx_add_python_unittest(SCRIPT BRAINSFitRigidRegistrationCrashIssue4139.py)

      list(APPEND KIT_PYTHON_SCRIPTS
        BRAINSFitRigidRegistrationCrashIssue4139.py
        )
    endif()
  endif()

  set(KIT_PYTHON_RESOURCES
    Resources/UI/ScenePerformance.ui
    Resources/UI/UtilTest.ui
    )

  ctkMacroCompilePythonScript(
    TARGET_NAME ApplicationSelfTests
    SCRIPTS "${KIT_PYTHON_SCRIPTS}"
    RESOURCES "${KIT_PYTHON_RESOURCES}"
    DESTINATION_DIR ${Cjyx_BINARY_DIR}/${Cjyx_QTSCRIPTEDMODULES_LIB_DIR}
    INSTALL_DIR ${Cjyx_INSTALL_QTSCRIPTEDMODULES_LIB_DIR}
    NO_INSTALL_SUBDIR
    )

endif()

