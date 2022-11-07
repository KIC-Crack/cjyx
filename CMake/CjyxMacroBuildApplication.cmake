################################################################################
#
#  Program: 3D Cjyx
#
#  Copyright (c) Kitware Inc.
#
#  See COPYRIGHT.txt
#  or http://www.slicer.org/copyright/copyright.txt for details.
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#  This file was originally developed by Jean-Christophe Fillion-Robin and Johan Andruejol, Kitware Inc.
#  and was partially funded by NIH grant 3P41RR013218-12S1
#
################################################################################


macro(cjyxMacroBuildAppLibrary)
  set(options
    WRAP_PYTHONQT
    )
  set(oneValueArgs
    NAME
    EXPORT_DIRECTIVE
    FOLDER
    APPLICATION_NAME
    DESCRIPTION_SUMMARY
    DESCRIPTION_FILE
    )
  set(multiValueArgs
    SRCS
    MOC_SRCS
    UI_SRCS
    RESOURCES
    INCLUDE_DIRECTORIES
    TARGET_LIBRARIES
    )
  cmake_parse_arguments(CJYXAPPLIB
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
    )

  # --------------------------------------------------------------------------
  # Sanity checks
  # --------------------------------------------------------------------------
  if(CJYXAPPLIB_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Unknown keywords given to CjyxMacroBuildAppLibrary(): \"${CJYXAPPLIB_UNPARSED_ARGUMENTS}\"")
  endif()

  set(expected_defined_vars NAME EXPORT_DIRECTIVE DESCRIPTION_SUMMARY)
  foreach(var ${expected_defined_vars})
    if(NOT DEFINED CJYXAPPLIB_${var})
      message(FATAL_ERROR "${var} is mandatory")
    endif()
  endforeach()

  set(expected_existing_vars DESCRIPTION_FILE)
  foreach(var ${expected_existing_vars})
    if(NOT EXISTS "${CJYXAPPLIB_${var}}")
      message(FATAL_ERROR "error: Variable ${var} set to ${CJYXAPPLIB_${var}} corresponds to an nonexistent file. ")
    endif()
  endforeach()

  if(NOT DEFINED Cjyx_INSTALL_NO_DEVELOPMENT)
    message(SEND_ERROR "Cjyx_INSTALL_NO_DEVELOPMENT is mandatory")
  endif()

  if(NOT DEFINED CJYXAPPLIB_APPLICATION_NAME)
    set(CJYXAPPLIB_APPLICATION_NAME ${CJYXAPPLIB_NAME})
  endif()

  message(STATUS "--------------------------------------------------")
  message(STATUS "Configuring ${CJYXAPPLIB_APPLICATION_NAME} application library: ${CJYXAPPLIB_NAME}")
  message(STATUS "--------------------------------------------------")

  macro(_set_applib_property varname)
    set_property(GLOBAL PROPERTY ${CJYXAPPLIB_APPLICATION_NAME}_${varname} ${CJYXAPPLIB_${varname}})
    message(STATUS "Setting ${CJYXAPPLIB_APPLICATION_NAME} ${varname} to '${CJYXAPPLIB_${varname}}'")
  endmacro()

  _set_applib_property(DESCRIPTION_SUMMARY)
  _set_applib_property(DESCRIPTION_FILE)

  # --------------------------------------------------------------------------
  # Define library name
  # --------------------------------------------------------------------------
  set(lib_name ${CJYXAPPLIB_NAME})

  # --------------------------------------------------------------------------
  # Folder
  # --------------------------------------------------------------------------
  if(NOT DEFINED CJYXAPPLIB_FOLDER)
    set(CJYXAPPLIB_FOLDER "App-${CJYXAPPLIB_APPLICATION_NAME}")
  endif()

  # --------------------------------------------------------------------------
  # Include dirs
  # --------------------------------------------------------------------------

  set(include_dirs
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Cjyx_Base_INCLUDE_DIRS}
    ${MRMLCore_INCLUDE_DIRS}
    ${MRMLLogic_INCLUDE_DIRS}
    ${qMRMLWidgets_INCLUDE_DIRS}
    ${qCjyxModulesCore_SOURCE_DIR}
    ${qCjyxModulesCore_BINARY_DIR}
    ${ITKFactoryRegistration_INCLUDE_DIRS}
    ${CJYXAPPLIB_INCLUDE_DIRECTORIES}
    )

  include_directories(${include_dirs})

  #-----------------------------------------------------------------------------
  # Update Cjyx_Base_INCLUDE_DIRS
  #-----------------------------------------------------------------------------

  # NA

  #-----------------------------------------------------------------------------
  # Configure
  # --------------------------------------------------------------------------
  set(MY_LIBRARY_EXPORT_DIRECTIVE ${CJYXAPPLIB_EXPORT_DIRECTIVE})
  set(MY_EXPORT_HEADER_PREFIX ${CJYXAPPLIB_NAME})
  set(MY_LIBNAME ${lib_name})

  configure_file(
    ${Cjyx_SOURCE_DIR}/CMake/qCjyxExport.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/${MY_EXPORT_HEADER_PREFIX}Export.h
    )
  set(dynamicHeaders
    "${dynamicHeaders};${CMAKE_CURRENT_BINARY_DIR}/${MY_EXPORT_HEADER_PREFIX}Export.h")

  #-----------------------------------------------------------------------------
  # Sources
  # --------------------------------------------------------------------------
    set(_moc_options OPTIONS -DCjyx_HAVE_QT5)
    QT5_WRAP_CPP(CJYXAPPLIB_MOC_OUTPUT ${CJYXAPPLIB_MOC_SRCS} ${_moc_options})
    QT5_WRAP_UI(CJYXAPPLIB_UI_CXX ${CJYXAPPLIB_UI_SRCS})
    if(DEFINED CJYXAPPLIB_RESOURCES)
      QT5_ADD_RESOURCES(CJYXAPPLIB_QRC_SRCS ${CJYXAPPLIB_RESOURCES})
    endif()

  set_source_files_properties(
    ${CJYXAPPLIB_UI_CXX}
    ${CJYXAPPLIB_MOC_OUTPUT}
    ${CJYXAPPLIB_QRC_SRCS}
    WRAP_EXCLUDE
    )

  # --------------------------------------------------------------------------
  # Source groups
  # --------------------------------------------------------------------------
  source_group("Resources" FILES
    ${CJYXAPPLIB_UI_SRCS}
    ${Cjyx_SOURCE_DIR}/Resources/qCjyx.qrc
    ${CJYXAPPLIB_RESOURCES}
  )

  source_group("Generated" FILES
    ${CJYXAPPLIB_UI_CXX}
    ${CJYXAPPLIB_MOC_OUTPUT}
    ${CJYXAPPLIB_QRC_SRCS}
    ${dynamicHeaders}
  )

  # --------------------------------------------------------------------------
  # Translation
  # --------------------------------------------------------------------------
  if(Cjyx_BUILD_I18N_SUPPORT)
    set(TS_DIR
      "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Translations/"
    )

    include(CjyxMacroTranslation)
    CjyxMacroTranslation(
      SRCS ${CJYXAPPLIB_SRCS}
      UI_SRCS ${CJYXAPPLIB_UI_SRCS}
      TS_DIR ${TS_DIR}
      TS_BASEFILENAME ${CJYXAPPLIB_NAME}
      TS_LANGUAGES ${Cjyx_LANGUAGES}
      QM_OUTPUT_DIR_VAR QM_OUTPUT_DIR
      QM_OUTPUT_FILES_VAR QM_OUTPUT_FILES
      )

    set_property(GLOBAL APPEND PROPERTY Cjyx_QM_OUTPUT_DIRS ${QM_OUTPUT_DIR})
  else()
    set(QM_OUTPUT_FILES )
  endif()

  # --------------------------------------------------------------------------
  # Build the library
  # --------------------------------------------------------------------------
  add_library(${lib_name}
    ${CJYXAPPLIB_SRCS}
    ${CJYXAPPLIB_MOC_OUTPUT}
    ${CJYXAPPLIB_UI_CXX}
    ${CJYXAPPLIB_QRC_SRCS}
    ${QM_OUTPUT_FILES}
    )
  set_target_properties(${lib_name} PROPERTIES LABELS ${lib_name})

  # Apply user-defined properties to the library target.
  if(Cjyx_LIBRARY_PROPERTIES)
    set_target_properties(${lib_name} PROPERTIES ${Cjyx_LIBRARY_PROPERTIES})
  endif()

  target_link_libraries(${lib_name}
    qCjyxBaseQTApp
    ${CJYXAPPLIB_TARGET_LIBRARIES}
    )

  # Folder
  set_target_properties(${lib_name} PROPERTIES FOLDER ${CJYXAPPLIB_FOLDER})

  #-----------------------------------------------------------------------------
  # Install library
  #-----------------------------------------------------------------------------
  install(TARGETS ${lib_name}
    RUNTIME DESTINATION ${Cjyx_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${Cjyx_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${Cjyx_INSTALL_LIB_DIR} COMPONENT Development
  )

  # --------------------------------------------------------------------------
  # Install headers
  # --------------------------------------------------------------------------
  if(NOT Cjyx_INSTALL_NO_DEVELOPMENT)
    # Install headers
    file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
    install(FILES
      ${headers}
      ${dynamicHeaders}
      DESTINATION ${Cjyx_INSTALL_INCLUDE_DIR}/${lib_name} COMPONENT Development
      )
  endif()

  # --------------------------------------------------------------------------
  # PythonQt wrapping
  # --------------------------------------------------------------------------
  if(Cjyx_USE_PYTHONQT AND CJYXAPPLIB_WRAP_PYTHONQT)
    ctkMacroBuildLibWrapper(
      NAMESPACE "osa" # Use "osa" instead of "org.cjyx.app" to avoid build error on windows
      TARGET ${lib_name}
      SRCS "${CJYXAPPLIB_SRCS}"
      INSTALL_BIN_DIR ${Cjyx_INSTALL_BIN_DIR}
      INSTALL_LIB_DIR ${Cjyx_INSTALL_LIB_DIR}
      )
    set_target_properties(${lib_name}PythonQt PROPERTIES FOLDER ${CJYXAPPLIB_FOLDER})
  endif()

  # --------------------------------------------------------------------------
  # Export target
  # --------------------------------------------------------------------------
  set_property(GLOBAL APPEND PROPERTY Cjyx_TARGETS ${CJYXAPPLIB_NAME})

endmacro()


#
# cjyxMacroBuildApplication
#

macro(cjyxMacroBuildApplication)
  set(options
    CONFIGURE_LAUNCHER
    )
  set(oneValueArgs
    NAME
    FOLDER
    APPLICATION_NAME

    DEFAULT_SETTINGS_FILE
    SPLASHSCREEN_ENABLED
    LAUNCHER_SPLASHSCREEN_FILE
    APPLE_ICON_FILE
    WIN_ICON_FILE
    LICENSE_FILE

    TARGET_NAME_VAR

    APPLICATION_DEFAULT_ARGUMENTS # space separated list
    )
  set(multiValueArgs
    SRCS
    INCLUDE_DIRECTORIES
    TARGET_LIBRARIES
    )
  cmake_parse_arguments(CJYXAPP
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
    )

  # --------------------------------------------------------------------------
  # Sanity checks
  # --------------------------------------------------------------------------
  if(CJYXAPP_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Unknown keywords given to cjyxMacroBuildApplication(): \"${CJYXAPP_UNPARSED_ARGUMENTS}\"")
  endif()

  # Set defaults
  if(NOT DEFINED CJYXAPP_SPLASHSCREEN_ENABLED)
    set(CJYXAPP_SPLASHSCREEN_ENABLED TRUE)
  endif()

  # Check expected variables
  set(expected_defined_vars
    NAME
    APPLE_ICON_FILE
    WIN_ICON_FILE
    LICENSE_FILE
    )
  if(CJYXAPP_SPLASHSCREEN_ENABLED)
    list(APPEND expected_defined_vars
      LAUNCHER_SPLASHSCREEN_FILE
      )
  endif()
  foreach(var ${expected_defined_vars})
    if(NOT DEFINED CJYXAPP_${var})
      message(FATAL_ERROR "${var} is mandatory")
    endif()
  endforeach()

  # Set defaults
  if(NOT DEFINED CJYXAPP_APPLICATION_NAME)
    string(REGEX REPLACE "(.+)App" "\\1" CJYXAPP_APPLICATION_NAME ${CJYXAPP_NAME})
  endif()

  message(STATUS "--------------------------------------------------")
  message(STATUS "Configuring ${CJYXAPP_APPLICATION_NAME} application: ${CJYXAPP_NAME}")
  message(STATUS "--------------------------------------------------")

  macro(_set_app_property varname)
    set_property(GLOBAL PROPERTY ${CJYXAPP_APPLICATION_NAME}_${varname} ${CJYXAPP_${varname}})
    message(STATUS "Setting ${CJYXAPP_APPLICATION_NAME} ${varname} to '${CJYXAPP_${varname}}'")
  endmacro()

  _set_app_property("APPLICATION_NAME")

  macro(_set_path_var varname)
    if(NOT IS_ABSOLUTE ${CJYXAPP_${varname}})
      set(CJYXAPP_${varname} ${CMAKE_CURRENT_SOURCE_DIR}/${CJYXAPP_${varname}})
    endif()
    if(NOT EXISTS "${CJYXAPP_${varname}}")
      message(FATAL_ERROR "error: Variable ${varname} set to ${CJYXAPP_${varname}} corresponds to an nonexistent file. ")
    endif()
    _set_app_property(${varname})
  endmacro()

  if(CJYXAPP_SPLASHSCREEN_ENABLED)
    _set_path_var(LAUNCHER_SPLASHSCREEN_FILE)
  endif()
  _set_path_var(APPLE_ICON_FILE)
  _set_path_var(WIN_ICON_FILE)
  _set_path_var(LICENSE_FILE)
  if(DEFINED CJYXAPP_DEFAULT_SETTINGS_FILE)
    _set_path_var(DEFAULT_SETTINGS_FILE)
  endif()

  # --------------------------------------------------------------------------
  # Folder
  # --------------------------------------------------------------------------
  if(NOT DEFINED CJYXAPP_FOLDER)
    set(CJYXAPP_FOLDER "App-${CJYXAPP_APPLICATION_NAME}")
  endif()

  # --------------------------------------------------------------------------
  # Configure Application Bundle Resources (Mac Only)
  # --------------------------------------------------------------------------

  if(APPLE)
    set(apple_bundle_sources ${CJYXAPP_APPLE_ICON_FILE})
    set_source_files_properties(
      "${apple_bundle_sources}"
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
      )
    get_filename_component(apple_icon_filename ${CJYXAPP_APPLE_ICON_FILE} NAME)
    set(MACOSX_BUNDLE_ICON_FILE ${apple_icon_filename})
    message(STATUS "Setting MACOSX_BUNDLE_ICON_FILE to '${MACOSX_BUNDLE_ICON_FILE}'")
  endif()

  # --------------------------------------------------------------------------
  # Include dirs
  # --------------------------------------------------------------------------

  set(include_dirs
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CJYXAPP_INCLUDE_DIRECTORIES}
    )

  include_directories(${include_dirs})

  # --------------------------------------------------------------------------
  # Build the executable
  # --------------------------------------------------------------------------
  set(Cjyx_HAS_CONSOLE_IO_SUPPORT TRUE)  # CjyxApp-real is a console application
  set(Cjyx_HAS_CONSOLE_LAUNCHER TRUE)    # Cjyx launcher is a console application
  if(WIN32)
    set(Cjyx_HAS_CONSOLE_IO_SUPPORT ${Cjyx_BUILD_WIN32_CONSOLE})
    set(Cjyx_HAS_CONSOLE_LAUNCHER ${Cjyx_BUILD_WIN32_CONSOLE_LAUNCHER})
  endif()

  set(CJYXAPP_EXE_OPTIONS)
  if(WIN32)
    if(NOT Cjyx_HAS_CONSOLE_IO_SUPPORT)
      set(CJYXAPP_EXE_OPTIONS WIN32)
    endif()
  endif()

  if(APPLE)
    set(CJYXAPP_EXE_OPTIONS MACOSX_BUNDLE)
  endif()

  set(cjyxapp_target ${CJYXAPP_NAME})
  if(DEFINED CJYXAPP_TARGET_NAME_VAR)
    set(${CJYXAPP_TARGET_NAME_VAR} ${cjyxapp_target})
  endif()

  set(executable_name ${CJYXAPP_APPLICATION_NAME})
  if(NOT APPLE)
    set(executable_name ${executable_name}App-real)
  endif()
  message(STATUS "Setting ${CJYXAPP_APPLICATION_NAME} executable name to '${executable_name}${CMAKE_EXECUTABLE_SUFFIX}'")

  ctk_add_executable_utf8(${cjyxapp_target}
    ${CJYXAPP_EXE_OPTIONS}
    Main.cxx
    ${apple_bundle_sources}
    )
  set_target_properties(${cjyxapp_target} PROPERTIES
    LABELS ${CJYXAPP_NAME}
    OUTPUT_NAME ${executable_name}
    )

  if(APPLE)
    set(link_flags "-Wl,-rpath,@loader_path/../")
    set_target_properties(${cjyxapp_target}
      PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "${CJYXAPP_APPLICATION_NAME} ${Cjyx_MAIN_PROJECT_VERSION_FULL}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${Cjyx_MAIN_PROJECT_VERSION_FULL}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${Cjyx_MACOSX_BUNDLE_GUI_IDENTIFIER}"
        MACOSX_BUNDLE_INFO_PLIST "${Cjyx_CMAKE_DIR}/MacOSXBundleInfo.plist.in"
        LINK_FLAGS ${link_flags}
      )
    if("${Cjyx_RELEASE_TYPE}" STREQUAL "Stable")
      set_target_properties(${cjyxapp_target} PROPERTIES
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${Cjyx_VERSION_MAJOR}.${Cjyx_VERSION_MINOR}.${Cjyx_VERSION_PATCH}"
        )
    endif()
  endif()

  get_target_property(_cjyxapp_output_dir ${cjyxapp_target} RUNTIME_OUTPUT_DIRECTORY)
  set(_cjyxapp_build_subdir "")

  if(APPLE)
    get_target_property(_is_bundle ${cjyxapp_target} MACOSX_BUNDLE)
    if(_is_bundle)
      set(_cjyxapp_build_subdir "${executable_name}.app/Contents/MacOS/")
    endif()
  endif()

  set(CJYXAPP_EXECUTABLE "${_cjyxapp_output_dir}/${_cjyxapp_build_subdir}${executable_name}${CMAKE_EXECUTABLE_SUFFIX}")
  _set_app_property("EXECUTABLE")

  if(WIN32)
    if(Cjyx_USE_PYTHONQT)
      # HACK - See https://mantisarchive.slicer.org/view.php?id=1180
      get_filename_component(_python_library_name_we ${PYTHON_LIBRARY} NAME_WE)
      add_custom_command(
        TARGET ${cjyxapp_target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PYTHON_LIBRARY_PATH}/${_python_library_name_we}.dll
                ${_cjyxapp_output_dir}/${CMAKE_CFG_INTDIR}
        COMMENT "Copy '${_python_library_name_we}.dll' along side '${cjyxapp_target}' executable. See Cjyx issue #1180"
        )
    endif()
  endif()

  if(DEFINED CJYXAPP_TARGET_LIBRARIES)
    target_link_libraries(${cjyxapp_target}
      ${CJYXAPP_TARGET_LIBRARIES}
      )
  endif()

  # Folder
  set_target_properties(${cjyxapp_target} PROPERTIES FOLDER ${CJYXAPP_FOLDER})

  # --------------------------------------------------------------------------
  # Install
  # --------------------------------------------------------------------------
  if(NOT APPLE)
    set(CJYXAPP_INSTALL_DESTINATION_ARGS RUNTIME DESTINATION ${Cjyx_INSTALL_BIN_DIR})
  else()
    set(CJYXAPP_INSTALL_DESTINATION_ARGS BUNDLE DESTINATION ".")
  endif()

  install(TARGETS ${cjyxapp_target}
    ${CJYXAPP_INSTALL_DESTINATION_ARGS}
    COMPONENT Runtime)

  if(DEFINED CJYXAPP_DEFAULT_SETTINGS_FILE)
    get_filename_component(default_settings_filename ${CJYXAPP_DEFAULT_SETTINGS_FILE} NAME)
    set(dest_default_settings_filename ${default_settings_filename})
    if(NOT ${default_settings_filename} MATCHES "^${CJYXAPP_APPLICATION_NAME}")
      set(dest_default_settings_filename ${CJYXAPP_APPLICATION_NAME}${default_settings_filename})
    endif()
    set(default_settings_build_dir ${CMAKE_BINARY_DIR}/${Cjyx_SHARE_DIR})
    message(STATUS "Copying '${default_settings_filename}' to '${default_settings_build_dir}/${dest_default_settings_filename}'")
    configure_file(
      ${CJYXAPP_DEFAULT_SETTINGS_FILE}
      ${default_settings_build_dir}/${dest_default_settings_filename}
      COPYONLY
      )
    install(FILES
      ${CJYXAPP_DEFAULT_SETTINGS_FILE}
      DESTINATION ${Cjyx_INSTALL_SHARE_DIR} COMPONENT Runtime
      RENAME ${dest_default_settings_filename}
      )
  endif()

  # --------------------------------------------------------------------------
  # Configure Launcher
  # --------------------------------------------------------------------------
  if(CJYXAPP_CONFIGURE_LAUNCHER)
    if(Cjyx_USE_CTKAPPLAUNCHER)

      find_package(CTKAppLauncher REQUIRED)

      # Define list of extra 'application to launch' to associate with the launcher
      # within the build tree
      set(extraApplicationToLaunchListForBuildTree)

      if(NOT QT_DESIGNER_EXECUTABLE)
        # Since Qt only provides a CMake module to find the designer library, we work
        # around this limitation by finding the designer executable.
        find_program(QT_DESIGNER_EXECUTABLE designer Designer HINTS "${QT_BINARY_DIR}" NO_DEFAULT_PATH)
      endif()

      if(EXISTS ${QT_DESIGNER_EXECUTABLE})
        ctkAppLauncherAppendExtraAppToLaunchToList(
          LONG_ARG designer
          HELP "Start Qt designer using Cjyx plugins"
          PATH ${QT_DESIGNER_EXECUTABLE}
          OUTPUTVAR extraApplicationToLaunchListForBuildTree
          )
      endif()
      set(executables)
      if(UNIX)
        list(APPEND executables gnome-terminal xterm)
      elseif(WIN32)
        list(APPEND executables VisualStudio VisualStudioProject cmd)
        set(VisualStudio_EXECUTABLE ${CMAKE_VS_DEVENV_COMMAND})
        set(VisualStudio_HELP "Open Visual Studio with Cjyx's DLL paths set up")
        set(VisualStudioProject_EXECUTABLE ${CMAKE_VS_DEVENV_COMMAND})
        set(VisualStudioProject_ARGUMENTS ${Cjyx_BINARY_DIR}/Cjyx.sln)
        set(VisualStudioProject_HELP "Open Visual Studio Cjyx project with Cjyx's DLL paths set up")
        set(cmd_ARGUMENTS "/c start cmd")
      endif()
      foreach(executable ${executables})
        find_program(${executable}_EXECUTABLE ${executable})
        if(${executable}_EXECUTABLE)
          message(STATUS "Enabling ${CJYXAPP_APPLICATION_NAME} build tree launcher option: --${executable}")
          if(NOT DEFINED ${executable}_HELP)
            set(${executable}_HELP "Start ${executable}")
          endif()
          ctkAppLauncherAppendExtraAppToLaunchToList(
            LONG_ARG ${executable}
            HELP ${${executable}_HELP}
            PATH ${${executable}_EXECUTABLE}
            ARGUMENTS ${${executable}_ARGUMENTS}
            OUTPUTVAR extraApplicationToLaunchListForBuildTree
            )
        endif()
      endforeach()

      # Define list of extra 'application to launch' to associate with the launcher
      # within the install tree
      set(executables)
      if(WIN32)
        list(APPEND executables cmd)
        set(cmd_ARGUMENTS "/c start cmd")
      endif()
      foreach(executable ${executables})
        find_program(${executable}_EXECUTABLE ${executable})
        if(${executable}_EXECUTABLE)
          message(STATUS "Enabling ${CJYXAPP_APPLICATION_NAME} install tree launcher option: --${executable}")
          ctkAppLauncherAppendExtraAppToLaunchToList(
            LONG_ARG ${executable}
            HELP "Start ${executable}"
            PATH ${${executable}_EXECUTABLE}
            ARGUMENTS ${${executable}_ARGUMENTS}
            OUTPUTVAR extraApplicationToLaunchListForInstallTree
            )
        endif()
      endforeach()

      if(EXISTS ${QT_DESIGNER_EXECUTABLE} AND NOT APPLE)
        ctkAppLauncherAppendExtraAppToLaunchToList(
          LONG_ARG designer
          HELP "Start Qt designer using Cjyx plugins"
          PATH "<APPLAUNCHER_SETTINGS_DIR>/../bin/designer-real${CMAKE_EXECUTABLE_SUFFIX}"
          OUTPUTVAR extraApplicationToLaunchListForInstallTree
          )
      endif()

      include(CjyxBlockCTKAppLauncherSettings)

      if(CJYXAPP_SPLASHSCREEN_ENABLED)
        set(_launcher_splashscreen_args
          SPLASH_IMAGE_PATH ${CJYXAPP_LAUNCHER_SPLASHSCREEN_FILE}
          SPLASH_IMAGE_INSTALL_SUBDIR ${Cjyx_BIN_DIR}
          SPLASHSCREEN_HIDE_DELAY_MS 3000
          )
        set(_launcher_application_default_arguments "${CJYXAPP_APPLICATION_DEFAULT_ARGUMENTS}")
      else()
        set(_launcher_splashscreen_args SPLASHSCREEN_DISABLED)
        set(_launcher_application_default_arguments "--no-splash ${CJYXAPP_APPLICATION_DEFAULT_ARGUMENTS}")
      endif()

      ctkAppLauncherConfigureForTarget(
        # Executable target associated with the launcher
        TARGET ${cjyxapp_target}
        # Location of the launcher settings in the install tree
        APPLICATION_INSTALL_SUBDIR ${Cjyx_BIN_DIR}
        # Info allowing to retrieve the Cjyx extension settings
        APPLICATION_NAME ${CJYXAPP_APPLICATION_NAME}
        APPLICATION_REVISION ${Cjyx_REVISION}
        ORGANIZATION_DOMAIN ${Cjyx_ORGANIZATION_DOMAIN}
        ORGANIZATION_NAME ${Cjyx_ORGANIZATION_NAME}
        USER_ADDITIONAL_SETTINGS_FILEBASENAME ${CJYX_REVISION_SPECIFIC_USER_SETTINGS_FILEBASENAME}
        # Splash screen
        ${_launcher_splashscreen_args}
        # Cjyx default arguments
        APPLICATION_DEFAULT_ARGUMENTS ${_launcher_application_default_arguments}
        # Cjyx arguments triggering display of launcher help
        HELP_SHORT_ARG "-h"
        HELP_LONG_ARG "--help"
        # Cjyx arguments that should NOT be associated with the spash screeb
        NOSPLASH_ARGS "--no-splash,--help,--version,--home,--program-path,--no-main-window,--settings-path,--temporary-path"
        # Extra application associated with the launcher
        EXTRA_APPLICATION_TO_LAUNCH_BUILD ${extraApplicationToLaunchListForBuildTree}
        EXTRA_APPLICATION_TO_LAUNCH_INSTALLED ${extraApplicationToLaunchListForInstallTree}
        # Location of the launcher settings in the build tree
        DESTINATION_DIR ${Cjyx_BINARY_DIR}
        # Launcher settings specific to build tree
        LIBRARY_PATHS_BUILD "${CJYX_LIBRARY_PATHS_BUILD}"
        PATHS_BUILD "${CJYX_PATHS_BUILD}"
        ENVVARS_BUILD "${CJYX_ENVVARS_BUILD}"
        # Launcher settings specific to install tree
        LIBRARY_PATHS_INSTALLED "${CJYX_LIBRARY_PATHS_INSTALLED}"
        PATHS_INSTALLED "${CJYX_PATHS_INSTALLED}"
        ENVVARS_INSTALLED "${CJYX_ENVVARS_INSTALLED}"
        # The ADDITIONAL_PATH_ENVVARS_(BUILD_INSTALLED) variables contains names of
        # environment variables expected to be associated with a list of paths.
        # Examples of such variables are PYTHONPATH, QT_PLUGIN_PATH, ...
        # For each "ADDITIONAL_PATH_ENVVARS", the "ctkAppLauncherConfigure" macro
        # will look for variables named <ADDITIONAL_PATH_ENVVARS_PREFIX>_<ADDITIONAL_PATH_ENVVAR>_(BUILD|INSTALLED)
        # listing paths.
        # For example: CJYX_PYTHONPATH_BUILD, CJYX_PYTHONPATH_INSTALLED
        ADDITIONAL_PATH_ENVVARS_PREFIX CJYX_
        ADDITIONAL_PATH_ENVVARS_BUILD "${CJYX_ADDITIONAL_PATH_ENVVARS_BUILD}"
        ADDITIONAL_PATH_ENVVARS_INSTALLED "${CJYX_ADDITIONAL_PATH_ENVVARS_INSTALLED}"
        )

      # Folder
      set_target_properties(${CJYXAPP_APPLICATION_NAME}ConfigureLauncher PROPERTIES FOLDER ${CJYXAPP_FOLDER})

      if(NOT APPLE)
        if(Cjyx_HAS_CONSOLE_LAUNCHER)
          install(
            PROGRAMS "${Cjyx_BINARY_DIR}/${CJYXAPP_APPLICATION_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
            DESTINATION "."
            COMPONENT Runtime
            )
        else()
          # Create command to update launcher icon
          add_custom_command(
            DEPENDS
              ${CTKAppLauncher_DIR}/bin/CTKAppLauncherW${CMAKE_EXECUTABLE_SUFFIX}
            OUTPUT
              ${Cjyx_BINARY_DIR}/CMakeFiles/${CJYXAPP_APPLICATION_NAME}W${CMAKE_EXECUTABLE_SUFFIX}
            COMMAND ${CMAKE_COMMAND} -E copy
              ${CTKAppLauncher_DIR}/bin/CTKAppLauncherW${CMAKE_EXECUTABLE_SUFFIX}
              ${Cjyx_BINARY_DIR}/CMakeFiles/${CJYXAPP_APPLICATION_NAME}W${CMAKE_EXECUTABLE_SUFFIX}
            COMMAND
              ${CTKResEdit_EXECUTABLE}
                --update-resource-ico ${Cjyx_BINARY_DIR}/CMakeFiles/${CJYXAPP_APPLICATION_NAME}W${CMAKE_EXECUTABLE_SUFFIX}
                IDI_ICON1 ${CJYXAPP_WIN_ICON_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT ""
            )
          add_custom_target(${CJYXAPP_APPLICATION_NAME}UpdateLauncherWIcon ALL
            DEPENDS
              ${Cjyx_BINARY_DIR}/CMakeFiles/${CJYXAPP_APPLICATION_NAME}W${CMAKE_EXECUTABLE_SUFFIX}
            )

          # Folder
          set_target_properties(${CJYXAPP_APPLICATION_NAME}UpdateLauncherWIcon PROPERTIES FOLDER ${CJYXAPP_FOLDER})
          install(
            PROGRAMS "${Cjyx_BINARY_DIR}/CMakeFiles/${CJYXAPP_APPLICATION_NAME}W${CMAKE_EXECUTABLE_SUFFIX}"
            DESTINATION "."
            RENAME "${CJYXAPP_APPLICATION_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
            COMPONENT Runtime
            )
        endif()

        if(CJYXAPP_SPLASHSCREEN_ENABLED)
          install(
            FILES ${CJYXAPP_LAUNCHER_SPLASHSCREEN_FILE}
            DESTINATION ${Cjyx_INSTALL_BIN_DIR}
            COMPONENT Runtime
            )
        endif()
      endif()

      #
      # On macOS, the installed launcher settings are *only* read directly by the
      # qCjyxCoreApplication using the LauncherLib.
      #
      # On Linux and Windows, the installed launcher settings are first read by the
      # installed launcher, and then read using the LauncherLib.
      #
      install(
        FILES "${Cjyx_BINARY_DIR}/${CJYXAPP_APPLICATION_NAME}LauncherSettingsToInstall.ini"
        DESTINATION ${Cjyx_INSTALL_BIN_DIR}
        RENAME ${CJYXAPP_APPLICATION_NAME}LauncherSettings.ini
        COMPONENT Runtime
        )

      if(WIN32)
        # Create target to update launcher icon
        add_custom_target(${CJYXAPP_APPLICATION_NAME}UpdateLauncherIcon ALL
          COMMAND
            ${CTKResEdit_EXECUTABLE}
              --update-resource-ico ${Cjyx_BINARY_DIR}/${CJYXAPP_APPLICATION_NAME}${CMAKE_EXECUTABLE_SUFFIX}
              IDI_ICON1 ${CJYXAPP_WIN_ICON_FILE}
          )
        add_dependencies(${CJYXAPP_APPLICATION_NAME}UpdateLauncherIcon ${CJYXAPP_APPLICATION_NAME}ConfigureLauncher)

        # Folder
        set_target_properties(${CJYXAPP_APPLICATION_NAME}UpdateLauncherIcon PROPERTIES FOLDER ${CJYXAPP_FOLDER})
      endif()

    endif()
  endif()

endmacro()
