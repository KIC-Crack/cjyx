project(qCjyxBaseQTCore)

#
# Dependencies
#

#
# DCMTK
#
if(Cjyx_BUILD_DICOM_SUPPORT)
  find_package(DCMTK REQUIRED)
endif()

#
# ModuleDescriptionParser - Required to define ModuleDescriptionParser_INCLUDE_DIRS
#
if(Cjyx_BUILD_CLI_SUPPORT)
  find_package(CjyxExecutionModel REQUIRED ModuleDescriptionParser)

  #
  # ITK
  #
  set(${PROJECT_NAME}_ITK_COMPONENTS
    # Import ITK targets required by ModuleDescriptionParser
    ${ModuleDescriptionParser_ITK_COMPONENTS}
    # Import ITK targets required by CTKImageProcessingITKCore
    ITKCommon
    )
  find_package(ITK 4.6 COMPONENTS ${${PROJECT_NAME}_ITK_COMPONENTS} REQUIRED)
  if(ITK_VERSION VERSION_GREATER_EQUAL "5.3")
    foreach(factory_uc IN ITEMS "IMAGEIO" "MESHIO" "TRANSFORMIO")
      set(ITK_NO_${factory_uc}_FACTORY_REGISTER_MANAGER 1)
    endforeach()
  else()
    set(ITK_NO_IO_FACTORY_REGISTER_MANAGER 1) # See Libs/ITKFactoryRegistration/CMakeLists.txt
  endif()
  include(${ITK_USE_FILE})
endif()

#
# qRestAPI
#
if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  find_package(qRestAPI REQUIRED)
  include(${qRestAPI_USE_FILE})
endif()

#
# CTKAppLauncherLib
#
find_package(CTKAppLauncherLib REQUIRED)

#
# See CMake/CjyxMacroBuildBaseQtLibrary.cmake for details
#

set(KIT_export_directive "Q_CJYX_BASE_QTCORE_EXPORT")

# Additional directories to include
set(KIT_include_directories
  ${MRMLLogic_INCLUDE_DIRS}
  ${MRMLDisplayableManager_INCLUDE_DIRS}
  )

if(Cjyx_BUILD_CLI_SUPPORT)
  list(APPEND KIT_include_directories
    ${ModuleDescriptionParser_INCLUDE_DIRS}
    ${MRMLCLI_INCLUDE_DIRS}
    )
endif()

# Source files
set(KIT_SRCS
  qCjyxAbstractCoreModule.cxx
  qCjyxAbstractCoreModule.h
  qCjyxAbstractModuleFactoryManager.cxx
  qCjyxAbstractModuleFactoryManager.h
  qCjyxAbstractModuleRepresentation.cxx
  qCjyxAbstractModuleRepresentation.h
  qCjyxCoreApplication.cxx
  qCjyxCoreApplication.h
  qCjyxCoreApplication_p.h
  qCjyxCoreCommandOptions.cxx
  qCjyxCoreCommandOptions.h
  qCjyxCoreIOManager.cxx
  qCjyxCoreIOManager.h
  qCjyxFileReader.cxx
  qCjyxFileReader.h
  qCjyxFileWriter.cxx
  qCjyxFileWriter.h
  qCjyxIO.cxx
  qCjyxIO.h
  qCjyxIOOptions.cxx
  qCjyxIOOptions.h
  qCjyxIOOptions_p.h
  qCjyxLoadableModuleFactory.cxx
  qCjyxLoadableModuleFactory.h
  qCjyxModuleFactoryManager.cxx
  qCjyxModuleFactoryManager.h
  qCjyxModuleManager.cxx
  qCjyxModuleManager.h
  qCjyxObject.cxx
  qCjyxObject.h
  qCjyxPersistentCookieJar.cxx
  qCjyxPersistentCookieJar.h
  qCjyxRelativePathMapper.cxx
  qCjyxRelativePathMapper.h
  qCjyxSceneBundleReader.cxx
  qCjyxSceneBundleReader.h
  qCjyxUtils.cxx
  qCjyxUtils.h
  )

if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  list(APPEND KIT_SRCS
    qCjyxExtensionDownloadTask.cxx
    qCjyxExtensionDownloadTask.h
    qCjyxExtensionsManagerModel.cxx
    qCjyxExtensionsManagerModel.h
    )
endif()

if(Cjyx_USE_PYTHONQT)
  list(APPEND KIT_SRCS
    qCjyxScriptedFileReader.cxx
    qCjyxScriptedFileReader.h
    qCjyxScriptedFileWriter.cxx
    qCjyxScriptedFileWriter.h
    qCjyxScriptedUtils.cxx
    qCjyxScriptedUtils_p.h
    )
endif()

