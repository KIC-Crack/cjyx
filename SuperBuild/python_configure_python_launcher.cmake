cmake_minimum_required(VERSION 3.16.3...3.19.7 FATAL_ERROR)

#
# This script implements different actions that may be selected
# by setting the ACTION variable.
#
# Available actions are the following:
#
# - default
# - replace_application_name
#

if(NOT "${ACTION}" MATCHES "^default|replace_application_name$")
  message(FATAL_ERROR "Unknown ACTION [${ACTION}]. Supported action are 'replace_application_name' and 'default'")
endif()

# Requirements common to all actions
foreach(varname IN ITEMS
  PYTHON_REAL_EXECUTABLE
  )
  if("${${varname}}" STREQUAL "")
    message(FATAL_ERROR "${varname} is empty")
  endif()
endforeach()

get_filename_component(python_bin_dir "${PYTHON_REAL_EXECUTABLE}" PATH)

# --------------------------------------------------------------------------
# replace_application_name
if("${ACTION}" STREQUAL "replace_application_name")
  # LauncherSettings
  set(filename "${python_bin_dir}/PythonCjyxLauncherSettings.ini")
  message(STATUS "Replacing 'name=PythonCjyx' with 'name=${Cjyx_MAIN_PROJECT_APPLICATION_NAME}' in ${filename}")
  file(READ ${filename} content)
  string(REPLACE "name=PythonCjyx" "name=${Cjyx_MAIN_PROJECT_APPLICATION_NAME}" content ${content})
  file(WRITE ${filename} ${content})

  # LauncherSettingsToInstall
  set(filename "${python_bin_dir}/PythonCjyxLauncherSettingsToInstall.ini")
  message(STATUS "Replacing 'name=PythonCjyx' with 'name=${Cjyx_MAIN_PROJECT_APPLICATION_NAME}' in ${filename}")
  file(READ ${filename} content)
  string(REPLACE "name=PythonCjyx" "name=${Cjyx_MAIN_PROJECT_APPLICATION_NAME}" content ${content})
  file(WRITE ${filename} ${content})

  return()
endif()

# --------------------------------------------------------------------------
# default
foreach(varname IN ITEMS
  CMAKE_EXECUTABLE_SUFFIX
  CJYX_REVISION_SPECIFIC_USER_SETTINGS_FILEBASENAME
  )
  if(NOT DEFINED ${varname})
    message(FATAL_ERROR "${varname} is not defined")
  endif()
endforeach()

foreach(varname IN ITEMS
  CTKAppLauncher_DIR
  python_DIR
  PYTHON_ENABLE_SSL
  PYTHON_SHARED_LIBRARY_DIR
  PYTHON_SITE_PACKAGES_SUBDIR
  PYTHON_STDLIB_SUBDIR
  Cjyx_BIN_DIR
  Cjyx_BINARY_DIR
  Cjyx_LIB_DIR
  Cjyx_SHARE_DIR
  Cjyx_SOURCE_DIR
  Cjyx_REVISION
  Cjyx_ORGANIZATION_DOMAIN
  Cjyx_ORGANIZATION_NAME
  )
  if("${${varname}}" STREQUAL "")
    message(FATAL_ERROR "${varname} is empty")
  endif()
endforeach()

find_package(CTKAppLauncher REQUIRED)

#
# Settings specific to the build tree.
#

set(PYTHONHOME "${python_DIR}")

# cjyx_dll_directories
#
# Helper module for providing additional search paths for native dependencies
# when importing extension modules or loading DLLs using ctypes.
#
# See https://docs.python.org/3/library/os.html#os.add_dll_directory
#
file(WRITE "${PYTHONHOME}/${PYTHON_STDLIB_SUBDIR}/cjyx_dll_directories.py" [==[
import os
import sys


def add(library_paths=None):
    if library_paths is None:
        library_paths = [os.path.abspath(path) for path in os.getenv("LibraryPaths", "").split(os.pathsep)]

    if sys.version_info < (3, 8) or sys.platform != 'win32':
        return

    for path in library_paths:
        if not os.path.exists(path):
            continue
        os.add_dll_directory(path)


if __name__ == "__main__":
    add()
]==])

