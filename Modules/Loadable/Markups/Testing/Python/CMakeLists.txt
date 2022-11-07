include(CjyxMacroBuildScriptedModule)
set(MARKUPS_PYTHON_RESOURCES
  )

# Test adding many fiducials
cjyxMacroBuildScriptedModule(
  NAME AddManyMarkupsFiducialTest
  SCRIPTS AddManyMarkupsFiducialTest.py
  RESOURCES ${MARKUPS_PYTHON_RESOURCES}
  )
cjyx_add_python_unittest(SCRIPT AddManyMarkupsFiducialTest.py
                           CJYX_ARGS --disable-cli-modules)

# Disabled scene view tests for now - they will be fixed in upcoming commit
# # Test scene views
# cjyx_add_python_test(SCRIPT MarkupsSceneViewRestoreTestSimple.py
#                        CJYX_ARGS --disable-cli-modules)
# cjyx_add_python_test(SCRIPT MarkupsSceneViewRestoreTestManyLists.py
#                        CJYX_ARGS --disable-cli-modules)

# Test fiducials in the neurosurgical planning tutorial
cjyxMacroBuildScriptedModule(
  NAME NeurosurgicalPlanningTutorialMarkupsSelfTest
  SCRIPTS NeurosurgicalPlanningTutorialMarkupsSelfTest.py
  RESOURCES ${MARKUPS_PYTHON_RESOURCES}
  )
cjyx_add_python_unittest(SCRIPT NeurosurgicalPlanningTutorialMarkupsSelfTest.py)

# Test fiducials in compare viewers
cjyxMacroBuildScriptedModule(
  NAME MarkupsInCompareViewersSelfTest
  SCRIPTS MarkupsInCompareViewersSelfTest.py
  RESOURCES ${MARKUPS_PYTHON_RESOURCES}
  )
cjyx_add_python_unittest(SCRIPT MarkupsInCompareViewersSelfTest.py)

# Test fiducials in different views
cjyxMacroBuildScriptedModule(
  NAME MarkupsInViewsSelfTest
  SCRIPTS MarkupsInViewsSelfTest.py
  RESOURCES ${MARKUPS_PYTHON_RESOURCES}
  )
cjyx_add_python_unittest(SCRIPT MarkupsInViewsSelfTest.py
                           CJYX_ARGS --disable-cli-modules)

# Test curvature computation
cjyx_add_python_test(SCRIPT MarkupsCurveMeasurementsTest.py
                       CJYX_ARGS --disable-cli-modules)

# Visual test of coordinate system along the curve (normal, binormal, tangent computation)
cjyx_add_python_test(SCRIPT MarkupsCurveCoordinateFrameTest.py
                       CJYX_ARGS --disable-cli-modules)

# Test other measurements
cjyx_add_python_test(SCRIPT MarkupsMeasurementsTest.py
                       CJYX_ARGS --disable-cli-modules)

# Test markups pluggable architecture
cjyxMacroBuildScriptedModule(
  NAME PluggableMarkupsSelfTest
  SCRIPTS PluggableMarkupsSelfTest.py
  RESOURCES ${MARKUPS_PYTHON_RESOURCES}
  )
cjyx_add_python_unittest(SCRIPT PluggableMarkupsSelfTest.py)
