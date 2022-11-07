set(modules
  CropVolumeSequence
  DataProbe
  PerformanceTests
  SampleData
  ScreenCapture
  SegmentEditor
  SegmentStatistics
  SelfTests
  VectorToScalarVolume
  WebServer
  )
if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  list(APPEND modules
    DMRIInstall
    ExtensionWizard
    )
endif()
if(Cjyx_USE_NUMPY)
  list(APPEND modules
    Endoscopy
    )
endif()
if(Cjyx_BUILD_DICOM_SUPPORT)
  list(APPEND modules
    DICOM
    DICOMLib
    DICOMPlugins
    DICOMPatcher
    )
endif()

include(CjyxCheckModuleEnabled)

foreach(module ${modules})
  cjyx_is_scripted_builtin_module_enabled(${module} _build_module)
  if(_build_module)
    add_subdirectory(${module})
  endif()
endforeach()
