set(KIT ${PROJECT_NAME}DesignerPlugins)
project(${KIT})

#
# See CTK/CMake/ctkMacroBuildQtPlugin.cmake for details
#

# The header '${KIT}Export.h' will be automatically configured.
set(${KIT}_EXPORT "Q_CJYX_DESIGNER_PLUGINS_EXPORT")

# Source files
set(${KIT}_SRCS
  qCjyxQTGUIAbstractPlugin.cxx
  qCjyxQTGUIAbstractPlugin.h
  qCjyxQTGUIPlugins.cxx
  qCjyxQTGUIPlugins.h

  qCjyxDirectoryListViewPlugin.cxx
  qCjyxDirectoryListViewPlugin.h
  qCjyxModulePanelPlugin.cxx
  qCjyxModulePanelPlugin.h
  qCjyxModulesListViewPlugin.cxx
  qCjyxModulesListViewPlugin.h
  qCjyxMouseModeToolBarPlugin.cxx
  qCjyxMouseModeToolBarPlugin.h
  qCjyxWidgetPlugin.cxx
  qCjyxWidgetPlugin.h
  )

# Headers that should run through moc
set(${KIT}_MOC_SRCS
  qCjyxQTGUIPlugins.h

  qCjyxDirectoryListViewPlugin.h
  qCjyxModulePanelPlugin.h
  qCjyxModulesListViewPlugin.h
  qCjyxMouseModeToolBarPlugin.h
  qCjyxWidgetPlugin.h
  )

set(${KIT}_TARGET_LIBRARIES
  qCjyxBaseQTGUI
  )
if(Cjyx_BUILD_CLI_SUPPORT)
  list(APPEND ${KIT}_TARGET_LIBRARIES
    qCjyxBaseQTCLI
    )
endif()

ctkMacroBuildQtDesignerPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT}
  FOLDER "Core-Base"
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
