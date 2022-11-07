
include(${Cjyx_CMAKE_DIR}/CjyxMacroBuildLoadableModule.cmake)

set(qtmodules
  Cameras
  Units
  Terminologies
  SubjectHierarchy
  Colors # Colors depends of SubjectHierarchy
  Annotations
  Markups
  Transforms
  Data
  Models
  Plots
  SceneViews
  Segmentations
  Sequences
  CjyxWelcome
  Tables
  Texts
  Reformat
  ViewControllers
  Volumes
  VolumeRendering # Volume Rendering depends of Volumes
  )
if(Cjyx_BUILD_CLI_SUPPORT)
  list(APPEND qtmodules
    CropVolume
    )
endif()

include(CjyxCheckModuleEnabled)

foreach(module ${qtmodules})
  cjyx_is_loadable_builtin_module_enabled(${module} _build_module)
  if(_build_module)
    add_subdirectory(${module})
  endif()
endforeach()
