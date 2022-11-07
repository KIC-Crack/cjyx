set(SegmentStatisticsPlugins_PYTHON_SCRIPTS
  __init__
  SegmentStatisticsPluginBase
  LabelmapSegmentStatisticsPlugin
  ScalarVolumeSegmentStatisticsPlugin
  ClosedSurfaceSegmentStatisticsPlugin
  )

set(SegmentStatisticsPlugins_PYTHON_RESOURCES
  )

ctkMacroCompilePythonScript(
  TARGET_NAME SegmentStatisticsPlugins
  SCRIPTS "${SegmentStatisticsPlugins_PYTHON_SCRIPTS}"
  RESOURCES "${SegmentStatisticsPlugins_PYTHON_RESOURCES}"
  DESTINATION_DIR ${Cjyx_BINARY_DIR}/${Cjyx_QTSCRIPTEDMODULES_LIB_DIR}/SegmentStatisticsPlugins
  INSTALL_DIR ${Cjyx_INSTALL_QTSCRIPTEDMODULES_LIB_DIR}/SegmentStatisticsPlugins
  NO_INSTALL_SUBDIR
  )
