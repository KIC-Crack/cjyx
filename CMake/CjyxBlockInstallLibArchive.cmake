# -------------------------------------------------------------------------
# Find and install LibArchive
# -------------------------------------------------------------------------
if(WIN32)
  install(FILES ${LibArchive_DIR}/bin/archive.dll
    DESTINATION ${Cjyx_INSTALL_LIB_DIR}
    COMPONENT Runtime
    )
else()
  cjyxInstallLibrary(FILE ${LibArchive_LIBRARY}
    DESTINATION ${Cjyx_INSTALL_LIB_DIR}
    COMPONENT Runtime
    STRIP
    )
endif()