# sitecustomize
#
# Ensures additional search paths are systmatically added when using PythonCjyx
#
# See https://docs.python.org/3/library/site.html
#
file(WRITE "${PYTHONHOME}/${PYTHON_STDLIB_SUBDIR}/sitecustomize.py" [==[
import cjyx_dll_directories

cjyx_dll_directories.add()
]==])

# PATHS
set(PYTHONLAUNCHER_PATHS_BUILD
  <APPLAUNCHER_DIR>
  )

# LIBRARY_PATHS
set(PYTHONLAUNCHER_LIBRARY_PATHS_BUILD
  ${PYTHON_SHARED_LIBRARY_DIR}
  )
if(PYTHON_ENABLE_SSL)
  list(APPEND PYTHONLAUNCHER_LIBRARY_PATHS_BUILD
    ${OPENSSL_EXPORT_LIBRARY_DIR}
    )
endif()

# ENVVARS
set(PYTHONLAUNCHER_ENVVARS_BUILD
  "PYTHONHOME=${PYTHONHOME}"
  "PYTHONNOUSERSITE=1"
  "PIP_REQUIRE_VIRTUALENV=0"
  )
if(PYTHON_ENABLE_SSL)

  set(_src ${Cjyx_SOURCE_DIR}/Base/QTCore/Resources/Certs/Cjyx.crt)
  set(_dest ${Cjyx_BINARY_DIR}/Cjyx-build/${Cjyx_SHARE_DIR}/Cjyx.crt)
  configure_file(${_src} ${_dest} COPYONLY)
  message(STATUS "Copying '${_src}' to '${_dest}'")

  list(APPEND PYTHONLAUNCHER_ENVVARS_BUILD
    "SSL_CERT_FILE=<APPLAUNCHER_DIR>/../../Cjyx-build/${Cjyx_SHARE_DIR}/Cjyx.crt"
    )
endif()

# PATH ENVVARS
set(PYTHONLAUNCHER_ADDITIONAL_PATH_ENVVARS_BUILD
  "PYTHONPATH,LibraryPaths"
  )
set(PYTHONLAUNCHER_PYTHONPATH_BUILD
  "${PYTHONHOME}/${PYTHON_STDLIB_SUBDIR}"
  "${PYTHONHOME}/${PYTHON_STDLIB_SUBDIR}/lib-dynload"
  "${PYTHONHOME}/${PYTHON_SITE_PACKAGES_SUBDIR}"
  )

#
# Settings specific to the install tree.
#
set(PYTHONHOME "<APPLAUNCHER_DIR>/../lib/Python")

# Windows:
#   Python library    -> Cjyx_BIN_DIR
#   OpenSSL libraries -> Cjyx_BIN_DIR
# Unix:
#   Python library    -> lib/Python/lib
#   OpenSSL libraries -> Cjyx_LIB_DIR

# PATHS
set(PYTHONLAUNCHER_PATHS_INSTALLED
  <APPLAUNCHER_DIR>
  )

# LIBRARY_PATHS
set(PYTHONLAUNCHER_LIBRARY_PATHS_INSTALLED
  <APPLAUNCHER_DIR>/../${Cjyx_BIN_DIR}
  <APPLAUNCHER_DIR>/../${Cjyx_LIB_DIR}
  <APPLAUNCHER_DIR>/../lib/Python/lib
  )

# ENVVARS
set(PYTHONLAUNCHER_ENVVARS_INSTALLED
  "PYTHONHOME=${PYTHONHOME}"
  "PYTHONNOUSERSITE=1"
  "PIP_REQUIRE_VIRTUALENV=0"
  )

if(PYTHON_ENABLE_SSL)
  list(APPEND PYTHONLAUNCHER_ENVVARS_INSTALLED
    "SSL_CERT_FILE=<APPLAUNCHER_DIR>/../${Cjyx_SHARE_DIR}/Cjyx.crt"
    )
endif()

# PATH ENVVARS
set(PYTHONLAUNCHER_ADDITIONAL_PATH_ENVVARS_INSTALLED
  "PYTHONPATH,LibraryPaths"
  )