# Since the wrapper doesn't consider hierarchy of classes, let's exclude
# classes deriving from pure abstract class and remaining abstract pure
set_source_files_properties(
  qCjyxFileReader.h
  qCjyxFileWriter.h
  WRAP_EXCLUDE
  )

# Headers that should run through moc
set(KIT_MOC_SRCS
  qCjyxAbstractCoreModule.h
  qCjyxAbstractModuleFactoryManager.h
  qCjyxCoreCommandOptions.h
  qCjyxCoreApplication.h
  qCjyxCoreIOManager.h
  qCjyxFileReader.h
  qCjyxFileWriter.h
  qCjyxIO.h
  qCjyxModuleFactoryManager.h
  qCjyxModuleManager.h
  qCjyxRelativePathMapper.h
  qCjyxSceneBundleReader.h
  )

if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  list(APPEND KIT_MOC_SRCS
    qCjyxExtensionDownloadTask.h
    qCjyxExtensionsManagerModel.h
    )
endif()

if(Cjyx_USE_PYTHONQT)
  list(APPEND KIT_MOC_SRCS
    qCjyxScriptedFileReader.h
    qCjyxScriptedFileWriter.h
    )
endif()

# Additional Target libraries
set(KIT_target_libraries
  ${QT_LIBRARIES}
  CjyxBaseLogic
  MRMLDisplayableManager
  ${CTKAppLauncherLib_LIBRARIES}
  CTKCore
  CTKImageProcessingITKCore
  CTKVisualizationVTKCore
  ${ITK_LIBRARIES}
  )

if(Cjyx_BUILD_DICOM_SUPPORT)
  list(APPEND KIT_target_libraries
    ${DCMTK_LIBRARIES}
    CTKDICOMCore
    )
endif()

if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  list(APPEND KIT_target_libraries
    qRestAPI
    )
endif()

if(Cjyx_BUILD_CLI_SUPPORT)
  list(APPEND KIT_target_libraries
    ${ITK_LIBRARIES}
    ModuleDescriptionParser
    )
endif()

# Resources
set(KIT_resources
  Resources/qCjyxBaseQTCore.qrc
  )

if(APPLE)
  set(QT_CONF_FILEPATH "${CMAKE_CURRENT_BINARY_DIR}/Resources/qt.conf")
  file(WRITE ${QT_CONF_FILEPATH}
"[Paths]
Plugins = ${Cjyx_QtPlugins_DIR}
")

  configure_file(
    Resources/qCjyxBaseQTCoreExtra.qrc.in
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/qCjyxBaseQTCoreExtra.qrc
    )

  list(APPEND KIT_resources
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/qCjyxBaseQTCoreExtra.qrc
    )
endif()

# Python wrap
if(Cjyx_USE_PYTHONQT)
  set(VTK_COMMON_PYTHON_LIBRARY
    VTK::WrappingPythonCore # For vtkPythonUtil
    )
  list(APPEND KIT_target_libraries
    CTKScriptingPythonCore
    ${VTK_COMMON_PYTHON_LIBRARY} # For vtkPythonUtils
    )
  if(WIN32)
    # TODO: this resolves link errors on windows, but shouldn't be
    # needed - issue may lie with ctk external library macros?
    list(APPEND KIT_target_libraries
      ${PYTHONQT_LIBRARY}
      )
  endif()

  # Sources
  list(APPEND KIT_SRCS
    qCjyxCorePythonManager.cxx
    qCjyxCorePythonManager.h
    )

  # Headers that should run through moc
  list(APPEND KIT_MOC_SRCS
    qCjyxCorePythonManager.h
    )

endif()

CjyxMacroBuildBaseQtLibrary(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${KIT_export_directive}
  INCLUDE_DIRECTORIES ${KIT_include_directories}
  SRCS ${KIT_SRCS}
  MOC_SRCS ${KIT_MOC_SRCS}
  TARGET_LIBRARIES ${KIT_target_libraries}
  RESOURCES ${KIT_resources}
  WRAP_PYTHONQT
  )

if(Cjyx_USE_PYTHONQT_WITH_OPENSSL)
  configure_file(
    Resources/Certs/Cjyx.crt
    ${Cjyx_BINARY_DIR}/${Cjyx_SHARE_DIR}/Cjyx.crt
    COPYONLY
    )
  install(
    FILES Resources/Certs/Cjyx.crt
    DESTINATION ${Cjyx_INSTALL_SHARE_DIR} COMPONENT Runtime
    )
endif()

# Plugins
#add_subdirectory(Plugins)

# Testing
# Note: Within both Testing/Cxx and Testing/Python, some code is useful to both Cjyx and the
#       Extensions. For that reason, the Testing directory is added inconditionnally.
#       The code specific to Cjyx will be excluded directly in either Testing/Python
#       or Testing/Cxx.
add_subdirectory(Testing)
