#-----------------------------------------------------------------------------
set(INPUT ${CMAKE_CURRENT_SOURCE_DIR}/../Data/Input)

set(testname "py_mainwindow_RestoreSceneViewWithoutCamera")
add_test(
  NAME ${testname}
  COMMAND ${Cjyx_LAUNCHER_EXECUTABLE}
  --disable-cli-modules --disable-scripted-loadable-modules
  --testing --python-code "getNode('vtkMRMLSceneViewNode1').RestoreScene(); cjyx.app.processEvents()"
  ${INPUT}/SceneViewWithoutCamera.mrml
  )
set_property(TEST ${testname} PROPERTY LABELS ${MODULE_NAME})

# Disabled scene view tests for now - they will be fixed in upcoming commit
#
# include(CjyxMacroBuildScriptedModule)
#
# # Test adding new storable data after creating a scene view
# cjyxMacroBuildScriptedModule(
#   NAME AddStorableDataAfterSceneViewTest
#   SCRIPTS AddStorableDataAfterSceneViewTest.py
# )
# cjyx_add_python_unittest(SCRIPT AddStorableDataAfterSceneViewTest.py
#                            CJYX_ARGS --disable-cli-modules)
# set_property(TEST py_AddStorableDataAfterSceneViewTest PROPERTY LABELS ${MODULE_NAME})
