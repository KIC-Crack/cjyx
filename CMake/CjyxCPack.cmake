
# -------------------------------------------------------------------------
# Disable source generator enabled by default
# -------------------------------------------------------------------------
set(CPACK_SOURCE_TBZ2 OFF CACHE BOOL "Enable to build TBZ2 source packages" FORCE)
set(CPACK_SOURCE_TGZ  OFF CACHE BOOL "Enable to build TGZ source packages" FORCE)
set(CPACK_SOURCE_TZ   OFF CACHE BOOL "Enable to build TZ source packages" FORCE)

# -------------------------------------------------------------------------
# Select generator
# -------------------------------------------------------------------------
if(UNIX)
  set(CPACK_GENERATOR "TGZ")
  if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
  endif()
elseif(WIN32)
  set(CPACK_GENERATOR "NSIS")
endif()

# -------------------------------------------------------------------------
# Install standalone executable and plugins
# -------------------------------------------------------------------------

if(Cjyx_USE_PYTHONQT AND NOT Cjyx_USE_SYSTEM_python)
  # Python install rules are common to both 'bundled' and 'regular' package
  include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallPython.cmake)
endif()

if(NOT Cjyx_USE_SYSTEM_QT)
  set(CjyxBlockInstallQtPlugins_subdirectories
    audio
    imageformats
    printsupport
    sqldrivers
    )
    if(Cjyx_BUILD_WEBENGINE_SUPPORT)
      list(APPEND CjyxBlockInstallQtPlugins_subdirectories
        designer:webengineview
        )
    endif()
    if(APPLE)
      list(APPEND CjyxBlockInstallQtPlugins_subdirectories
        platforms:cocoa
        )
    elseif(UNIX)
      list(APPEND CjyxBlockInstallQtPlugins_subdirectories
        platforms:xcb
        xcbglintegrations:xcb-glx-integration
        )
    elseif(WIN32)
      list(APPEND CjyxBlockInstallQtPlugins_subdirectories
        platforms:windows
        )
    endif()
  include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallQtPlugins.cmake)
endif()

if(Cjyx_BUILD_DICOM_SUPPORT AND NOT Cjyx_USE_SYSTEM_DCMTK)
  include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallDCMTKApps.cmake)
endif()

# Install Qt designer launcher
if(Cjyx_BUILD_QT_DESIGNER_PLUGINS)
  set(executablename "CjyxDesigner")
  set(build_designer_executable "${QT_BINARY_DIR}/designer${CMAKE_EXECUTABLE_SUFFIX}")
  if(APPLE)
    set(build_designer_executable "${QT_BINARY_DIR}/Designer.app/Contents/MacOS/designer")
  endif()
  set(installed_designer_executable "designer-real${CMAKE_EXECUTABLE_SUFFIX}")
  set(installed_designer_subdir ".")
  if(APPLE)
    set(installed_designer_executable "Designer")
    set(installed_designer_subdir "Designer.app/Contents/MacOS")
  endif()
  # Ensure directory exists at configuration time
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${Cjyx_BIN_DIR})
  # Configure designer launcher
  find_package(CTKAppLauncher REQUIRED)
  ctkAppLauncherConfigureForExecutable(
    APPLICATION_NAME ${executablename}
    SPLASHSCREEN_DISABLED
    # Additional settings exclude groups
    ADDITIONAL_SETTINGS_EXCLUDE_GROUPS "General,Application,ExtraApplicationToLaunch"
    # Launcher settings specific to build tree
    APPLICATION_EXECUTABLE ${build_designer_executable}
    DESTINATION_DIR ${CMAKE_BINARY_DIR}/${Cjyx_BIN_DIR}
    ADDITIONAL_SETTINGS_FILEPATH_BUILD "${Cjyx_BINARY_DIR}/${Cjyx_BINARY_INNER_SUBDIR}/CjyxLauncherSettings.ini"
    # Launcher settings specific to install tree
    APPLICATION_INSTALL_EXECUTABLE_NAME "${installed_designer_executable}"
    APPLICATION_INSTALL_SUBDIR "${installed_designer_subdir}"
    ADDITIONAL_SETTINGS_FILEPATH_INSTALLED "<APPLAUNCHER_SETTINGS_DIR>/CjyxLauncherSettings.ini"
    )
  # Install designer launcher settings
  install(
    FILES ${CMAKE_BINARY_DIR}/${Cjyx_BIN_DIR}/${executablename}LauncherSettingsToInstall.ini
    DESTINATION ${Cjyx_INSTALL_BIN_DIR}
    RENAME ${executablename}LauncherSettings.ini
    COMPONENT Runtime
    )
  # Install designer launcher
  set(_launcher CTKAppLauncher)
  if(Cjyx_BUILD_WIN32_CONSOLE)
    set(_launcher CTKAppLauncherW)
  endif()
  install(
    PROGRAMS ${CTKAppLauncher_DIR}/bin/${_launcher}${CMAKE_EXECUTABLE_SUFFIX}
    DESTINATION ${Cjyx_INSTALL_BIN_DIR}
    RENAME ${executablename}${CMAKE_EXECUTABLE_SUFFIX}
    COMPONENT Runtime
    )
