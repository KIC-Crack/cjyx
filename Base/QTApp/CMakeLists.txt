project(qCjyxBaseQTApp)

#
# See CMake/CjyxMacroBuildBaseQtLibrary.cmake for details
#

set(KIT_export_directive "Q_CJYX_BASE_QTAPP_EXPORT")

# Additional directories to include
set(KIT_include_directories
  ${qCjyxBaseQTCore_SOURCE_DIR}
  ${qCjyxBaseQTCore_BINARY_DIR}
  ${qCjyxBaseQTGUI_SOURCE_DIR}
  ${qCjyxBaseQTGUI_BINARY_DIR}
  ${qCjyxModulesCore_SOURCE_DIR}
  ${qCjyxModulesCore_BINARY_DIR}
  )

# Source files
set(KIT_SRCS
  qCjyxAboutDialog.cxx
  qCjyxAboutDialog.h
  qCjyxApplicationHelper.cxx
  qCjyxApplicationHelper.h
  qCjyxErrorReportDialog.cxx
  qCjyxErrorReportDialog.h
  qCjyxMainWindow.cxx
  qCjyxMainWindow.h
  )

# Headers that should run through moc
set(KIT_MOC_SRCS
  qCjyxAboutDialog.h
  qCjyxApplicationHelper.h
  qCjyxErrorReportDialog.h
  qCjyxMainWindow.h
  )

# UI files
set(KIT_UI_SRCS
  Resources/UI/qCjyxAboutDialog.ui
  Resources/UI/qCjyxErrorReportDialog.ui
  Resources/UI/qCjyxMainWindow.ui
  )

# Additional Target libraries (Qt libs already included)
set(KIT_target_libraries
  # ${QT_LIBRARIES} # Not needed: All dependencies are transitively satisfied by other targets
  qCjyxModulesCore
  )

if(Cjyx_BUILD_CLI_SUPPORT)
  list(APPEND KIT_target_libraries
    qCjyxBaseQTCLI
    )
endif()

# Resources
set(KIT_resources
  )

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

# --------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()

