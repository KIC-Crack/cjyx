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

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QLineEdit>

// QtGUI includes
#include "qCjyxApplication.h"
#include "qCjyxVolumeRenderingSettingsPanel.h"
#include "ui_qCjyxVolumeRenderingSettingsPanel.h"

// MRMLDisplayableManager includes
#include <vtkMRMLVolumeRenderingDisplayableManager.h>

// VolumeRendering Logic includes
#include <vtkCjyxVolumeRenderingLogic.h>

// VTK includes
#include <vtkNew.h>
#include <vtkSmartPointer.h>

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLViewNode.h>

// --------------------------------------------------------------------------
// qCjyxVolumeRenderingSettingsPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxVolumeRenderingSettingsPanelPrivate: public Ui_qCjyxVolumeRenderingSettingsPanel
{
  Q_DECLARE_PUBLIC(qCjyxVolumeRenderingSettingsPanel);
protected:
  qCjyxVolumeRenderingSettingsPanel* const q_ptr;

public:
  qCjyxVolumeRenderingSettingsPanelPrivate(qCjyxVolumeRenderingSettingsPanel& object);
  void init();

  void addRenderingMethod(const QString& methodName, const QString& methodClassName);

  vtkMRMLScene* mrmlScene();
  vtkMRMLViewNode* defaultMrmlViewNode();

  vtkSmartPointer<vtkCjyxVolumeRenderingLogic> VolumeRenderingLogic;
};

