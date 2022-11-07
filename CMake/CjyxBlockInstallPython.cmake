# -------------------------------------------------------------------------
# Find and install python
# -------------------------------------------------------------------------
if(Cjyx_USE_PYTHONQT)

  # Sanity checks
  foreach(varname IN ITEMS Cjyx_SUPERBUILD_DIR PYTHON_STDLIB_SUBDIR)
    if("${${varname}}" STREQUAL "")
      message(FATAL_ERROR "${varname} CMake variable is expected to be set")
    endif()
  endforeach()

  set(PYTHON_DIR "${Cjyx_SUPERBUILD_DIR}/python-install")
  if(NOT EXISTS "${PYTHON_DIR}/${PYTHON_STDLIB_SUBDIR}")
    message(WARNING "###############################################################################
Skipping generation of python install rules

Non-existing directory PYTHON_DIR:${PYTHON_DIR}/${PYTHON_STDLIB_SUBDIR}

This probably means that you are building Cjyx against your own installation of Python.

To create a Cjyx package including python libraries, you can *NOT* provide your own version of the python libraries.
###############################################################################")
    return()
  endif()

  # Install libraries

  set(extra_exclude_pattern)
  if(UNIX)
    list(APPEND extra_exclude_pattern
      REGEX "distutils/command/wininst-.*" EXCLUDE
      )
  endif()

  install(
    DIRECTORY "${PYTHON_DIR}/${PYTHON_STDLIB_SUBDIR}/"
    DESTINATION "${Cjyx_INSTALL_ROOT}lib/Python/${PYTHON_STDLIB_SUBDIR}/"
    COMPONENT Runtime
    USE_SOURCE_PERMISSIONS
    REGEX "lib2to3/tests/" EXCLUDE
    REGEX "lib[-]old/" EXCLUDE
    REGEX "plat[-].*" EXCLUDE
    REGEX "/test/" EXCLUDE
    REGEX "/tests/" EXCLUDE
    ${extra_exclude_pattern}
    )
  # Strip symbols of selected libraries for which ones (1) stripping has a
  # significant impact and (2) stripping does not prevent the import of
  # of the module.
  # See https://github.com/Slicer/Slicer/issues/5474
  cjyxStripInstalledLibrary(
    PATTERN "${Cjyx_INSTALL_ROOT}lib/Python/${PYTHON_STDLIB_SUBDIR}/_SimpleITK.*.so"
    COMPONENT Runtime
    )

  # Install python library
  if(UNIX)
    if(NOT APPLE)
      cjyxInstallLibrary(
        FILE ${PYTHON_LIBRARY}
        DESTINATION ${Cjyx_INSTALL_ROOT}lib/Python/lib
        COMPONENT Runtime
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ)
      # Explicitly call "cjyxStripInstalledLibrary" because directly
      # calling "cjyxInstallLibrary" improperly gets the python library
      # filename ("libpython3" instead of "libpython3.6m").
      #
      # This happens because "cjyxInstallLibrary" internally uses
      # "get_filename_component" with the NAME_WE option.
      get_filename_component(libname ${PYTHON_LIBRARY} NAME)
      cjyxStripInstalledLibrary(
        FILES "${Cjyx_INSTALL_ROOT}lib/Python/lib/${libname}"
        COMPONENT Runtime)
    endif()
  elseif(WIN32)
    # Copy Python36.dll
    get_filename_component(PYTHON_LIB_BASE ${PYTHON_LIBRARY} NAME_WE)
    install(FILES "${PYTHON_LIBRARY_PATH}/${PYTHON_LIB_BASE}.dll"
      DESTINATION bin
      COMPONENT Runtime)
    # Copy python3.dll
    # The file name is always exactly this, as this file is guaranteed
    # to be ABI compatible across all Python3 versions
    # (see https://www.python.org/dev/peps/pep-0384/#linkage
    # and https://github.com/Slicer/Slicer/issues/5816 for details).
    install(FILES "${PYTHON_LIBRARY_PATH}/python3.dll"
      DESTINATION bin
      COMPONENT Runtime)
  endif()

  # Install interpreter
  get_filename_component(python_bin_dir ${PYTHON_EXECUTABLE} PATH)
  install(
    PROGRAMS ${python_bin_dir}/python${CMAKE_EXECUTABLE_SUFFIX}
    DESTINATION ${Cjyx_INSTALL_BIN_DIR}
    RENAME python-real${CMAKE_EXECUTABLE_SUFFIX}
    COMPONENT Runtime
    )

  if(APPLE)
    # Fixes Cjyx issue #4554
    set(dollar "$")
    install(CODE
      "set(app ${Cjyx_INSTALL_BIN_DIR}/python-real)
       set(appfilepath \"${dollar}ENV{DESTDIR}${dollar}{CMAKE_INSTALL_PREFIX}/${dollar}{app}\")
       message(\"CPack: - Adding rpath to ${dollar}{app}\")
       execute_process(COMMAND install_name_tool -add_rpath @loader_path/..  ${dollar}{appfilepath})"
      COMPONENT Runtime
      )
  endif()

  # Install Cjyx python launcher settings

  macro(_install_python_launcher executablename)
    # Install Cjyx python launcher settings
    install(
      FILES ${python_bin_dir}/${executablename}LauncherSettingsToInstall.ini
      DESTINATION ${Cjyx_INSTALL_BIN_DIR}
      RENAME ${executablename}LauncherSettings.ini
      COMPONENT Runtime
      )
    # Install Cjyx python launcher
    # Regardless of how the main application is built (GUI or console application - as specified by Cjyx_BUILD_WIN32_CONSOLE),
    # the Python console is always use the console launcher (CTKAppLauncher) and not the GUI launcher (CTKAppLauncherW).
    set(_launcher CTKAppLauncher)
    install(
      PROGRAMS ${CTKAppLauncher_DIR}/bin/${_launcher}${CMAKE_EXECUTABLE_SUFFIX}
      DESTINATION ${Cjyx_INSTALL_BIN_DIR}
      RENAME ${executablename}${CMAKE_EXECUTABLE_SUFFIX}
      COMPONENT Runtime
      )
  endmacro()

  _install_python_launcher(PythonCjyx)

  # Install headers
  set(python_include_subdir /Include/)
  if(UNIX)
    set(python_include_subdir /include/python${Cjyx_PYTHON_VERSION_DOT}${Cjyx_PYTHON_ABIFLAGS}/)
  endif()

  install(FILES "${PYTHON_DIR}${python_include_subdir}/pyconfig.h"
    DESTINATION ${Cjyx_INSTALL_ROOT}lib/Python${python_include_subdir}
    COMPONENT Runtime
    )

endif()
