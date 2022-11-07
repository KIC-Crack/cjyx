
include(BundleUtilities)

# Sanity checks
set(expected_existing_vars
  Cjyx_INSTALL_DIR
  Cjyx_MAIN_PROJECT_APPLICATION_NAME
  Cjyx_VERSION_FULL
  )
foreach(var ${expected_existing_vars})
  if(NOT EXISTS "${MY_${var}}")
    message(FATAL_ERROR "Variable ${var} is set to an inexistent directory or file ! [${${var}}]")
  endif()
endforeach()

set(app_name "${Cjyx_MAIN_PROJECT_APPLICATION_NAME}.app")
verify_app("${Cjyx_INSTALL_DIR}/${app_name")
