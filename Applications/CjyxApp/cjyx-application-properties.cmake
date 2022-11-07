
set(APPLICATION_NAME
  Cjyx
  )

set(VERSION_MAJOR
  ${Cjyx_VERSION_MAJOR}
  )
set(VERSION_MINOR
  ${Cjyx_VERSION_MINOR}
  )
set(VERSION_PATCH
  ${Cjyx_VERSION_PATCH}
  )

set(DESCRIPTION_SUMMARY
  "Medical Visualization and Processing Environment for Research"
  )
set(DESCRIPTION_FILE
  ${Cjyx_SOURCE_DIR}/README.txt
  )

set(LAUNCHER_SPLASHSCREEN_FILE
  "${CMAKE_CURRENT_LIST_DIR}/Resources/Images/${APPLICATION_NAME}-SplashScreen.png"
  )
set(APPLE_ICON_FILE
  "${CMAKE_CURRENT_LIST_DIR}/Resources/${APPLICATION_NAME}.icns"
  )
set(WIN_ICON_FILE
  "${CMAKE_CURRENT_LIST_DIR}/Resources/${APPLICATION_NAME}.ico"
  )

set(LICENSE_FILE
  "${Cjyx_SOURCE_DIR}/License.txt"
  )