set(PYTHONLAUNCHER_PYTHONPATH_INSTALLED
  "${PYTHONHOME}/lib/Python/${PYTHON_STDLIB_SUBDIR}"
  "${PYTHONHOME}/lib/Python/${PYTHON_STDLIB_SUBDIR}/lib-dynload"
  "${PYTHONHOME}/lib/Python/${PYTHON_SITE_PACKAGES_SUBDIR}"
  "<APPLAUNCHER_DIR>/../${Cjyx_BIN_DIR}/Python"
  )

#
# Notes:
#
#  * Install rules for PythonCjyxLauncherSettingsToInstall.ini and PythonCjyx executable
#  are specified in CjyxBlockInstallPython.cmake
#
#  * Since the setting "name=<launcherName>" is initially set to "PythonCjyx" based on the
#  launcher name by calling ctkAppLauncherConfigureForExecutable() below, it is
#  explicitly updated to "Cjyx" in a second step using the action "replace_application_name"
#  implemented above.
#
set(_python_launcher_config_params

  # Application properties required to lookup user revision settings
  APPLICATION_REVISION ${Cjyx_REVISION}
  ORGANIZATION_DOMAIN ${Cjyx_ORGANIZATION_DOMAIN}
  ORGANIZATION_NAME ${Cjyx_ORGANIZATION_NAME}
  USER_ADDITIONAL_SETTINGS_FILEBASENAME ${CJYX_REVISION_SPECIFIC_USER_SETTINGS_FILEBASENAME}

  SPLASHSCREEN_DISABLED

  # Additional settings exclude groups
  ADDITIONAL_SETTINGS_EXCLUDE_GROUPS "General,Application,ExtraApplicationToLaunch"

  # Additional path envars prefix
  ADDITIONAL_PATH_ENVVARS_PREFIX PYTHONLAUNCHER_

  # Launcher settings specific to build tree
  APPLICATION_EXECUTABLE ${PYTHON_REAL_EXECUTABLE}
  DESTINATION_DIR ${python_bin_dir}
  PATHS_BUILD "${PYTHONLAUNCHER_PATHS_BUILD}"
  LIBRARY_PATHS_BUILD "${PYTHONLAUNCHER_LIBRARY_PATHS_BUILD}"
  ENVVARS_BUILD "${PYTHONLAUNCHER_ENVVARS_BUILD}"
  ADDITIONAL_PATH_ENVVARS_BUILD "${PYTHONLAUNCHER_ADDITIONAL_PATH_ENVVARS_BUILD}"
  ADDITIONAL_SETTINGS_FILEPATH_BUILD "${Cjyx_BINARY_DIR}/${Cjyx_BINARY_INNER_SUBDIR}/${Cjyx_MAIN_PROJECT_APPLICATION_NAME}LauncherSettings.ini"

  # Launcher settings specific to install tree
  APPLICATION_INSTALL_EXECUTABLE_NAME python-real${CMAKE_EXECUTABLE_SUFFIX}
  APPLICATION_INSTALL_SUBDIR "."
  PATHS_INSTALLED "${PYTHONLAUNCHER_PATHS_INSTALLED}"
  LIBRARY_PATHS_INSTALLED "${PYTHONLAUNCHER_LIBRARY_PATHS_INSTALLED}"
  ENVVARS_INSTALLED "${PYTHONLAUNCHER_ENVVARS_INSTALLED}"
  ADDITIONAL_PATH_ENVVARS_INSTALLED "${PYTHONLAUNCHER_ADDITIONAL_PATH_ENVVARS_INSTALLED}"
  ADDITIONAL_SETTINGS_FILEPATH_INSTALLED "<APPLAUNCHER_SETTINGS_DIR>/${Cjyx_MAIN_PROJECT_APPLICATION_NAME}LauncherSettings.ini"
  )

# Custom Python executable name must start with Python for compatibility with
# development tools, such as PyCharm.
ctkAppLauncherConfigureForExecutable(
  APPLICATION_NAME PythonCjyx
  ${_python_launcher_config_params}
  )
