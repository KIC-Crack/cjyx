
#-----------------------------------------------------------------------------
set(MODULE_NAME DICOMPlugins)

#-----------------------------------------------------------------------------
set(MODULE_PYTHON_SCRIPTS
  DICOMEnhancedUSVolumePlugin.py
  DICOMGeAbusPlugin.py
  DICOMImageSequencePlugin.py
  DICOMScalarVolumePlugin.py
  DICOMVolumeSequencePlugin.py
  DICOMCjyxDataBundlePlugin.py
  )

set(MODULE_PYTHON_RESOURCES
  )

#-----------------------------------------------------------------------------
cjyxMacroBuildScriptedModule(
  NAME ${MODULE_NAME}
  SCRIPTS ${MODULE_PYTHON_SCRIPTS}
  RESOURCES ${MODULE_PYTHON_RESOURCES}
  )
