set(KIT ${PROJECT_NAME}StylePlugins)
project(${KIT})

#
# See CTK/CMake/ctkMacroBuildQtPlugin.cmake for details
#

# The header '${KIT}Export.h' will be automatically configured.
set(${KIT}_EXPORT "Q_CJYX_STYLES_PLUGINS_EXPORT")

# Source files
set(${KIT}_SRCS
  qCjyxStylePlugin.cxx
  qCjyxStylePlugin.h
  )

# Headers that should run through moc
set(${KIT}_MOC_SRCS
  qCjyxStylePlugin.h
  )

set(${KIT}_TARGET_LIBRARIES
  qCjyxBaseQTGUI
  )

ctkMacroBuildQtStylesPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT}
  FOLDER "Core-Base"
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
