if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT AND Cjyx_USE_PYTHONQT)
  # Determine name of Python interpreter
  #
  # NOTE: Do NOT use ${PYTHON_EXECUTABLE}; when using our own Python, it is set
  #       to a ctkAppLauncher wrapper, and we don't want that!
  if(Cjyx_USE_SYSTEM_python)
    set(PYTHON python)
  else()
    find_program(PYTHON python
      PATHS ${Cjyx_SUPERBUILD_DIR}/python-install/bin
      NO_DEFAULT_PATH
      )
  endif()

  # Configure and Extension Wizard launcher scripts
  configure_file(
    cjyxExtensionWizard.sh.in
    ${Cjyx_BINARY_DIR}/bin/cjyxExtensionWizard @ONLY
    )

  configure_file(
    cjyxExtensionWizard-install.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/cjyxExtensionWizard-install.sh @ONLY
    )

  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/cjyxExtensionWizard-install.sh
    DESTINATION ${Cjyx_INSTALL_BIN_DIR}
    COMPONENT Runtime
    RENAME cjyxExtensionWizard
    )

  # Install Python libexec scripts
  set(Cjyx_LIBEXEC_PYTHON_SCRIPTS
    ExtensionWizard.py
    )

  ctkMacroCompilePythonScript(
    TARGET_NAME ExecScripts
    SCRIPTS "${Cjyx_LIBEXEC_PYTHON_SCRIPTS}"
    DESTINATION_DIR ${Cjyx_BINARY_DIR}/libexec
    INSTALL_DIR ${Cjyx_INSTALL_LIBEXEC_DIR}
    NO_INSTALL_SUBDIR
    )
endif()
