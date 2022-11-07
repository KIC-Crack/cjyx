
#-----------------------------------------------------------------------------
# WARNING: CLI modules should be topologically ordered.
#-----------------------------------------------------------------------------
set(cli_modules
  ACPCTransform
  AddScalarVolumes
  CastScalarVolume
  CheckerBoardFilter
  CurvatureAnisotropicDiffusion
  ExecutionModelTour
  ExtractSkeleton
  GaussianBlurImageFilter
  GradientAnisotropicDiffusion
  GrayscaleFillHoleImageFilter
  GrayscaleGrindPeakImageFilter
  GrayscaleModelMaker
  HistogramMatching
  ImageLabelCombine
  LabelMapSmoothing
  MaskScalarVolume
  MedianImageFilter
  MergeModels
  ModelMaker
  ModelToLabelMap
  MultiplyScalarVolumes
  N4ITKBiasFieldCorrection
  OrientScalarVolume
  ProbeVolumeWithModel
  ResampleDTIVolume # Needed by ResampleScalarVectorDWIVolume (compile time dependency)
  ResampleScalarVectorDWIVolume # Depends on DiffusionApplications, needed by CropVolume (runtime dependency)
  RobustStatisticsSegmenter
  SimpleRegionGrowingSegmentation
  SubtractScalarVolumes
  ThresholdScalarVolume
  VotingBinaryHoleFillingImageFilter
  )
if(BUILD_TESTING)
  list(APPEND cli_modules ROITest)
endif()

if(Cjyx_BUILD_DICOM_SUPPORT)
  list(APPEND cli_modules CreateDICOMSeries)
  list(APPEND cli_modules PETStandardUptakeValueComputation)
endif()

if(Cjyx_BUILD_LEGACY_CLI)
  set(legacy_cli_modules
    FiducialRegistration
    ResampleScalarVolume
    )
  if(BUILD_TESTING)
    list(APPEND legacy_cli_modules
      DiffusionTensorTest
      TestGridTransformRegistration
      )
  endif()
  list(APPEND cli_modules ${legacy_cli_modules})
endif()

if(BUILD_TESTING)
  # TODO Since the following snippet have been moved from CLI/Testing/CMakeLists.txt into
  # CLI/CMakeLists.txt, the variable CJYX_BRAINWEB_DATA_ROOT won't be set properly.
  # Configure the default CJYX_BRAINWEB_DATA_ROOT for the location of BrainWeb Data.
  # When this data is available, additional 3D tests are enabled.
  find_path(CJYX_BRAINWEB_DATA_ROOT brainweb165a10f17.mha
    DOC "Directory with data taken from https://public.kitware.com/pub/itk/Data/BrainWeb/")
  mark_as_advanced(CJYX_BRAINWEB_DATA_ROOT)

  # Set CLI testing directories as cache variable so that it can be used by
  # remote module CjyxDMRI included from top-level CMakeLists.txt
  set(BASELINE "${Cjyx_SOURCE_DIR}/Testing/Data/Baseline/CLI"
    CACHE PATH "MRML CLI baseline directory")
  set(TEST_DATA "${Cjyx_SOURCE_DIR}/Testing/Data/Input"
    CACHE PATH "MRML CLI testing input data directory")
  set(MRML_TEST_DATA "${Cjyx_SOURCE_DIR}/Libs/MRML/Core/Testing/TestData"
    CACHE PATH "MRML CLI testing common data directory")
  set(TEMP "${Cjyx_BINARY_DIR}/Testing/Temporary"
    CACHE PATH "MRML CLI testing temp directory")

  set(MRML_DATA "${Cjyx_HOME}/share/MRML/Testing/TestData")
endif()

include(CjyxCheckModuleEnabled)

foreach(module ${cli_modules})
  set(${module}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${module})
  cjyx_is_cli_builtin_module_enabled(${module} _build_module)
  if(_build_module)
    add_subdirectory(${module})
  endif()
endforeach()
