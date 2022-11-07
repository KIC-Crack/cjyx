/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// SubjectHierarchy MRML includes
#include "vtkMRMLSubjectHierarchyNode.h"
#include "vtkMRMLSubjectHierarchyConstants.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin.h"
#include "qCjyxSubjectHierarchyDefaultPlugin.h"
#include "qCjyxSubjectHierarchyVolumesPlugin.h"

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "vtkCjyxApplicationLogic.h"

// MRML includes
#include <vtkMRMLDiffusionTensorVolumeNode.h>
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkCollection.h>

// Qt includes
#include <QDebug>
#include <QStandardItem>
#include <QAction>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxModuleManager.h"
#include "qCjyxAbstractCoreModule.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Plugins
class qCjyxSubjectHierarchyDiffusionTensorVolumesPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin);
protected:
  qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyDiffusionTensorVolumesPluginPrivate(qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin& object);
  ~qCjyxSubjectHierarchyDiffusionTensorVolumesPluginPrivate() override;
  void init();
public:
  QIcon DiffusionTensorVolumeIcon;

  QAction* TractographyLabelMapSeedingAction;
  QAction* TractographyInteractiveSeedingAction;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyDiffusionTensorVolumesPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyDiffusionTensorVolumesPluginPrivate::qCjyxSubjectHierarchyDiffusionTensorVolumesPluginPrivate(qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin& object)
: q_ptr(&object)
{
  this->DiffusionTensorVolumeIcon = QIcon(":Icons/DiffusionTensorVolume.png");

  this->TractographyLabelMapSeedingAction = nullptr;
  this->TractographyInteractiveSeedingAction = nullptr;
}

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyDiffusionTensorVolumesPluginPrivate::init()
{
  Q_Q(qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin);

  this->TractographyLabelMapSeedingAction = new QAction("Tractography labelmap seeding...",q);
  QObject::connect(this->TractographyLabelMapSeedingAction, SIGNAL(triggered()), q, SLOT(onTractographyLabelMapSeeding()));
  this->TractographyInteractiveSeedingAction = new QAction("Tractography interactive seeding...",q);
  QObject::connect(this->TractographyInteractiveSeedingAction, SIGNAL(triggered()), q, SLOT(onTractographyInteractiveSeeding()));
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyDiffusionTensorVolumesPluginPrivate::~qCjyxSubjectHierarchyDiffusionTensorVolumesPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyDiffusionTensorVolumesPluginPrivate(*this) )
{
  this->m_Name = QString("DiffusionTensorVolumes");

  Q_D(qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::~qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin() = default;

//----------------------------------------------------------------------------
double qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::canAddNodeToSubjectHierarchy(
  vtkMRMLNode* node, vtkIdType parentItemID/*=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID*/)const
{
  Q_UNUSED(parentItemID);
  if (!node)
    {
    qCritical() << Q_FUNC_INFO << ": Input node is nullptr!";
    return 0.0;
    }
  else if (node->IsA("vtkMRMLDiffusionTensorVolumeNode"))
    {
    // Node is a DTI
    return 0.7;
    }
  return 0.0;
}

//---------------------------------------------------------------------------
double qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return 0.0;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return 0.0;
    }

  // DTI volume
  vtkMRMLNode* associatedNode = shNode->GetItemDataNode(itemID);
  if (associatedNode && associatedNode->IsA("vtkMRMLDiffusionTensorVolumeNode"))
    {
    return 0.7;
    }

  return 0.0;
}

//---------------------------------------------------------------------------
const QString qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::roleForPlugin()const
{
  return "DTI volume";
}

