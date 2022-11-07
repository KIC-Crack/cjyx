#-----------------------------------------------------------------------------
set(INPUT ${CMAKE_CURRENT_SOURCE_DIR}/../Data/Input)
#-----------------------------------------------------------------------------

cjyx_add_python_test(SCRIPT AnnotationsTestingAddManyFiducials.py
  CJYX_ARGS --disable-cli-modules)

cjyx_add_python_test(SCRIPT AnnotationsTestingAddManyRulers.py
  CJYX_ARGS --disable-cli-modules)

cjyx_add_python_test(SCRIPT AnnotationsTestingAddManyROIs.py
  CJYX_ARGS --disable-cli-modules)

cjyx_add_python_test(SCRIPT RemoveAnnotationsIDFromSelectionNode.py
  CJYX_ARGS --disable-cli-modules)

cjyx_add_python_test(SCRIPT LoadAnnotationRulerScene.py
  SCRIPT_ARGS ${INPUT}/ruler.mrml
  CJYX_ARGS --disable-cli-modules)

# Disabled scene view tests for now - they will be fixed in upcoming commit
# cjyx_add_python_test(SCRIPT AnnotationsTestingFiducialWithSceneViewRestore.py
#   CJYX_ARGS --disable-cli-modules)
