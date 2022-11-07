project(qCjyxBaseQTGUI)

#
# See CMake/CjyxMacroBuildBaseQtLibrary.cmake for details
#

set(KIT_export_directive "Q_CJYX_BASE_QTGUI_EXPORT")

# Additional directories to include
set(KIT_include_directories
  ${MRMLLogic_INCLUDE_DIRS}
  ${MRMLDisplayableManager_INCLUDE_DIRS}
  ${qCjyxBaseQTCore_SOURCE_DIR}
  ${qCjyxBaseQTCore_BINARY_DIR}
  )

# Source files
set(KIT_SRCS
  qCjyxAbstractModule.cxx
  qCjyxAbstractModule.h
  qCjyxAbstractModulePanel.cxx
  qCjyxAbstractModulePanel.h
  qCjyxAbstractModuleWidget.cxx
  qCjyxAbstractModuleWidget.h
  qCjyxActionsDialog.cxx
  qCjyxActionsDialog.h
  qCjyxApplication.cxx
  qCjyxApplication.h
  qCjyxCommandOptions.cxx
  qCjyxCommandOptions.h
  qCjyxDarkStyle.cxx
  qCjyxDarkStyle.h
  qCjyxDataDialog.cxx
  qCjyxDataDialog.h
  qCjyxDirectoryListView.cxx
  qCjyxDirectoryListView.h
  qCjyxExportNodeDialog.cxx
  qCjyxExportNodeDialog.h

  qCjyxFileDialog.cxx
  qCjyxFileDialog.h
  qCjyxFileWriterOptionsWidget.cxx
  qCjyxFileWriterOptionsWidget.h
  qCjyxIOManager.cxx
  qCjyxIOManager.h
  qCjyxIOOptionsWidget.cxx
  qCjyxIOOptionsWidget.h
  qCjyxLayoutManager.cxx
  qCjyxLayoutManager.h
  qCjyxLightStyle.cxx
  qCjyxLightStyle.h
  qCjyxLoadableModule.cxx
  qCjyxLoadableModule.h

  qCjyxModelsDialog.cxx
  qCjyxModelsDialog.h
  qCjyxModelsDialog_p.h

  qCjyxModuleFactoryFilterModel.cxx
  qCjyxModuleFactoryFilterModel.h
  qCjyxModuleFinderDialog.cxx
  qCjyxModuleFinderDialog.h
  qCjyxModulesListView.cxx
  qCjyxModulesListView.h
  qCjyxModulesMenu.cxx
  qCjyxModulesMenu.h

  qCjyxModulePanel.cxx
  qCjyxModulePanel.h

  qCjyxModuleSelectorToolBar.cxx
  qCjyxModuleSelectorToolBar.h
  qCjyxMouseModeToolBar.cxx
  qCjyxMouseModeToolBar.h
  qCjyxMouseModeToolBar_p.h
  qCjyxNodeWriter.cxx
  qCjyxNodeWriter.h
  qCjyxNodeWriterOptionsWidget.cxx
  qCjyxNodeWriterOptionsWidget.h
  qCjyxNodeWriterOptionsWidget_p.h
  qCjyxQListQVariantMapMetaType.h
  qCjyxSaveDataDialog.cxx
  qCjyxSaveDataDialog.h
  qCjyxSettingsCachePanel.cxx
  qCjyxSettingsCachePanel.h
  qCjyxSettingsGeneralPanel.cxx
  qCjyxSettingsGeneralPanel.h
  qCjyxSettingsModulesPanel.cxx
  qCjyxSettingsModulesPanel.h
  qCjyxSettingsStylesPanel.cxx
  qCjyxSettingsStylesPanel.h
  qCjyxSettingsViewsPanel.cxx
  qCjyxSettingsViewsPanel.h
  qCjyxSettingsDeveloperPanel.cxx
  qCjyxSettingsDeveloperPanel.h
  qCjyxSettingsUserInformationPanel.cxx
  qCjyxSettingsUserInformationPanel.h
  qCjyxStyle.cxx
  qCjyxStyle.h
  qCjyxViewersToolBar.cxx
  qCjyxViewersToolBar.h
  qCjyxViewersToolBar_p.h
  qCjyxWidget.cxx
  qCjyxWidget.h

  qCjyxSingletonViewFactory.cxx
  qCjyxSingletonViewFactory.h
  )

if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  list(APPEND KIT_SRCS
    qCjyxExtensionsLocalWidget.cxx
    qCjyxExtensionsLocalWidget.h
    qCjyxExtensionsManagerDialog.cxx
    qCjyxExtensionsManagerDialog.h
    qCjyxExtensionsManagerWidget.cxx
    qCjyxExtensionsManagerWidget.h
    qCjyxSettingsExtensionsPanel.cxx
    qCjyxSettingsExtensionsPanel.h
    )
  if(Cjyx_BUILD_WEBENGINE_SUPPORT)
    list(APPEND KIT_SRCS
      qCjyxExtensionsServerWidget.cxx
      qCjyxExtensionsServerWidget.h
      qCjyxExtensionsServerWidget_p.h
      )
  endif()