// --------------------------------------------------------------------------
// qCjyxVolumeRenderingSettingsPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxVolumeRenderingSettingsPanelPrivate
::qCjyxVolumeRenderingSettingsPanelPrivate(qCjyxVolumeRenderingSettingsPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanelPrivate::init()
{
  Q_Q(qCjyxVolumeRenderingSettingsPanel);

  this->setupUi(q);

  //
  // Quality
  //
  for (int qualityIndex=0; qualityIndex<vtkMRMLViewNode::VolumeRenderingQuality_Last; qualityIndex++)
    {
    this->QualityControlComboBox->addItem(vtkMRMLViewNode::GetVolumeRenderingQualityAsString(qualityIndex));
    }
  this->QualityControlComboBox->setCurrentText(vtkMRMLViewNode::GetVolumeRenderingQualityAsString(vtkMRMLViewNode::Normal));
  QObject::connect(this->QualityControlComboBox, SIGNAL(currentIndexChanged(int)),
                   q, SLOT(onDefaultQualityChanged(int)));
  q->registerProperty("VolumeRendering/DefaultQuality", q,
                      "defaultQuality", SIGNAL(defaultQualityChanged(QString)));

  //
  // Interactive speed
  //
  QObject::connect(this->InteractiveSpeedSlider, SIGNAL(valueChanged(double)),
                   q, SLOT(onDefaultInteractiveSpeedChanged(double)));
  q->registerProperty("VolumeRendering/DefaultInteractiveSpeed", q,
                      "defaultInteractiveSpeed", SIGNAL(defaultInteractiveSpeedChanged(int)));

  //
  // Surface smoothing
  //
  QObject::connect(this->SurfaceSmoothingCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(onDefaultSurfaceSmoothingChanged(bool)));
  q->registerProperty("VolumeRendering/DefaultSurfaceSmoothing", q,
                      "defaultSurfaceSmoothing", SIGNAL(defaultSurfaceSmoothingChanged(bool)));

  //
  // Auto-release graphics resources
  //
  QObject::connect(this->AutoReleaseGraphicsResourcesCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(onDefaultAutoReleaseGraphicsResourcesChanged(bool)));
  q->registerProperty("VolumeRendering/DefaultAutoReleaseGraphicsResources", q,
                      "defaultAutoReleaseGraphicsResources", SIGNAL(defaultAutoReleaseGraphicsResourcesChanged(bool)));

  //
  // GPU memory
  //

  // Currently, VTK ignores GPU memory size request - hide it on the GUI to not confuse users
  this->GPUMemoryLabel->hide();
  this->GPUMemoryComboBox->hide();

  QObject::connect(this->GPUMemoryComboBox, SIGNAL(editTextChanged(QString)),
                   q, SLOT(onGPUMemoryChanged()));
  QObject::connect(this->GPUMemoryComboBox, SIGNAL(currentTextChanged(QString)),
                   q, SLOT(onGPUMemoryChanged()));

  q->registerProperty("VolumeRendering/GPUMemorySize", q,
                      "gpuMemory", SIGNAL(gpuMemoryChanged(QString)));

  // Update default view node from settings when startup completed.
  // MRML scene is not accessible yet from the logic when it is set, so cannot access default view node
  // either. Need to setup default node and set defaults to 3D views when the scene is available.
  QObject::connect(qCjyxApplication::application(), SIGNAL(startupCompleted()),
                   q, SLOT(updateDefaultViewNodeFromWidget()));
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanelPrivate::addRenderingMethod(
  const QString& methodName, const QString& methodClassName )
{
  this->RenderingMethodComboBox->addItem(methodName, methodClassName);
}

// --------------------------------------------------------------------------
vtkMRMLScene* qCjyxVolumeRenderingSettingsPanelPrivate::mrmlScene()
{
  Q_Q(qCjyxVolumeRenderingSettingsPanel);

  vtkCjyxVolumeRenderingLogic* logic = q->volumeRenderingLogic();
  if (!logic)
    {
    return nullptr;
    }
  return logic->GetMRMLScene();
}

// --------------------------------------------------------------------------
vtkMRMLViewNode* qCjyxVolumeRenderingSettingsPanelPrivate::defaultMrmlViewNode()
{
  vtkMRMLScene* scene = this->mrmlScene();
  if (!scene)
    {
    return nullptr;
    }

  // Setup a default 3D view node so that the default settings are propagated to all new 3D views
  vtkSmartPointer<vtkMRMLNode> defaultNode = scene->GetDefaultNodeByClass("vtkMRMLViewNode");
  if (!defaultNode)
    {
    defaultNode.TakeReference(scene->CreateNodeByClass("vtkMRMLViewNode"));
    scene->AddDefaultNode(defaultNode);
    }
  return vtkMRMLViewNode::SafeDownCast(defaultNode.GetPointer());
}

// --------------------------------------------------------------------------
// qCjyxVolumeRenderingSettingsPanel methods

// --------------------------------------------------------------------------
qCjyxVolumeRenderingSettingsPanel::qCjyxVolumeRenderingSettingsPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxVolumeRenderingSettingsPanelPrivate(*this))
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxVolumeRenderingSettingsPanel::~qCjyxVolumeRenderingSettingsPanel() = default;

// --------------------------------------------------------------------------
vtkCjyxVolumeRenderingLogic* qCjyxVolumeRenderingSettingsPanel::volumeRenderingLogic()const
{
  Q_D(const qCjyxVolumeRenderingSettingsPanel);
  return d->VolumeRenderingLogic;
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::setVolumeRenderingLogic(vtkCjyxVolumeRenderingLogic* logic)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);

  qvtkReconnect(d->VolumeRenderingLogic, logic, vtkCommand::ModifiedEvent,
                this, SLOT(onVolumeRenderingLogicModified()));
  d->VolumeRenderingLogic = logic;

  this->onVolumeRenderingLogicModified();

  this->registerProperty("VolumeRendering/RenderingMethod", this,
                         "defaultRenderingMethod", SIGNAL(defaultRenderingMethodChanged(QString)));
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::onVolumeRenderingLogicModified()
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);

  // Update default rendering method
  const std::map<std::string, std::string>& renderingMethods =
    d->VolumeRenderingLogic->GetRenderingMethods();
  /// \todo not the best test to make sure the list is different
  if (static_cast<int>(renderingMethods.size()) != d->RenderingMethodComboBox->count())
    {
    std::map<std::string, std::string>::const_iterator it;
    for (it = renderingMethods.begin(); it != renderingMethods.end(); ++it)
      {
      d->addRenderingMethod(it->first.c_str(), it->second.c_str());
      }
    }
  QObject::connect(d->RenderingMethodComboBox, SIGNAL(currentIndexChanged(int)),
                   this, SLOT(onDefaultRenderingMethodChanged(int)),Qt::UniqueConnection);

  const char* defaultRenderingMethod = d->VolumeRenderingLogic->GetDefaultRenderingMethod();
  if (defaultRenderingMethod == nullptr)
    {
    defaultRenderingMethod = "vtkMRMLGPURayCastVolumeRenderingDisplayNode";
    }
  int defaultRenderingMethodIndex = d->RenderingMethodComboBox->findData(
    QString(defaultRenderingMethod));
  d->RenderingMethodComboBox->setCurrentIndex(defaultRenderingMethodIndex);
}

