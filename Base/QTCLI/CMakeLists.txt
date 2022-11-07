project(qCjyxBaseQTCLI)

#
# Dependencies
#

#
# ModuleDescriptionParser
#
find_package(CjyxExecutionModel REQUIRED ModuleDescriptionParser)

#
# ITK - Import ITK targets required by ModuleDescriptionParser
#
set(${PROJECT_NAME}_ITK_COMPONENTS
  ${ModuleDescriptionParser_ITK_COMPONENTS}
  )
find_package(ITK 4.6 COMPONENTS ${${PROJECT_NAME}_ITK_COMPONENTS} REQUIRED)

#
# VTK
#
if(Cjyx_USE_PYTHONQT AND NOT VTK_WRAP_PYTHON)
  message(FATAL_ERROR "Since Cjyx_USE_PYTHONQT is ON, VTK_WRAP_PYTHON is expected to be ON. "
                      "Re-configure VTK with python wrapping.")
endif()

#
# See CMake/CjyxMacroBuildBaseQtLibrary.cmake for details
#

set(KIT_export_directive "Q_CJYX_BASE_QTCLI_EXPORT")

# Additional directories to include
set(KIT_include_directories
  ${qCjyxBaseQTCore_SOURCE_DIR}
  ${qCjyxBaseQTCore_BINARY_DIR}
  ${qCjyxBaseQTGUI_SOURCE_DIR}
  ${qCjyxBaseQTGUI_BINARY_DIR}
  ${ModuleDescriptionParser_INCLUDE_DIRS}
  ${MRMLCLI_INCLUDE_DIRS}
  ${MRMLLogic_INCLUDE_DIRS}
  )

# Source files
set(KIT_VTK_SRCS
  vtkCjyxCLIModuleLogic.cxx
  vtkCjyxCLIModuleLogic.h
  )

# Source files
set(KIT_SRCS
  ${KIT_VTK_SRCS}
  qCjyxCLIExecutableModuleFactory.cxx
  qCjyxCLIExecutableModuleFactory.h
  qCjyxCLILoadableModuleFactory.cxx
  qCjyxCLILoadableModuleFactory.h
  qCjyxCLIModule.cxx
  qCjyxCLIModule.h
  qCjyxCLIModuleFactoryHelper.cxx
  qCjyxCLIModuleFactoryHelper.h
  qCjyxCLIModuleUIHelper.cxx
  qCjyxCLIModuleUIHelper.h
  qCjyxCLIModuleWidget.cxx
  qCjyxCLIModuleWidget.h
  qCjyxCLIModuleWidget_p.h
  qCjyxCLIProgressBar.cxx
  qCjyxCLIProgressBar.h
  )

# Headers that should run through moc
set(KIT_MOC_SRCS
  qCjyxCLIModule.h
  qCjyxCLIModuleWidget.h
  qCjyxCLIModuleWidget_p.h
  qCjyxCLIModuleUIHelper.h
  qCjyxCLIProgressBar.h
  )

# UI files
set(KIT_UI_SRCS
  Resources/UI/qCjyxCLIModuleWidget.ui
  )

# Resources
set(KIT_resources
  )

# Additional Target libraries (Qt libs already included)
set(KIT_target_libraries
  # ${QT_LIBRARIES} # Not needed: All dependencies are transitively satisfied by other targets
  qCjyxBaseQTCore
  qCjyxBaseQTGUI
  ModuleDescriptionParser ${ITK_LIBRARIES}
  MRMLCLI
  )
if(VTK_WRAP_PYTHON AND ${VTK_VERSION} VERSION_GREATER_EQUAL "8.90")
  # HACK Explicitly list transitive VTK dependencies because _get_dependencies_recurse
  # used in vtkAddon/CMake/vtkMacroKitPythonWrap.cmake only recurses over dependencies
  # that are VTK python wrapped.
  list(APPEND KIT_target_libraries
    CjyxBaseLogic
    )
endif()

if(Cjyx_USE_QtTesting)
  list(APPEND KIT_SRCS
    qCjyxCLIModuleWidgetEventPlayer.cxx
    qCjyxCLIModuleWidgetEventPlayer.h
    )
  list(APPEND KIT_MOC_SRCS
    qCjyxCLIModuleWidgetEventPlayer.h
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

if(Cjyx_BUILD_QT_DESIGNER_PLUGINS)
  add_subdirectory(DesignerPlugins)
endif()

# --------------------------------------------------------------------------
# Python Wrapping
# --------------------------------------------------------------------------
if(VTK_WRAP_PYTHON)
  include(vtkMacroKitPythonWrap)

  set(_python_wrapped_libraries
    )

  vtkMacroKitPythonWrap(
    KIT_NAME ${PROJECT_NAME}
    KIT_SRCS ${KIT_VTK_SRCS}
    KIT_INSTALL_BIN_DIR ${Cjyx_INSTALL_BIN_DIR}
    KIT_INSTALL_LIB_DIR ${Cjyx_INSTALL_LIB_DIR}
    KIT_PYTHON_LIBRARIES ${_python_wrapped_libraries}
    )
  set_property(GLOBAL APPEND PROPERTY Cjyx_TARGETS ${PROJECT_NAME}Python)
  # Folder
  set_target_properties(${PROJECT_NAME}Python PROPERTIES FOLDER "Core-Base")
  if(TARGET ${PROJECT_NAME}Hierarchy)
    set_target_properties(${PROJECT_NAME}Hierarchy PROPERTIES FOLDER "Core-Base")
  endif()
endif()

# --------------------------------------------------------------------------
# Testing
# --------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