endif()

if(Cjyx_BUILD_I18N_SUPPORT)
  list(APPEND KIT_SRCS
    qCjyxSettingsInternationalizationPanel.cxx
    qCjyxSettingsInternationalizationPanel.h
    )
endif()

if(Cjyx_BUILD_WEBENGINE_SUPPORT)
  list(APPEND KIT_SRCS
    qCjyxWebPythonProxy.cxx
    qCjyxWebPythonProxy.h
    qCjyxWebDownloadWidget.cxx
    qCjyxWebDownloadWidget.h
    qCjyxWebWidget.cxx
    qCjyxWebWidget.h
    qCjyxWebWidget_p.h
    )
endif()

if(Cjyx_USE_PYTHONQT)
  list(APPEND KIT_SRCS
    qCjyxScriptedLoadableModuleFactory.cxx
    qCjyxScriptedLoadableModuleFactory.h
    qCjyxScriptedLoadableModuleWidget.cxx
    qCjyxScriptedLoadableModuleWidget.h
    qCjyxScriptedLoadableModule.cxx
    qCjyxScriptedLoadableModule.h
    qCjyxScriptedFileDialog.cxx
    qCjyxScriptedFileDialog.h
    )
endif()

if(Cjyx_USE_PYTHONQT)
  list(APPEND KIT_SRCS
    qCjyxSettingsPythonPanel.cxx
    qCjyxSettingsPythonPanel.h
    )
endif()

# Since the wrapper doesn't consider hierarchy of classes, let's exclude
# classes deriving from pure abstract class and remaining abstract pure
set_source_files_properties(
  qCjyxAbstractModule.h
  qCjyxAbstractModulePanel.h
  qCjyxAbstractModuleWidget.h
  qCjyxFileDialog.h
  qCjyxLoadableModule.h
  WRAP_EXCLUDE
  )

# Headers that should run through moc
set(KIT_MOC_SRCS
  qCjyxAbstractModule.h
  qCjyxAbstractModulePanel.h
  qCjyxAbstractModuleWidget.h
  qCjyxActionsDialog.h
  qCjyxApplication.h
  qCjyxCommandOptions.h
  qCjyxDarkStyle.h
  qCjyxDataDialog.h
  qCjyxDataDialog_p.h
  qCjyxDirectoryListView.h
  qCjyxExportNodeDialog.h
  qCjyxExportNodeDialog_p.h
  qCjyxFileDialog.h
  qCjyxFileWriterOptionsWidget.h
  qCjyxIOManager.h
  qCjyxIOOptionsWidget.h
  qCjyxLayoutManager.h
  qCjyxLightStyle.h
  qCjyxLoadableModule.h

  qCjyxModelsDialog.h
  qCjyxModelsDialog_p.h

  qCjyxModuleFactoryFilterModel.h
  qCjyxModuleFinderDialog.h
  qCjyxModulesListView.h
  qCjyxModulesMenu.h
  qCjyxModulePanel.h
  qCjyxModuleSelectorToolBar.h
  qCjyxMouseModeToolBar.h
  qCjyxMouseModeToolBar_p.h
  qCjyxNodeWriter.h
  qCjyxNodeWriterOptionsWidget.h
  qCjyxNodeWriterOptionsWidget_p.h
  qCjyxSaveDataDialog.h
  qCjyxSaveDataDialog_p.h
  qCjyxSettingsCachePanel.h
  qCjyxSettingsGeneralPanel.h
  qCjyxSettingsModulesPanel.h
  qCjyxSettingsStylesPanel.h
  qCjyxSettingsViewsPanel.h
  qCjyxSettingsDeveloperPanel.h
  qCjyxSettingsUserInformationPanel.h
  qCjyxStyle.h
  qCjyxViewersToolBar.h
  qCjyxViewersToolBar_p.h
  qCjyxWidget.h

  qCjyxSingletonViewFactory.h
  )

if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  list(APPEND KIT_MOC_SRCS
    qCjyxExtensionsLocalWidget.h
    qCjyxExtensionsManagerDialog.h
    qCjyxExtensionsManagerWidget.h
    qCjyxSettingsExtensionsPanel.h
    )
  if(Cjyx_BUILD_WEBENGINE_SUPPORT)
    list(APPEND KIT_MOC_SRCS
      qCjyxExtensionsServerWidget.h
      qCjyxExtensionsServerWidget_p.h
      )
  endif()
endif()

if(Cjyx_BUILD_I18N_SUPPORT)
  list(APPEND KIT_MOC_SRCS
    qCjyxSettingsInternationalizationPanel.h
    )
endif()

if(Cjyx_BUILD_WEBENGINE_SUPPORT)
  list(APPEND KIT_MOC_SRCS
    qCjyxWebDownloadWidget.h
    qCjyxWebPythonProxy.h
    qCjyxWebWidget.h
    qCjyxWebWidget_p.h
    )
endif()