endif()

# -------------------------------------------------------------------------
# Update CPACK_INSTALL_CMAKE_PROJECTS
# -------------------------------------------------------------------------
set(CPACK_INSTALL_CMAKE_PROJECTS)

# Ensure external project associated with bundled extensions are packaged
foreach(extension_name ${Cjyx_BUNDLED_EXTENSION_NAMES})
  if(DEFINED "${extension_name}_CPACK_INSTALL_CMAKE_PROJECTS")
    set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${${extension_name}_CPACK_INSTALL_CMAKE_PROJECTS}")
  endif()
endforeach()

# Install CTK Apps and Plugins (PythonQt modules, QtDesigner plugins ...)
if(NOT "${CTK_DIR}" STREQUAL "" AND EXISTS "${CTK_DIR}/CTK-build/CMakeCache.txt")
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CTK_DIR}/CTK-build;CTK;RuntimeApplications;/")
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CTK_DIR}/CTK-build;CTK;RuntimePlugins;/")
endif()

if(NOT APPLE)
  if(NOT Cjyx_USE_SYSTEM_QT)
    include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallQt.cmake)
  endif()
  if(Cjyx_BUILD_DICOM_SUPPORT AND NOT Cjyx_USE_SYSTEM_DCMTK)
    include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallDCMTKLibs.cmake)
  endif()
  if(Cjyx_USE_PYTHONQT AND NOT Cjyx_USE_SYSTEM_CTK)
    include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallPythonQt.cmake)
  endif()
  if(Cjyx_USE_PYTHONQT_WITH_OPENSSL AND NOT Cjyx_USE_SYSTEM_OpenSSL)
    include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallOpenSSL.cmake)
  endif()
  if(Cjyx_USE_TBB AND NOT Cjyx_USE_SYSTEM_TBB)
    include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallTBB.cmake)
  endif()
  if(Cjyx_USE_QtTesting AND NOT Cjyx_USE_SYSTEM_CTK)
    include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallQtTesting.cmake)
  endif()
  if(NOT Cjyx_USE_SYSTEM_LibArchive)
    include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallLibArchive.cmake)
  endif()
  # XXX Note that installation of OpenMP libraries is available only
  #     when using msvc compiler.
  if(NOT DEFINED CMAKE_INSTALL_OPENMP_LIBRARIES)
    set(CMAKE_INSTALL_OPENMP_LIBRARIES ON)
  endif()
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT "RuntimeLibraries")
  include(InstallRequiredSystemLibraries)

  include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallCMakeProjects.cmake)

