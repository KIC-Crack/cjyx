/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QSettings>

// Cjyx includes

// Slices QTModule includes
#include "qCjyxViewControllersModule.h"
#include "qCjyxViewControllersModuleWidget.h"

#include "vtkCjyxViewControllersLogic.h"

#include <vtkAddonMathUtilities.h>
#include <vtkMatrix3x3.h>
#include <vtkMRMLPlotViewNode.h>
#include <vtkMRMLSliceNode.h>
#include <vtkMRMLViewNode.h>
#include <vtkNew.h>

//-----------------------------------------------------------------------------
class qCjyxViewControllersModulePrivate
{
public:
};

//-----------------------------------------------------------------------------
qCjyxViewControllersModule::qCjyxViewControllersModule(QObject* _parent)
  :Superclass(_parent)
  , d_ptr(new qCjyxViewControllersModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxViewControllersModule::~qCjyxViewControllersModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxViewControllersModule::acknowledgementText()const
{
  return "This module was developed by Jean-Christophe Fillion-Robin, Kitware Inc. "
         "This work was supported by NIH grant 3P41RR013218-12S1, "
         "NA-MIC, NAC and Cjyx community.";
}

//-----------------------------------------------------------------------------
QStringList qCjyxViewControllersModule::categories() const
{
  return QStringList() << "";
}

//-----------------------------------------------------------------------------
QIcon qCjyxViewControllersModule::icon() const
{
  return QIcon(":Icons/ViewControllers.png");
}

//-----------------------------------------------------------------------------
QString qCjyxViewControllersModule::helpText() const
{
  QString help = "The ViewControllers module allows modifying the views options.<br>";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
void qCjyxViewControllersModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
void qCjyxViewControllersModule::setMRMLScene(vtkMRMLScene* scene)
{
  this->Superclass::setMRMLScene(scene);
  vtkCjyxViewControllersLogic* logic = vtkCjyxViewControllersLogic::SafeDownCast(this->logic());
  if (!logic)
    {
    qCritical() << Q_FUNC_INFO << " failed: logic is invalid";
    return;
    }
  // Update default view nodes from settings
  this->readDefaultSliceViewSettings(logic->GetDefaultSliceViewNode());
  this->readDefaultThreeDViewSettings(logic->GetDefaultThreeDViewNode());
  this->writeDefaultSliceViewSettings(logic->GetDefaultSliceViewNode());
  this->writeDefaultThreeDViewSettings(logic->GetDefaultThreeDViewNode());
  // Update all existing view nodes to default
  logic->ResetAllViewNodesToDefault();
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxViewControllersModule::createWidgetRepresentation()
{
  return new qCjyxViewControllersModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxViewControllersModule::createLogic()
{
  return vtkCjyxViewControllersLogic::New();
}

//-----------------------------------------------------------------------------
void qCjyxViewControllersModule::readCommonViewSettings(vtkMRMLAbstractViewNode* defaultViewNode, QSettings& settings)
{
  if (settings.contains("OrientationMarkerType"))
    {
    defaultViewNode->SetOrientationMarkerType(vtkMRMLAbstractViewNode::GetOrientationMarkerTypeFromString(
      settings.value("OrientationMarkerType").toString().toUtf8()));
    }
  if (settings.contains("OrientationMarkerSize"))
    {
    defaultViewNode->SetOrientationMarkerSize(vtkMRMLAbstractViewNode::GetOrientationMarkerSizeFromString(
      settings.value("OrientationMarkerSize").toString().toUtf8()));
    }
  if (settings.contains("RulerType"))
    {
    defaultViewNode->SetRulerType(vtkMRMLAbstractViewNode::GetRulerTypeFromString(
      settings.value("RulerType").toString().toUtf8()));
    }
}

//-----------------------------------------------------------------------------
void qCjyxViewControllersModule::writeCommonViewSettings(vtkMRMLAbstractViewNode* defaultViewNode, QSettings& settings)
{
  settings.setValue("OrientationMarkerType",vtkMRMLAbstractViewNode::GetOrientationMarkerTypeAsString(defaultViewNode->GetOrientationMarkerType()));
  settings.setValue("OrientationMarkerSize",vtkMRMLAbstractViewNode::GetOrientationMarkerSizeAsString(defaultViewNode->GetOrientationMarkerSize()));
  settings.setValue("RulerType",vtkMRMLAbstractViewNode::GetRulerTypeAsString(defaultViewNode->GetRulerType()));
}

//-----------------------------------------------------------------------------
void qCjyxViewControllersModule::readDefaultThreeDViewSettings(vtkMRMLViewNode* defaultViewNode)
{
  if (!defaultViewNode)
    {
    qCritical() << Q_FUNC_INFO << " failed: defaultViewNode is invalid";
    return;
    }
  QSettings settings;
  settings.beginGroup("Default3DView");
  if (settings.contains("BoxVisibility"))
    {
    defaultViewNode->SetBoxVisible(settings.value("BoxVisibility").toBool());
    }
  if (settings.contains("AxisLabelsVisibility"))
    {
    defaultViewNode->SetAxisLabelsVisible(settings.value("AxisLabelsVisibility").toBool());
    }
  if (settings.contains("UseOrthographicProjection"))
    {
    defaultViewNode->SetRenderMode(settings.value("UseOrthographicProjection").toBool() ? vtkMRMLViewNode::Orthographic : vtkMRMLViewNode::Perspective);
    }
  if (settings.contains("UseDepthPeeling"))
    {
    defaultViewNode->SetUseDepthPeeling(settings.value("UseDepthPeeling").toBool());
    }
  readCommonViewSettings(defaultViewNode, settings);
}

//-----------------------------------------------------------------------------
void qCjyxViewControllersModule::writeDefaultThreeDViewSettings(vtkMRMLViewNode* defaultViewNode)
{
  if (!defaultViewNode)
    {
    qCritical() << Q_FUNC_INFO << " failed: defaultViewNode is invalid";
    return;
    }
  QSettings settings;
  settings.beginGroup("Default3DView");
  settings.setValue("BoxVisibility", bool(defaultViewNode->GetBoxVisible()));
  settings.setValue("AxisLabelsVisibility", bool(defaultViewNode->GetAxisLabelsVisible()));
  settings.setValue("UseOrthographicProjection", defaultViewNode->GetRenderMode()==vtkMRMLViewNode::Orthographic);
  settings.setValue("UseDepthPeeling", bool(defaultViewNode->GetUseDepthPeeling()));
  writeCommonViewSettings(defaultViewNode, settings);
}

//-----------------------------------------------------------------------------
void qCjyxViewControllersModule::readDefaultSliceViewSettings(vtkMRMLSliceNode* defaultViewNode)
{
  if (!defaultViewNode)
    {
    qCritical() << Q_FUNC_INFO << " failed: defaultViewNode is invalid";
    return;
    }
  QSettings settings;
  settings.beginGroup("DefaultSliceView");
  if (settings.contains("Orientation"))
    {
    QString defaultSliceOrientation = settings.value("Orientation").toString();
    if (defaultSliceOrientation == "PatientRightIsScreenLeft")
      {
      vtkMRMLSliceNode::AddDefaultSliceOrientationPresets(this->mrmlScene(), true);
      }
    else if (defaultSliceOrientation == "PatientRightIsScreenRight")
      {
      vtkMRMLSliceNode::AddDefaultSliceOrientationPresets(this->mrmlScene(), false);
      }
    else
      {
      qWarning() << Q_FUNC_INFO << ": Unknown DefaultSliceView/Orientation setting " << defaultSliceOrientation << ", using PatientRightIsScreenLeft instead.";
      vtkMRMLSliceNode::AddDefaultSliceOrientationPresets(this->mrmlScene(), true);
      }
    }
  readCommonViewSettings(defaultViewNode, settings);
}

//-----------------------------------------------------------------------------
void qCjyxViewControllersModule::writeDefaultSliceViewSettings(vtkMRMLSliceNode* defaultViewNode)
{
  if (!defaultViewNode)
    {
    qCritical() << Q_FUNC_INFO << " failed: defaultViewNode is invalid";
    return;
    }
  QSettings settings;
  settings.beginGroup("DefaultSliceView");

  vtkNew<vtkMatrix3x3> axialOrientationForPatientRightIsScreenRight;
  vtkMRMLSliceNode::GetAxialSliceToRASMatrix(axialOrientationForPatientRightIsScreenRight, false);
  vtkMatrix3x3* axialOrientation = defaultViewNode->GetSliceOrientationPreset("Axial");
  QString defaultSliceOrientation("PatientRightIsScreenLeft");
  if (vtkAddonMathUtilities::MatrixAreEqual(axialOrientation, axialOrientationForPatientRightIsScreenRight))
    {
    defaultSliceOrientation = "PatientRightIsScreenRight";
    }
  settings.setValue("Orientation", defaultSliceOrientation);

  writeCommonViewSettings(defaultViewNode, settings);
}

//-----------------------------------------------------------------------------
void qCjyxViewControllersModule::readDefaultPlotViewSettings(vtkMRMLPlotViewNode* defaultViewNode)
{
  if (!defaultViewNode)
    {
    qCritical() << Q_FUNC_INFO << " failed: defaultViewNode is invalid";
    return;
    }
  QSettings settings;
  settings.beginGroup("DefaultPlotView");
  readCommonViewSettings(defaultViewNode, settings);
}

//-----------------------------------------------------------------------------
void qCjyxViewControllersModule::writeDefaultPlotViewSettings(vtkMRMLPlotViewNode* defaultViewNode)
{
  if (!defaultViewNode)
    {
    qCritical() << Q_FUNC_INFO << " failed: defaultViewNode is invalid";
    return;
    }
  QSettings settings;
  settings.beginGroup("DefaultPlotView");
  writeCommonViewSettings(defaultViewNode, settings);
}

//-----------------------------------------------------------------------------
QStringList qCjyxViewControllersModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Wendy Plesniak (SPL, BWH)");
  moduleContributors << QString("Jim Miller (GE)");
  moduleContributors << QString("Steve Pieper (Isomics)");
  moduleContributors << QString("Ron Kikinis (SPL, BWH)");
  moduleContributors << QString("Jean-Christophe Fillion-Robin (Kitware)");
  return moduleContributors;
}