if(Cjyx_USE_PYTHONQT)
  list(APPEND KIT_MOC_SRCS
    qCjyxScriptedLoadableModule.h
    qCjyxScriptedLoadableModuleWidget.h
    qCjyxScriptedFileDialog.h
    )
endif()

# PythonQt decorator
if(Cjyx_USE_PYTHONQT)
  list(APPEND KIT_MOC_SRCS
    qCjyxSettingsPythonPanel.h
    )
endif()

# UI files
set(KIT_UI_SRCS
  Resources/UI/qCjyxActionsDialog.ui
  Resources/UI/qCjyxDataDialog.ui
  Resources/UI/qCjyxExportNodeDialog.ui
  Resources/UI/qCjyxModelsDialog.ui
  Resources/UI/qCjyxModuleFinderDialog.ui
  Resources/UI/qCjyxModulePanel.ui
  Resources/UI/qCjyxNodeWriterOptionsWidget.ui
  Resources/UI/qCjyxSaveDataDialog.ui
  Resources/UI/qCjyxSettingsCachePanel.ui
  Resources/UI/qCjyxSettingsGeneralPanel.ui
  Resources/UI/qCjyxSettingsInternationalizationPanel.ui
  Resources/UI/qCjyxSettingsModulesPanel.ui
  Resources/UI/qCjyxSettingsStylesPanel.ui
  Resources/UI/qCjyxSettingsViewsPanel.ui
  Resources/UI/qCjyxSettingsDeveloperPanel.ui
  Resources/UI/qCjyxSettingsUserInformationPanel.ui
  Resources/UI/qCjyxWebWidget.ui
  )
if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  list(APPEND KIT_UI_SRCS
    Resources/UI/qCjyxExtensionsActionsWidget.ui
    Resources/UI/qCjyxExtensionsButtonBox.ui
    Resources/UI/qCjyxExtensionsManagerDialog.ui
    Resources/UI/qCjyxExtensionsManagerWidget.ui
    Resources/UI/qCjyxExtensionsToolsWidget.ui
    Resources/UI/qCjyxSettingsExtensionsPanel.ui
    )
endif()

# PythonQt decorator
if(Cjyx_USE_PYTHONQT)
  list(APPEND KIT_UI_SRCS
    Resources/UI/qCjyxSettingsPythonPanel.ui
  )
endif()

# Additional Target libraries (Qt libs already included)
set(KIT_target_libraries
  qCjyxBaseQTCore
  qMRMLWidgets
  )

# Python wrap
if(Cjyx_USE_PYTHONQT)
  list(APPEND KIT_target_libraries
    CTKScriptingPythonWidgets
    )
endif()

# DICOM
if(Cjyx_BUILD_DICOM_SUPPORT)
  list(APPEND KIT_target_libraries
    CTKDICOMWidgets
    )
endif()

# Resources
set(KIT_resources
  Resources/qCjyxBaseQTGUI.qrc
  ${qMRMLWidgets_SOURCE_DIR}/Resources/qMRMLWidgets.qrc
  )

# Python wrap
if(Cjyx_USE_PYTHONQT)

  # Sources
  list(APPEND KIT_SRCS
    qCjyxPythonManager.cxx
    qCjyxPythonManager.h
    )

  # Headers that should run through moc
  list(APPEND KIT_MOC_SRCS
    qCjyxPythonManager.h
    )

endif()

CjyxMacroBuildBaseQtLibrary(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${KIT_export_directive}
  INCLUDE_DIRECTORIES ${KIT_include_directories}
  SRCS ${KIT_SRCS}
  MOC_SRCS ${KIT_MOC_SRCS}
  UI_SRCS ${KIT_UI_SRCS}
  TARGET_LIBRARIES ${KIT_target_libraries}
  RESOURCES ${KIT_resources}
  WRAP_PYTHONQT
  )

#-----------------------------------------------------------------------------
# Cjyx icon engine
#-----------------------------------------------------------------------------

set(CTK_LIBRARIES CTKWidgets)
ctkMacroBuildQtIconEnginesPlugin(
  NAME qCjyxIconEnginePlugin
  EXPORT_DIRECTIVE "Q_CJYX_BASE_QTGUI_ICON_ENGINE_EXPORT"
  FOLDER "Core-Base"
  SRCS qCjyxIconEnginePlugin.cxx qCjyxIconEnginePlugin.h
  MOC_SRCS qCjyxIconEnginePlugin.h
  TARGET_LIBRARIES ${QT_LIBRARIES} ${CTK_LIBRARIES}
  )

#-----------------------------------------------------------------------------
# Qt Plugins
#-----------------------------------------------------------------------------
if(Cjyx_BUILD_QT_DESIGNER_PLUGINS)
  add_subdirectory(DesignerPlugins)
endif()
add_subdirectory(StylePlugins)

#-----------------------------------------------------------------------------
# Testing
# Note: Within both Testing/Cxx and Testing/Python, some code is useful to both Cjyx and the
#       Extensions. For that reason, the Testing directory is added unconditionally.
#       The code specific to Cjyx will be excluded directly in either Testing/Python
#       or Testing/Cxx.
add_subdirectory(Testing)