else()

  #------------------------------------------------------------------------------
  # macOS specific configuration used by the "fix-up" script
  #------------------------------------------------------------------------------
  set(CMAKE_INSTALL_NAME_TOOL "" CACHE FILEPATH "" FORCE)

  if(Cjyx_USE_PYTHONQT)
    include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallExternalPythonModules.cmake)
  endif()

  # Calling find_package will ensure the *_LIBRARY_DIRS expected by the fixup script are set.
  if(Cjyx_BUILD_CLI_SUPPORT)
    find_package(CjyxExecutionModel REQUIRED)
  endif()
  set(VTK_LIBRARY_DIRS "${VTK_DIR}/lib")

  # Get Qt root directory
  get_property(_filepath TARGET "Qt5::Core" PROPERTY LOCATION_RELEASE)
  get_filename_component(_dir ${_filepath} PATH)
  set(qt_root_dir "${_dir}/..")

  #------------------------------------------------------------------------------
  # <ExtensionName>_FIXUP_BUNDLE_LIBRARY_DIRECTORIES
  #------------------------------------------------------------------------------

  #
  # Setting this variable in the CMakeLists.txt of an extension allows to update
  # the list of directories used by the "fix-up" script to look up libraries
  # that should be copied into the Cjyx package when the extension is bundled.
  #
  # To ensure the extension can be bundled, the variable should be set as a CACHE
  # variable.
  #

  set(EXTENSION_BUNDLE_FIXUP_LIBRARY_DIRECTORIES)
  foreach(project ${Cjyx_BUNDLED_EXTENSION_NAMES})
    if(DEFINED ${project}_FIXUP_BUNDLE_LIBRARY_DIRECTORIES)
      # Exclude system directories.
      foreach(lib_path IN LISTS ${project}_FIXUP_BUNDLE_LIBRARY_DIRECTORIES)
        if(lib_path MATCHES "^(/lib|/lib32|/libx32|/lib64|/usr/lib|/usr/lib32|/usr/libx32|/usr/lib64|/usr/X11R6|/usr/bin)"
            OR lib_path MATCHES "^(/System/Library|/usr/lib)")
          continue()
        endif()
        list(APPEND EXTENSION_BUNDLE_FIXUP_LIBRARY_DIRECTORIES ${lib_path})
      endforeach()
    endif()
  endforeach()

  #------------------------------------------------------------------------------
  # Configure "fix-up" script
  #------------------------------------------------------------------------------
  set(fixup_path @rpath)
  set(cjyx_cpack_bundle_fixup_directory ${Cjyx_BINARY_DIR}/CMake/CjyxCPackBundleFixup)
  configure_file(
    "${Cjyx_SOURCE_DIR}/CMake/CjyxCPackBundleFixup.cmake.in"
    "${cjyx_cpack_bundle_fixup_directory}/CjyxCPackBundleFixup.cmake"
    @ONLY)
  # HACK - For a given directory, "install(SCRIPT ...)" rule will be evaluated first,
  #        let's make sure the following install rule is evaluated within its own directory.
  #        Otherwise, the associated script will be executed before any other relevant install rules.
  file(WRITE ${cjyx_cpack_bundle_fixup_directory}/CMakeLists.txt
    "install(SCRIPT \"${cjyx_cpack_bundle_fixup_directory}/CjyxCPackBundleFixup.cmake\" COMPONENT Runtime)")
  add_subdirectory(${cjyx_cpack_bundle_fixup_directory} ${cjyx_cpack_bundle_fixup_directory}-binary)

endif()

include(${Cjyx_CMAKE_DIR}/CjyxBlockInstallExtensionPackages.cmake)

# -------------------------------------------------------------------------
# Update CPACK_INSTALL_CMAKE_PROJECTS
# -------------------------------------------------------------------------

# Install additional projects if any, but also do a find_package to load CPACK
# variables of the Cjyx_MAIN_PROJECT if different from CjyxApp
set(additional_projects ${Cjyx_ADDITIONAL_DEPENDENCIES} ${Cjyx_ADDITIONAL_PROJECTS})
foreach(additional_project ${additional_projects})
  if(NOT Cjyx_USE_SYSTEM_${additional_project})
    find_package(${additional_project} QUIET)
    if(${additional_project}_FOUND)
      if(${additional_project}_USE_FILE)
        include(${${additional_project}_USE_FILE})
      endif()
      if(NOT APPLE)
        if(DEFINED ${additional_project}_CPACK_INSTALL_CMAKE_PROJECTS)
          set(CPACK_INSTALL_CMAKE_PROJECTS
            "${CPACK_INSTALL_CMAKE_PROJECTS};${${additional_project}_CPACK_INSTALL_CMAKE_PROJECTS}")
        endif()
      endif()
    endif()
  endif()
endforeach()

# Install Cjyx
set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${Cjyx_BINARY_DIR};Cjyx;RuntimeLibraries;/")
set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${Cjyx_BINARY_DIR};Cjyx;RuntimePlugins;/")
if(NOT Cjyx_INSTALL_NO_DEVELOPMENT)
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${Cjyx_BINARY_DIR};Cjyx;Development;/")
endif()

# Installation of 'Runtime' should be last to ensure the 'CjyxCPackBundleFixup.cmake' is executed last.
set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${Cjyx_BINARY_DIR};Cjyx;Runtime;/")

# -------------------------------------------------------------------------
# Define helper macros and functions
# -------------------------------------------------------------------------
function(cjyx_verbose_set varname)
  message(STATUS "Setting ${varname} to '${ARGN}'")
  set(${varname} "${ARGN}" PARENT_SCOPE)
endfunction()

# Convenience variable used below: This is the name of the application (e.g Cjyx)
# whereas Cjyx_MAIN_PROJECT is the application project name (e.g CjyxApp, AwesomeApp, ...)
set(app_name ${${Cjyx_MAIN_PROJECT}_APPLICATION_NAME})

