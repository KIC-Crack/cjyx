
# In case the list was converted to a string with carats, replace them to comply with list convention
string (REPLACE "^^" ";" Cjyx_EXTENSION_INSTALL_DIRS "${Cjyx_EXTENSION_INSTALL_DIRS}")

# Install extension install directories
foreach(dir IN LISTS Cjyx_EXTENSION_INSTALL_DIRS)
  if(NOT EXISTS ${dir})
    message(WARNING "Skipping nonexistent extension install directory [${dir}]")
    continue()
  endif()
  install(
    DIRECTORY "${dir}/"
    DESTINATION ${Cjyx_INSTALL_ROOT}
    USE_SOURCE_PERMISSIONS
    COMPONENT Runtime
    )
endforeach()