// --------------------------------------------------------------------------
QString qCjyxVolumeRenderingSettingsPanel::gpuMemory()const
{
  Q_D(const qCjyxVolumeRenderingSettingsPanel);
  return d->GPUMemoryComboBox->currentGPUMemoryAsString();
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::setGPUMemory(const QString& gpuMemoryString)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  d->GPUMemoryComboBox->setCurrentGPUMemoryFromString(gpuMemoryString);
  this->onGPUMemoryChanged();
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::onGPUMemoryChanged()
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  if (!d->mrmlScene())
    {
    return;
    }

  int memory = d->GPUMemoryComboBox->currentGPUMemoryInMB();

  // Set to default view node
  vtkMRMLViewNode* defaultViewNode = d->defaultMrmlViewNode();
  if (defaultViewNode)
    {
    defaultViewNode->SetGPUMemorySize(memory);
    }

  // Set to all existing view nodes
  std::vector<vtkMRMLNode*> viewNodes;
  d->mrmlScene()->GetNodesByClass("vtkMRMLViewNode", viewNodes);
  for (std::vector<vtkMRMLNode*>::iterator it=viewNodes.begin(); it!=viewNodes.end(); ++it)
    {
    vtkMRMLViewNode* viewNode = vtkMRMLViewNode::SafeDownCast(*it);
    viewNode->SetGPUMemorySize(memory);
    }

  emit gpuMemoryChanged(this->gpuMemory());
}

