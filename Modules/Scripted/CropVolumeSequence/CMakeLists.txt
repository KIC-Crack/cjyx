#-----------------------------------------------------------------------------
set(MODULE_NAME CropVolumeSequence)

#-----------------------------------------------------------------------------
set(MODULE_PYTHON_SCRIPTS
  ${MODULE_NAME}.py
  )

set(MODULE_PYTHON_RESOURCES
  Resources/Icons/${MODULE_NAME}.png
  )

#-----------------------------------------------------------------------------
cjyxMacroBuildScriptedModule(
  NAME ${MODULE_NAME}
  SCRIPTS ${MODULE_PYTHON_SCRIPTS}
  RESOURCES ${MODULE_PYTHON_RESOURCES}
  WITH_GENERIC_TESTS
  )

#-----------------------------------------------------------------------------
if(BUILD_TESTING)

  if(CMAKE_CONFIGURATION_TYPES)
    set(MODULE_BUILD_DIR "")
    foreach(config ${CMAKE_CONFIGURATION_TYPES})
      list(APPEND MODULE_BUILD_DIR "${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_LIB_DIR}/${config}")
    endforeach()
  else()
    set(MODULE_BUILD_DIR "${CMAKE_BINARY_DIR}/${Cjyx_QTLOADABLEMODULES_LIB_DIR}")
  endif()

  # Register the unittest subclass in the main script as a ctest.
  # Note that the test will also be available at runtime.
  cjyx_add_python_unittest(
    SCRIPT ${MODULE_NAME}.py
    # TODO: cjyx_add_python_unittest should take care of adding the additional module paths
    CJYX_ARGS
      --additional-module-paths ${MODULE_BUILD_DIR} ${CMAKE_BINARY_DIR}/${Cjyx_QTSCRIPTEDMODULES_LIB_DIR}
    )

  # Additional build-time testing
  add_subdirectory(Testing)
endif()
