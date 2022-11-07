project(qCjyxModulesCore)

#
# See CMake/CjyxMacroBuildBaseQtLibrary.cmake for details
#
set(KIT_export_directive "Q_CJYX_MODULES_CORE_EXPORT")

# Additional directories to include
set(KIT_include_directories
  ${qCjyxBaseQTGUI_SOURCE_DIR}
  ${qCjyxBaseQTGUI_BINARY_DIR}
  ${MRMLLogic_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/EventBroker
  )

# Source files
set(KIT_SRCS
  qCjyxCoreModule.cxx
  qCjyxCoreModule.h

  qCjyxCoreModuleFactory.cxx
  qCjyxCoreModuleFactory.h

  EventBroker/qCjyxEventBrokerModule.cxx
  EventBroker/qCjyxEventBrokerModule.h
  EventBroker/qCjyxEventBrokerModuleWidget.cxx
  EventBroker/qCjyxEventBrokerModuleWidget.h
  )

# Headers that should run through moc
set(KIT_MOC_SRCS
  qCjyxCoreModule.h

  EventBroker/qCjyxEventBrokerModule.h
  EventBroker/qCjyxEventBrokerModuleWidget.h
  )

# UI files
set(KIT_UI_SRCS
  Resources/UI/qCjyxEventBrokerModuleWidget.ui
  )

# Additional Target libraries (Qt libs already included)
set(KIT_target_libraries
  # ${QT_LIBRARIES} # Not needed: All dependencies are transitively satisfied by other targets
  qCjyxBaseQTGUI
  )

# Resources
set(KIT_resources
  )

CjyxMacroBuildBaseQtLibrary(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${KIT_export_directive}
  INCLUDE_DIRECTORIES ${KIT_include_directories}
  SRCS ${KIT_SRCS}
  MOC_SRCS ${KIT_MOC_SRCS}
  UI_SRCS ${KIT_UI_SRCS}
  TARGET_LIBRARIES ${KIT_target_libraries}
  RESOURCES ${KIT_resources}
  )

# Plugin
#add_subdirectory(Plugins)

# Testing
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
