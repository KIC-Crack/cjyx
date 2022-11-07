
project(CjyxApp)

include(CjyxMacroBuildApplication)

# --------------------------------------------------------------------------
# Properties
# --------------------------------------------------------------------------
CjyxReadApplicationProperties()

# --------------------------------------------------------------------------
# Folder
# --------------------------------------------------------------------------
set(${PROJECT_NAME}_FOLDER "App-${CjyxApp_APPLICATION_NAME}")

# --------------------------------------------------------------------------
# Application library
# --------------------------------------------------------------------------
set(APPLIB_NAME "q${PROJECT_NAME}")

set(APPLIB_SRCS
  qCjyxAppMainWindow.cxx
  qCjyxAppMainWindow.h
  qCjyxAppMainWindow_p.h
  )

set(APPLIB_MOC_SRCS
  qCjyxAppMainWindow.h
  )

set(APPLIB_UI_SRCS
  )

set(APPLIB_RESOURCES
  Resources/CjyxApp.qrc
  ${qCjyxBaseQTGUI_SOURCE_DIR}/Resources/qCjyxBaseQTGUI.qrc
  )

cjyxMacroBuildAppLibrary(
  NAME ${APPLIB_NAME}
  APPLICATION_NAME ${CjyxApp_APPLICATION_NAME}
  DESCRIPTION_SUMMARY ${CjyxApp_DESCRIPTION_SUMMARY}
  DESCRIPTION_FILE ${CjyxApp_DESCRIPTION_FILE}
  EXPORT_DIRECTIVE "Q_CJYX_APP_EXPORT"
  FOLDER ${${PROJECT_NAME}_FOLDER}
  SRCS ${APPLIB_SRCS}
  MOC_SRCS ${APPLIB_MOC_SRCS}
  UI_SRCS ${APPLIB_UI_SRCS}
  RESOURCES ${APPLIB_RESOURCES}
  )

# --------------------------------------------------------------------------
# Application executable
# --------------------------------------------------------------------------

# Configure launcher only for the main application
set(extra_args)
if(${PROJECT_NAME} STREQUAL ${Cjyx_MAIN_PROJECT})
  set(extra_args CONFIGURE_LAUNCHER)
endif()

set(APP_SRCS
  Main.cxx
  )

cjyxMacroBuildApplication(
  NAME ${PROJECT_NAME}
  APPLICATION_NAME ${${PROJECT_NAME}_APPLICATION_NAME}
  LAUNCHER_SPLASHSCREEN_FILE ${CjyxApp_LAUNCHER_SPLASHSCREEN_FILE}
  APPLE_ICON_FILE ${CjyxApp_APPLE_ICON_FILE}
  WIN_ICON_FILE ${CjyxApp_WIN_ICON_FILE}
  LICENSE_FILE ${CjyxApp_LICENSE_FILE}
  FOLDER ${${PROJECT_NAME}_FOLDER}
  SRCS ${APP_SRCS}
  TARGET_LIBRARIES ${APPLIB_NAME}
  TARGET_NAME_VAR "APP_TARGET_NAME"
  ${extra_args}
  )

# --------------------------------------------------------------------------
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
