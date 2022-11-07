
if(BUILD_TESTING)
  # Code specific to Cjyx testing is added here.

endif()

#
# Test templates - Common to both Cjyx and the extensions
#

set(Cjyx_PYTHON_MODULE_TEST_TEMPLATES_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  CACHE INTERNAL "Path to python test templates directory.")

# Install the file so that project building against the 'installed tree' can reference it.
if(NOT Cjyx_INSTALL_NO_DEVELOPMENT)
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/qCjyxModuleGenericTest.py.in
    DESTINATION ${Cjyx_INSTALL_SHARE_DIR}/test-templates COMPONENT Development
    )
endif()
