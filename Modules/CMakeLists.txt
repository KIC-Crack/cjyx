# "Core" folder is included in the top-level CMakeLists.txt just before "Base/QTApp". Doing so
# make it possible for QTApp to use on the Core module factory.

if(Cjyx_BUILD_QTLOADABLEMODULES)
  add_subdirectory(Loadable)
endif()

if(Cjyx_BUILD_QTSCRIPTEDMODULES)
  add_subdirectory(Scripted)
endif()

if(Cjyx_BUILD_CLI_SUPPORT AND Cjyx_BUILD_CLI)
  add_subdirectory(CLI)
endif()