macro(cjyx_cpack_set varname)
  if(DEFINED ${app_name}_${varname})
    cjyx_verbose_set(${varname} ${${app_name}_${varname}})
  elseif(DEFINED Cjyx_${varname})
    cjyx_verbose_set(${varname} ${Cjyx_${varname}})
  else()
    if(NOT "Cjyx" STREQUAL "${app_name}")
      set(_error_msg "Neither Cjyx_${varname} or ${app_name}_${varname} are defined.")
    else()
      set(_error_msg "${app_name}_${varname} is not defined")
    endif()
    message(FATAL_ERROR "Failed to set variable ${varname}. ${_error_msg}")
  endif()
endmacro()

# -------------------------------------------------------------------------
# Common package properties
# -------------------------------------------------------------------------
set(CPACK_MONOLITHIC_INSTALL ON)
if(UNIX AND NOT APPLE AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  # Reduce package size stripping symbols from the regular symbol table
  # for ELF libraries and executables.
  # See also use of cjyxInstallLibrary() and cjyxStripInstalledLibrary() functions
  set(CPACK_STRIP_FILES 1)
endif()

set(${app_name}_CPACK_PACKAGE_NAME ${app_name})
cjyx_cpack_set("CPACK_PACKAGE_NAME")

set(Cjyx_CPACK_PACKAGE_VENDOR ${Cjyx_ORGANIZATION_NAME})
cjyx_cpack_set("CPACK_PACKAGE_VENDOR")

set(Cjyx_CPACK_PACKAGE_VERSION_MAJOR "${Cjyx_VERSION_MAJOR}")
set(${app_name}_CPACK_PACKAGE_VERSION_MAJOR ${Cjyx_MAIN_PROJECT_VERSION_MAJOR})
cjyx_cpack_set("CPACK_PACKAGE_VERSION_MAJOR")

set(Cjyx_CPACK_PACKAGE_VERSION_MINOR "${Cjyx_VERSION_MINOR}")
set(${app_name}_CPACK_PACKAGE_VERSION_MINOR ${Cjyx_MAIN_PROJECT_VERSION_MINOR})
cjyx_cpack_set("CPACK_PACKAGE_VERSION_MINOR")

set(Cjyx_CPACK_PACKAGE_VERSION_PATCH "${Cjyx_VERSION_PATCH}")
set(${app_name}_CPACK_PACKAGE_VERSION_PATCH ${Cjyx_MAIN_PROJECT_VERSION_PATCH})
cjyx_cpack_set("CPACK_PACKAGE_VERSION_PATCH")

set(Cjyx_CPACK_PACKAGE_VERSION "${Cjyx_VERSION_FULL}")
set(${app_name}_CPACK_PACKAGE_VERSION ${Cjyx_MAIN_PROJECT_VERSION_FULL})
cjyx_cpack_set("CPACK_PACKAGE_VERSION")

set(CPACK_SYSTEM_NAME "${Cjyx_OS}-${Cjyx_ARCHITECTURE}")

set(Cjyx_CPACK_PACKAGE_INSTALL_DIRECTORY "${${app_name}_CPACK_PACKAGE_NAME} ${CPACK_PACKAGE_VERSION}")
cjyx_cpack_set("CPACK_PACKAGE_INSTALL_DIRECTORY")

get_property(${app_name}_CPACK_PACKAGE_DESCRIPTION_FILE GLOBAL PROPERTY ${app_name}_DESCRIPTION_FILE)
cjyx_cpack_set("CPACK_PACKAGE_DESCRIPTION_FILE")

get_property(${app_name}_CPACK_RESOURCE_FILE_LICENSE GLOBAL PROPERTY ${app_name}_LICENSE_FILE)
cjyx_cpack_set("CPACK_RESOURCE_FILE_LICENSE")

get_property(${app_name}_CPACK_PACKAGE_DESCRIPTION_SUMMARY GLOBAL PROPERTY ${app_name}_DESCRIPTION_SUMMARY)
cjyx_cpack_set("CPACK_PACKAGE_DESCRIPTION_SUMMARY")

get_property(${app_name}_CPACK_PACKAGE_ICON GLOBAL PROPERTY ${app_name}_APPLE_ICON_FILE)
if(APPLE)
  cjyx_cpack_set("CPACK_PACKAGE_ICON")
endif()

# -------------------------------------------------------------------------
# NSIS package properties
# -------------------------------------------------------------------------
if(CPACK_GENERATOR STREQUAL "NSIS")

  set(Cjyx_CPACK_NSIS_INSTALL_SUBDIRECTORY "")
  cjyx_cpack_set("CPACK_NSIS_INSTALL_SUBDIRECTORY")

  set(_nsis_install_root "${Cjyx_CPACK_NSIS_INSTALL_ROOT}")

  # Use ManifestDPIAware to improve appearance of installer
  string(APPEND CPACK_NSIS_DEFINES "\n  ;Use ManifestDPIAware to improve appearance of installer")
  string(APPEND CPACK_NSIS_DEFINES "\n  ManifestDPIAware true\n")

  if (NOT Cjyx_CPACK_NSIS_INSTALL_REQUIRES_ADMIN_ACCOUNT)
    # Install as regular user (UAC dialog will not be shown).
    string(APPEND CPACK_NSIS_DEFINES "\n  ;Install as regular user (UAC dialog will not be shown).")
    string(APPEND CPACK_NSIS_DEFINES "\n  RequestExecutionLevel user")
  endif()

  # Installers for 32- vs. 64-bit CMake:
  #  - "NSIS package/display name" (text used in the installer GUI)
  #  - Registry key used to store info about the installation

  if(CMAKE_CL_64)
    cjyx_verbose_set(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    cjyx_verbose_set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_INSTALL_DIRECTORY} (Win64)")
  else()
    cjyx_verbose_set(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} (Win32)")
    cjyx_verbose_set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
  endif()

  if(NOT CPACK_NSIS_INSTALL_SUBDIRECTORY STREQUAL "")
    set(_nsis_install_root "${_nsis_install_root}/${CPACK_NSIS_INSTALL_SUBDIRECTORY}")
  endif()
  string(REPLACE "/" "\\\\" _nsis_install_root "${_nsis_install_root}")
  cjyx_verbose_set(CPACK_NSIS_INSTALL_ROOT ${_nsis_install_root})

  # Cjyx does *NOT* require setting the windows path
  set(CPACK_NSIS_MODIFY_PATH OFF)

  set(APPLICATION_NAME "${Cjyx_MAIN_PROJECT_APPLICATION_NAME}")
  set(EXECUTABLE_NAME "${Cjyx_MAIN_PROJECT_APPLICATION_NAME}")
  # Set application name used to create Start Menu shortcuts
  set(PACKAGE_APPLICATION_NAME "${APPLICATION_NAME} ${CPACK_PACKAGE_VERSION}")
  cjyx_verbose_set(CPACK_PACKAGE_EXECUTABLES "..\\\\${EXECUTABLE_NAME}" "${PACKAGE_APPLICATION_NAME}")

  get_property(${app_name}_CPACK_NSIS_MUI_ICON GLOBAL PROPERTY ${app_name}_WIN_ICON_FILE)
  cjyx_cpack_set("CPACK_NSIS_MUI_ICON")
  cjyx_verbose_set(CPACK_NSIS_INSTALLED_ICON_NAME "${app_name}.exe")

  # -------------------------------------------------------------------------
  # File extensions
  # -------------------------------------------------------------------------
  set(FILE_EXTENSIONS .mrml .xcat .mrb)

  if(FILE_EXTENSIONS)

    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS)
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS)
    foreach(ext ${FILE_EXTENSIONS})
      string(LENGTH "${ext}" len)
      math(EXPR len_m1 "${len} - 1")
      string(SUBSTRING "${ext}" 1 ${len_m1} ext_wo_dot)
      set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
        "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
