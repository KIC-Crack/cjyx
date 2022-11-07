#cjyx_add_python_test(SCRIPT LoadVolumeDisplaybleSceneModelClose.py
#  CJYX_ARGS --disable-cli-modules)

cjyx_add_python_unittest(SCRIPT LoadVolumeDisplaybleSceneModelClose.py)

cjyx_add_python_unittest(CJYX_ARGS --disable-cli-modules SCRIPT VolumesLogicCompareVolumeGeometry.py)