// --------------------------------------------------------------------------
QString qCjyxVolumeRenderingSettingsPanel::defaultRenderingMethod()const
{
  Q_D(const qCjyxVolumeRenderingSettingsPanel);
  QString renderingClassName =
    d->RenderingMethodComboBox->itemData(d->RenderingMethodComboBox->currentIndex()).toString();
  return renderingClassName;
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::setDefaultRenderingMethod(const QString& method)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  int methodIndex = d->RenderingMethodComboBox->findData(method);
  d->RenderingMethodComboBox->setCurrentIndex(methodIndex);
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::onDefaultRenderingMethodChanged(int index)
{
  Q_UNUSED(index);
  this->updateVolumeRenderingLogicDefaultRenderingMethod();
  emit defaultRenderingMethodChanged(this->defaultRenderingMethod());
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::updateVolumeRenderingLogicDefaultRenderingMethod()
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  if (d->VolumeRenderingLogic == nullptr)
    {
    return;
    }
  d->VolumeRenderingLogic->SetDefaultRenderingMethod(this->defaultRenderingMethod().toUtf8());
}

// --------------------------------------------------------------------------
QString qCjyxVolumeRenderingSettingsPanel::defaultQuality()const
{
  Q_D(const qCjyxVolumeRenderingSettingsPanel);
  int qualityIndex = d->QualityControlComboBox->currentIndex();
  QString quality(vtkMRMLViewNode::GetVolumeRenderingQualityAsString(qualityIndex));
  return quality;
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::setDefaultQuality(const QString& quality)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  int qualityIndex = d->QualityControlComboBox->findText(quality);
  d->QualityControlComboBox->setCurrentIndex(qualityIndex);
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::onDefaultQualityChanged(int qualityIndex)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  if (!d->mrmlScene())
    {
    return;
    }

  // Set to default view node
  vtkMRMLViewNode* defaultViewNode = d->defaultMrmlViewNode();
  if (defaultViewNode)
    {
    defaultViewNode->SetVolumeRenderingQuality(qualityIndex);
    }

  // Set to all existing view nodes
  std::vector<vtkMRMLNode*> viewNodes;
  d->mrmlScene()->GetNodesByClass("vtkMRMLViewNode", viewNodes);
  for (std::vector<vtkMRMLNode*>::iterator it=viewNodes.begin(); it!=viewNodes.end(); ++it)
    {
    vtkMRMLViewNode* viewNode = vtkMRMLViewNode::SafeDownCast(*it);
    viewNode->SetVolumeRenderingQuality(qualityIndex);
    }

  QString quality(vtkMRMLViewNode::GetVolumeRenderingQualityAsString(qualityIndex));
  emit defaultQualityChanged(quality);
}

// --------------------------------------------------------------------------
int qCjyxVolumeRenderingSettingsPanel::defaultInteractiveSpeed()const
{
  Q_D(const qCjyxVolumeRenderingSettingsPanel);
  int interactiveSpeed = d->InteractiveSpeedSlider->value();
  return interactiveSpeed;
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::setDefaultInteractiveSpeed(int interactiveSpeed)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  d->InteractiveSpeedSlider->setValue(interactiveSpeed);
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::onDefaultInteractiveSpeedChanged(double interactiveSpeed)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  if (!d->mrmlScene())
    {
    return;
    }

  // Set to default view node
  vtkMRMLViewNode* defaultViewNode = d->defaultMrmlViewNode();
  if (defaultViewNode)
    {
    defaultViewNode->SetExpectedFPS((int)interactiveSpeed);
    }

  // Set to all existing view nodes
  std::vector<vtkMRMLNode*> viewNodes;
  d->mrmlScene()->GetNodesByClass("vtkMRMLViewNode", viewNodes);
  for (std::vector<vtkMRMLNode*>::iterator it=viewNodes.begin(); it!=viewNodes.end(); ++it)
    {
    vtkMRMLViewNode* viewNode = vtkMRMLViewNode::SafeDownCast(*it);
    viewNode->SetExpectedFPS((int)interactiveSpeed);
    }

  emit defaultInteractiveSpeedChanged((int)interactiveSpeed);
}

// --------------------------------------------------------------------------
bool qCjyxVolumeRenderingSettingsPanel::defaultSurfaceSmoothing()const
{
  Q_D(const qCjyxVolumeRenderingSettingsPanel);
  bool smoothing = d->SurfaceSmoothingCheckBox->isChecked();
  return smoothing;
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::setDefaultSurfaceSmoothing(bool surfaceSmoothing)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  d->SurfaceSmoothingCheckBox->setChecked(surfaceSmoothing);
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::onDefaultSurfaceSmoothingChanged(bool smoothing)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  if (!d->mrmlScene())
    {
    return;
    }

  // Set to default view node
  vtkMRMLViewNode* defaultViewNode = d->defaultMrmlViewNode();
  if (defaultViewNode)
    {
    defaultViewNode->SetVolumeRenderingSurfaceSmoothing(smoothing);
    }

  // Set to all existing view nodes
  std::vector<vtkMRMLNode*> viewNodes;
  d->mrmlScene()->GetNodesByClass("vtkMRMLViewNode", viewNodes);
  for (std::vector<vtkMRMLNode*>::iterator it=viewNodes.begin(); it!=viewNodes.end(); ++it)
    {
    vtkMRMLViewNode* viewNode = vtkMRMLViewNode::SafeDownCast(*it);
    viewNode->SetVolumeRenderingSurfaceSmoothing(smoothing);
    }

  emit defaultSurfaceSmoothingChanged(smoothing);
}

// --------------------------------------------------------------------------
bool qCjyxVolumeRenderingSettingsPanel::defaultAutoReleaseGraphicsResources()const
{
  Q_D(const qCjyxVolumeRenderingSettingsPanel);
  bool autoRelease = d->AutoReleaseGraphicsResourcesCheckBox->isChecked();
  return autoRelease;
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::setDefaultAutoReleaseGraphicsResources(bool autoRelease)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  d->AutoReleaseGraphicsResourcesCheckBox->setChecked(autoRelease);
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::onDefaultAutoReleaseGraphicsResourcesChanged(bool autoRelease)
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);
  if (!d->mrmlScene())
    {
    return;
    }

  // Set to default view node
  vtkMRMLViewNode* defaultViewNode = d->defaultMrmlViewNode();
  if (defaultViewNode)
    {
    defaultViewNode->SetAutoReleaseGraphicsResources(autoRelease);
    }

  // Set to all existing view nodes
  std::vector<vtkMRMLNode*> viewNodes;
  d->mrmlScene()->GetNodesByClass("vtkMRMLViewNode", viewNodes);
  for (std::vector<vtkMRMLNode*>::iterator it=viewNodes.begin(); it!=viewNodes.end(); ++it)
    {
    vtkMRMLViewNode* viewNode = vtkMRMLViewNode::SafeDownCast(*it);
    viewNode->SetAutoReleaseGraphicsResources(autoRelease);
    }

  emit defaultAutoReleaseGraphicsResourcesChanged(autoRelease);
}

// --------------------------------------------------------------------------
void qCjyxVolumeRenderingSettingsPanel::updateDefaultViewNodeFromWidget()
{
  Q_D(qCjyxVolumeRenderingSettingsPanel);

  this->onDefaultQualityChanged(d->QualityControlComboBox->currentIndex());
  this->onDefaultInteractiveSpeedChanged(d->InteractiveSpeedSlider->value());
  this->onDefaultSurfaceSmoothingChanged(d->SurfaceSmoothingCheckBox->isChecked());
  this->onDefaultAutoReleaseGraphicsResourcesChanged(d->AutoReleaseGraphicsResourcesCheckBox->isChecked());
  this->onGPUMemoryChanged();
}
