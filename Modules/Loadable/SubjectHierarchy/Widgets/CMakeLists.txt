project(qCjyx${MODULE_NAME}ModuleWidgets)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "Q_CJYX_MODULE_${MODULE_NAME_UPPER}_WIDGETS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkCjyxSubjectHierarchyModuleLogic_INCLUDE_DIRS}
  ${qCjyxTerminologiesModuleWidgets_INCLUDE_DIRS}
  ${MRMLCore_INCLUDE_DIRS}
  )
if(Cjyx_BUILD_CLI_SUPPORT)
  list(APPEND ${KIT}_INCLUDE_DIRECTORIES
    ${MRMLCLI_INCLUDE_DIRS}
  )
endif()

set(${KIT}_SRCS
  qMRMLSubjectHierarchyModel.cxx
  qMRMLSubjectHierarchyModel.h
  qMRMLSubjectHierarchyModel_p.h
  qMRMLSortFilterSubjectHierarchyProxyModel.cxx
  qMRMLSortFilterSubjectHierarchyProxyModel.h
  qMRMLSubjectHierarchyTreeView.cxx
  qMRMLSubjectHierarchyTreeView.h
  qMRMLSubjectHierarchyComboBox.cxx
  qMRMLSubjectHierarchyComboBox.h
  qCjyxSubjectHierarchyPluginLogic.cxx
  qCjyxSubjectHierarchyPluginLogic.h
  qCjyxSubjectHierarchyAbstractPlugin.cxx
  qCjyxSubjectHierarchyAbstractPlugin.h
  qCjyxSubjectHierarchyPluginHandler.cxx
  qCjyxSubjectHierarchyPluginHandler.h
  qCjyxSubjectHierarchyDefaultPlugin.cxx
  qCjyxSubjectHierarchyDefaultPlugin.h
  qCjyxSubjectHierarchyCloneNodePlugin.cxx
  qCjyxSubjectHierarchyCloneNodePlugin.h
  qCjyxSubjectHierarchyParseLocalDataPlugin.cxx
  qCjyxSubjectHierarchyParseLocalDataPlugin.h
  qCjyxSubjectHierarchyRegisterPlugin.cxx
  qCjyxSubjectHierarchyRegisterPlugin.h
  qCjyxSubjectHierarchyFolderPlugin.cxx
  qCjyxSubjectHierarchyFolderPlugin.h
  qCjyxSubjectHierarchyOpacityPlugin.cxx
  qCjyxSubjectHierarchyOpacityPlugin.h
  qCjyxSubjectHierarchyViewContextMenuPlugin.cxx
  qCjyxSubjectHierarchyViewContextMenuPlugin.h
  qCjyxSubjectHierarchyVisibilityPlugin.cxx
  qCjyxSubjectHierarchyVisibilityPlugin.h
  qCjyxSubjectHierarchyExportPlugin.cxx
  qCjyxSubjectHierarchyExportPlugin.h
  )
if(Cjyx_USE_PYTHONQT)
  list(APPEND ${KIT}_SRCS
    qCjyxSubjectHierarchyScriptedPlugin.cxx
    qCjyxSubjectHierarchyScriptedPlugin.h
    )
endif()

set(${KIT}_MOC_SRCS
  qCjyxSubjectHierarchyPluginLogic.h
  qCjyxSubjectHierarchyAbstractPlugin.h
  qCjyxSubjectHierarchyPluginHandler.h
  qCjyxSubjectHierarchyDefaultPlugin.h
  qCjyxSubjectHierarchyCloneNodePlugin.h
  qCjyxSubjectHierarchyParseLocalDataPlugin.h
  qCjyxSubjectHierarchyRegisterPlugin.h
  qCjyxSubjectHierarchyFolderPlugin.h
  qCjyxSubjectHierarchyOpacityPlugin.h
  qCjyxSubjectHierarchyViewContextMenuPlugin.h
  qCjyxSubjectHierarchyVisibilityPlugin.h
  qMRMLSubjectHierarchyTreeView.h
  qMRMLSubjectHierarchyComboBox.h
  qMRMLSubjectHierarchyModel.h
  qMRMLSortFilterSubjectHierarchyProxyModel.h
  qCjyxSubjectHierarchyExportPlugin.h
  )
if(Cjyx_USE_PYTHONQT)
  list(APPEND ${KIT}_MOC_SRCS
    qCjyxSubjectHierarchyScriptedPlugin.h
    )
endif()

set(${KIT}_UI_SRCS
  )

set(${KIT}_RESOURCES
  Resources/${KIT}.qrc
  )

#-----------------------------------------------------------------------------
set_source_files_properties( qCjyxSubjectHierarchyAbstractPlugin.h WRAP_EXCLUDE )

set(${KIT}_TARGET_LIBRARIES
  vtkCjyx${MODULE_NAME}ModuleLogic
  qCjyxTerminologiesModuleWidgets
  MRMLCore
  vtkCjyxVolumesModuleLogic
  )
if(Cjyx_BUILD_CLI_SUPPORT)
  list(APPEND ${KIT}_TARGET_LIBRARIES
    MRMLCLI
  )
endif()

#-----------------------------------------------------------------------------
CjyxMacroBuildModuleWidgets(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  UI_SRCS ${${KIT}_UI_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  RESOURCES ${${KIT}_RESOURCES}
  WRAP_PYTHONQT
  )

#-----------------------------------------------------------------------------
if(Cjyx_USE_PYTHONQT)
  add_subdirectory(Python)
endif()

#-----------------------------------------------------------------------------
if(Cjyx_BUILD_QT_DESIGNER_PLUGINS)
  add_subdirectory(DesignerPlugins)
endif()