WriteRegStr SHCTX \\\"SOFTWARE\\\\Classes\\\\${APPLICATION_NAME}\\\" \\\"\\\" \\\"${APPLICATION_NAME} supported file\\\"
WriteRegStr SHCTX \\\"SOFTWARE\\\\Classes\\\\${APPLICATION_NAME}\\\" \\\"URL Protocol\\\" \\\"\\\"
WriteRegStr SHCTX \\\"SOFTWARE\\\\Classes\\\\${APPLICATION_NAME}\\\\shell\\\\open\\\\command\\\" \
\\\"\\\" \\\"$\\\\\\\"$INSTDIR\\\\${EXECUTABLE_NAME}.exe$\\\\\\\" $\\\\\\\"%1$\\\\\\\"\\\"
WriteRegStr SHCTX \\\"SOFTWARE\\\\Classes\\\\${ext}\\\" \\\"\\\" \\\"${APPLICATION_NAME}\\\"
WriteRegStr SHCTX \\\"SOFTWARE\\\\Classes\\\\${ext}\\\" \\\"Content Type\\\" \\\"application/x-${ext_wo_dot}\\\"
")
      set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
DeleteRegKey SHCTX \\\"SOFTWARE\\\\Classes\\\\${APPLICATION_NAME}\\\"
DeleteRegKey SHCTX \\\"SOFTWARE\\\\Classes\\\\${ext}\\\"
")
    endforeach()
  endif()

endif()

include(CPack)

