
// Qt includes
#include <QDebug>

// Cjyx includes
#include <qCjyxCoreApplication.h>
#include <qCjyxModuleManager.h>

// CropVolume Logic includes
#include <vtkCjyxCLIModuleLogic.h>
#include <vtkCjyxCropVolumeLogic.h>
#include <vtkCjyxVolumesLogic.h>

// CropVolume includes
#include "qCjyxCropVolumeModule.h"
#include "qCjyxCropVolumeModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_CropVolume
class qCjyxCropVolumeModulePrivate
{
public:
  qCjyxCropVolumeModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxCropVolumeModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxCropVolumeModulePrivate::qCjyxCropVolumeModulePrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxCropVolumeModule methods

//-----------------------------------------------------------------------------
qCjyxCropVolumeModule::qCjyxCropVolumeModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxCropVolumeModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxCropVolumeModule::~qCjyxCropVolumeModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxCropVolumeModule::helpText()const
{
  return "CropVolume module extracts the subvolume of the image described "
         "by the Region of Interest widget and can also be used to resample the volume.";
}

//-----------------------------------------------------------------------------
QString qCjyxCropVolumeModule::acknowledgementText()const
{
  return "This module was developed by Andrey Fedorov and Ron Kikinis. "
         "This work was supported by NIH grants CA111288 and CA151261, "
         "NA-MIC, NAC and Cjyx community.";
}

//-----------------------------------------------------------------------------
QStringList qCjyxCropVolumeModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Andrey Fedorov (BWH, SPL)");
  moduleContributors << QString("Ron Kikinis (BWH, SPL)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxCropVolumeModule::icon()const
{
  return QIcon(":/Icons/CropVolume.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxCropVolumeModule::categories()const
{
  return QStringList() << "Converters";
}

//-----------------------------------------------------------------------------
QStringList qCjyxCropVolumeModule::dependencies()const
{
  return QStringList() << "Volumes" << "ResampleScalarVectorDWIVolume";
}

//-----------------------------------------------------------------------------
void qCjyxCropVolumeModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxCropVolumeModule::createWidgetRepresentation()
{
  return new qCjyxCropVolumeModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxCropVolumeModule::createLogic()
{
  return vtkCjyxCropVolumeLogic::New();
}

//-----------------------------------------------------------------------------
QStringList qCjyxCropVolumeModule::associatedNodeTypes() const
{
  return QStringList() << "vtkMRMLCropVolumeParametersNode";
}
