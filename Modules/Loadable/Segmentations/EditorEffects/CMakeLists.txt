project(qCjyx${MODULE_NAME}EditorEffects)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "Q_CJYX_${MODULE_NAME_UPPER}_EFFECTS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkCjyxSegmentationsModuleMRML_INCLUDE_DIRS}
  ${vtkCjyxSegmentationsModuleMRMLDisplayableManager_INCLUDE_DIRS}
  ${vtkCjyxSegmentationsModuleLogic_INCLUDE_DIRS}
  ${Cjyx_Base_INCLUDE_DIRS}
  ${qMRMLWidgets_INCLUDE_DIRS}
  ${MRMLLogic_INCLUDE_DIRS}
  ${MRMLCore_INCLUDE_DIRS}
  ${MRMLDisplayableManager_INCLUDE_DIRS}
  )

set(${KIT}_SRCS
  qCjyxSegmentEditorEffectFactory.cxx
  qCjyxSegmentEditorEffectFactory.h
  qCjyxSegmentEditorAbstractEffect.cxx
  qCjyxSegmentEditorAbstractEffect.h
  qCjyxSegmentEditorAbstractEffect_p.h
  qCjyxSegmentEditorAbstractLabelEffect.cxx
  qCjyxSegmentEditorAbstractLabelEffect.h
  qCjyxSegmentEditorAbstractLabelEffect_p.h
  qCjyxSegmentEditorPaintEffect.cxx
  qCjyxSegmentEditorPaintEffect.h
  qCjyxSegmentEditorPaintEffect_p.h
  qCjyxSegmentEditorEraseEffect.cxx
  qCjyxSegmentEditorEraseEffect.h
  qCjyxSegmentEditorScissorsEffect.cxx
  qCjyxSegmentEditorScissorsEffect.h
  )
if(Cjyx_USE_PYTHONQT)
  list(APPEND ${KIT}_SRCS
    qCjyxSegmentEditorScriptedEffect.cxx
    qCjyxSegmentEditorScriptedEffect.h
    qCjyxSegmentEditorScriptedLabelEffect.cxx
    qCjyxSegmentEditorScriptedLabelEffect.h
    qCjyxSegmentEditorScriptedPaintEffect.cxx
    qCjyxSegmentEditorScriptedPaintEffect.h
    )
endif()

set(${KIT}_MOC_SRCS
  qCjyxSegmentEditorEffectFactory.h
  qCjyxSegmentEditorAbstractEffect.h
  qCjyxSegmentEditorAbstractEffect_p.h
  qCjyxSegmentEditorAbstractLabelEffect.h
  qCjyxSegmentEditorAbstractLabelEffect_p.h
  qCjyxSegmentEditorPaintEffect.h
  qCjyxSegmentEditorPaintEffect_p.h
  qCjyxSegmentEditorEraseEffect.h
  qCjyxSegmentEditorScissorsEffect.h
  )
if(Cjyx_USE_PYTHONQT)
  list(APPEND ${KIT}_MOC_SRCS
    qCjyxSegmentEditorScriptedEffect.h
    qCjyxSegmentEditorScriptedLabelEffect.h
    qCjyxSegmentEditorScriptedPaintEffect.h
    )
endif()

set(${KIT}_UI_SRCS
  )

set(${KIT}_RESOURCES
  Resources/${KIT}.qrc
  )

#-----------------------------------------------------------------------------
set(${KIT}_TARGET_LIBRARIES
  vtkCjyxSegmentationsModuleMRML
  vtkCjyxSegmentationsModuleMRMLDisplayableManager
  vtkCjyxSegmentationsModuleLogic
  qMRMLWidgets
  MRMLLogic
  MRMLCore
  qCjyxBaseQTGUI # required for access to qCjyxApplication singletons
  )

#-----------------------------------------------------------------------------
CjyxMacroBuildModuleQtLibrary(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  FOLDER "Module-${MODULE_NAME}"
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  UI_SRCS ${${KIT}_UI_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  RESOURCES ${${KIT}_RESOURCES}
  WRAP_PYTHONQT
  )

#-----------------------------------------------------------------------------
if(Cjyx_USE_PYTHONQT)
  add_subdirectory(Python)
endif()
