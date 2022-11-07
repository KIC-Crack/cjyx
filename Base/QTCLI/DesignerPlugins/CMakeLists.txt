set(KIT ${PROJECT_NAME}Plugins)
project(${KIT})

#
# See CTK/CMake/ctkMacroBuildQtPlugin.cmake for details
#

# The header '${KIT}Export.h' will be automatically configured.
set(${KIT}_EXPORT "Q_CJYX_BASE_QTCLI_PLUGINS_EXPORT")

# Source files
set(${KIT}_SRCS
  qCjyxQTCLIAbstractPlugin.cxx
  qCjyxQTCLIAbstractPlugin.h
  qCjyxQTCLIPlugins.cxx
  qCjyxQTCLIPlugins.h

  qCjyxCLIProgressBarPlugin.cxx
  qCjyxCLIProgressBarPlugin.h
  )

# Headers that should run through moc
set(${KIT}_MOC_SRCS
  qCjyxQTCLIPlugins.h
  qCjyxCLIProgressBarPlugin.h
  )

set(${KIT}_TARGET_LIBRARIES
  qCjyxBaseQTCLI
  )

ctkMacroBuildQtDesignerPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT}
  FOLDER "Core-Base"
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