//-----------------------------------------------------------------------------
QString qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::tooltip(vtkIdType itemID)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QString("Invalid!");
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return QString("Error!");
    }

  // Get basic tooltip from volumes plugin
  QString tooltipString = qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes")->tooltip(itemID);

  // Additional DTI-related information
  vtkMRMLDiffusionTensorVolumeNode* dtiVolumeNode = vtkMRMLDiffusionTensorVolumeNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  Q_UNUSED(dtiVolumeNode);
  //vtkImageData* imageData = (dtiVolumeNode ? dtiVolumeNode->GetImageData() : nullptr);
  //if (dtiVolumeNode && imageData)
  //  {
  //  int dimensions[3] = {0,0,0};
  //  imageData->GetDimensions(dimensions);
  //  double spacing[3] = {0.0,0.0,0.0};
  //  dtiVolumeNode->GetSpacing(spacing);
  //  tooltipString.append( QString(" (Dimensions: %1x%2x%3  Spacing: %4mm x %5mm x %6mm)")
  //    .arg(dimensions[0]).arg(dimensions[1]).arg(dimensions[2])
  //    .arg(spacing[0],0,'g',3).arg(spacing[1],0,'g',3).arg(spacing[2],0,'g',3) );
  //  }
  //else
  //  {
  //  tooltipString.append(" !Invalid volume!");
  //  }

  return tooltipString;
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::icon(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin);

  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QIcon();
    }

  // DTI volume
  if (this->canOwnSubjectHierarchyItem(itemID))
    {
    return d->DiffusionTensorVolumeIcon;
    }

  // Item unknown by plugin
  return QIcon();
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::visibilityIcon(int visible)
{
  return qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes")->visibilityIcon(visible);
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::setDisplayVisibility(vtkIdType itemID, int visible)
{
  qCjyxSubjectHierarchyVolumesPlugin* volumesPlugin = qobject_cast<qCjyxSubjectHierarchyVolumesPlugin*>(
    qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes") );
  if (!volumesPlugin)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy volumes plugin";
    return;
    }

  volumesPlugin->setDisplayVisibility(itemID, visible);
}

//-----------------------------------------------------------------------------
int qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::getDisplayVisibility(vtkIdType itemID)const
{
  qCjyxSubjectHierarchyVolumesPlugin* volumesPlugin = qobject_cast<qCjyxSubjectHierarchyVolumesPlugin*>(
    qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes") );
  if (!volumesPlugin)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy volumes plugin";
    return -1;
    }

  return volumesPlugin->getDisplayVisibility(itemID);
}

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::itemContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin);

  QList<QAction*> actions;
  actions << d->TractographyLabelMapSeedingAction << d->TractographyInteractiveSeedingAction;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::showContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin);

  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    // There are no scene actions in this plugin
    return;
    }

  // DTI volume
  if (this->canOwnSubjectHierarchyItem(itemID))
    {
    // Only show if the extension is installed (those specific modules are available)
    if (!qCjyxApplication::application() || !qCjyxApplication::application()->moduleManager())
      {
      return;
      }

    qCjyxAbstractCoreModule* tractographyInteractiveSeedingModule =
      qCjyxApplication::application()->moduleManager()->module("TractographyInteractiveSeeding");
    d->TractographyInteractiveSeedingAction->setVisible(tractographyInteractiveSeedingModule);

    qCjyxAbstractCoreModule* tractographyLabelMapSeedingModule =
      qCjyxApplication::application()->moduleManager()->module("TractographyLabelMapSeeding");
    d->TractographyLabelMapSeedingAction->setVisible(tractographyLabelMapSeedingModule);
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::onTractographyLabelMapSeeding()
{
#ifdef Cjyx_BUILD_CLI_SUPPORT
  //TODO: Select inputs too
  qCjyxAbstractModuleWidget* moduleWidget = qCjyxSubjectHierarchyAbstractPlugin::switchToModule("TractographyLabelMapSeeding");
  Q_UNUSED(moduleWidget);
#else
  qWarning() << Q_FUNC_INFO << ": This operation cannot be performed with CLI disabled";
#endif
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::onTractographyInteractiveSeeding()
{
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  vtkIdType currentItemID = qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
  if (currentItemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid current item!";
    return;
    }

  qCjyxApplication::application()->openNodeModule(shNode->GetItemDataNode(currentItemID));
}

//-----------------------------------------------------------------------------
bool qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin::showItemInView(vtkIdType itemID, vtkMRMLAbstractViewNode* viewNode, vtkIdList* allItemsToShow)
{
  qCjyxSubjectHierarchyVolumesPlugin* volumesPlugin = qobject_cast<qCjyxSubjectHierarchyVolumesPlugin*>(
    qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes"));
  if (!volumesPlugin)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access Volumes subject hierarchy plugin";
    return false;
    }
  return volumesPlugin->showItemInView(itemID, viewNode, allItemsToShow);
}
