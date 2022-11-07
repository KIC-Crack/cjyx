if(BUILD_TESTING)
  # Code specific to Cjyx testing is added here.
  set(KIT ${PROJECT_NAME})




  #-----------------------------------------------------------------------------
  set(${KIT}Testing_ITK_COMPONENTS
    ITKCommon
    )
  find_package(ITK 4.6 COMPONENTS ${${KIT}Testing_ITK_COMPONENTS} REQUIRED)
  if(ITK_VERSION VERSION_GREATER_EQUAL "5.3")
    foreach(factory_uc IN ITEMS "IMAGEIO" "MESHIO" "TRANSFORMIO")
      set(ITK_NO_${factory_uc}_FACTORY_REGISTER_MANAGER 1)
    endforeach()
  else()
    set(ITK_NO_IO_FACTORY_REGISTER_MANAGER 1) # See Libs/ITKFactoryRegistration/CMakeLists.txt
  endif()
  list(APPEND ITK_LIBRARIES ITKFactoryRegistration)
  list(APPEND ITK_INCLUDE_DIRS
    ${ITKFactoryRegistration_INCLUDE_DIRS}
    )
  include(${ITK_USE_FILE})

  #-----------------------------------------------------------------------------
  set(CMAKE_TESTDRIVER_BEFORE_TESTMAIN  "
    DEBUG_LEAKS_ENABLE_EXIT_ERROR();
    TESTING_OUTPUT_INIT();
    itk::itkFactoryRegistration();
    ")

  set(CMAKE_TESTDRIVER_AFTER_TESTMAIN  "
    TESTING_OUTPUT_ASSERT_WARNINGS_ERRORS(0);
    ")

  set(EXTRA_INCLUDE "vtkMRMLDebugLeaksMacro.h\"\n\#include <itkConfigure.h>\n\#include <itkFactoryRegistration.h>\n\#include \"vtkTestingOutputWindow.h")

  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  set(KIT_TEST_SRCS
    qCjyxAbstractModuleTest1.cxx
    qCjyxAbstractModuleWidgetTest1.cxx
    qCjyxApplicationTest1.cxx
    qCjyxDataDialogTest1.cxx
    qCjyxDirectoryListViewTest1.cxx
    qCjyxLayoutManagerTest1.cxx
    qCjyxModulePanelTest1.cxx
    qCjyxMouseModeToolBarTest1.cxx
    qCjyxSaveDataDialogCustomFileWriterTest.cxx
    qCjyxWidgetTest1.cxx
    qCjyxWidgetTest2.cxx
    )
  if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
    list(APPEND KIT_TEST_SRCS
      )
  endif()
  if(Cjyx_USE_PYTHONQT)
    list(APPEND KIT_TEST_SRCS
      qCjyxLoadableModuleWithPythonTest.cxx
      qCjyxPythonManagerWithoutApplicationTest.cxx
      qCjyxScriptedLoadableModuleTest.cxx
      qCjyxScriptedLoadableModuleWidgetTest.cxx
      )
  endif()
  create_test_sourcelist(Tests ${KIT}CxxTests.cxx
    ${KIT_TEST_SRCS}
    EXTRA_INCLUDE ${EXTRA_INCLUDE}
    )

  set(KIT_TEST_GENERATE_MOC_SRCS
    )
  if(Cjyx_USE_PYTHONQT)
    list(APPEND KIT_TEST_GENERATE_MOC_SRCS
      qCjyxLoadableModuleWithPythonTest.cxx
      qCjyxPythonManagerWithoutApplicationTest.cxx
      qCjyxScriptedLoadableModuleTest.cxx
      qCjyxScriptedLoadableModuleWidgetTest.cxx
      )
  endif()

    if(KIT_TEST_GENERATE_MOC_SRCS)
      QT5_GENERATE_MOCS(${KIT_TEST_GENERATE_MOC_SRCS})
    endif()
    QT5_ADD_RESOURCES(Tests Resources/${KIT}Testing.qrc)

  ctk_add_executable_utf8(${KIT}CxxTests ${Tests})
  target_link_libraries(${KIT}CxxTests ${KIT})
  set_target_properties(${KIT}CxxTests PROPERTIES LABELS ${KIT})
  set_target_properties(${KIT}CxxTests PROPERTIES FOLDER "Core-Base")

  #
  # Add Tests
  #

  simple_test( qCjyxApplicationTest1 )
  simple_test( qCjyxAbstractModuleTest1 )
  simple_test( qCjyxAbstractModuleWidgetTest1 )
  simple_test( qCjyxDataDialogTest1 )
  simple_test( qCjyxDirectoryListViewTest1 )
  simple_test( qCjyxLayoutManagerTest1 )
  simple_test( qCjyxModulePanelTest1 )
  simple_test( qCjyxMouseModeToolBarTest1 )
  simple_test( qCjyxSaveDataDialogCustomFileWriterTest )
  simple_test( qCjyxWidgetTest1 )
  simple_test( qCjyxWidgetTest2 DATA{${CMAKE_SOURCE_DIR}/Libs/MRML/Core/Testing/TestData/fixed.nrrd} )

  if(Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT)
  endif()

  if(Cjyx_USE_PYTHONQT)
    simple_test( qCjyxLoadableModuleWithPythonTest )
    simple_test( qCjyxPythonManagerWithoutApplicationTest )
    simple_test( qCjyxScriptedLoadableModuleTest )
    simple_test( qCjyxScriptedLoadableModuleWidgetTest )
  endif()

endif()

#
# Test templates - Common to both Cjyx and the extensions
#

set(Cjyx_CXX_MODULE_TEST_TEMPLATES_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  CACHE INTERNAL "Path to cxx test templates directory.")

