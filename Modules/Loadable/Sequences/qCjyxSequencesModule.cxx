/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/


// Qt includes
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QTimer>

#include "qCjyxApplication.h"
#include "qCjyxCoreApplication.h"
#include "qCjyxModuleManager.h"

#include "vtkMRMLScene.h"

// Cjyx includes
#include "qCjyxIOManager.h"
#include "qCjyxNodeWriter.h"

// Sequence Logic includes
#include <vtkCjyxSequencesLogic.h>

// Sequence includes
#include "vtkMRMLSequenceBrowserNode.h"
#include "qMRMLSequenceBrowserToolBar.h"
#include "qCjyxSequencesModule.h"
#include "qCjyxSequencesModuleWidget.h"
#include "qCjyxSequencesReader.h"

static const double UPDATE_VIRTUAL_OUTPUT_NODES_PERIOD_SEC = 0.020; // refresh output with a maximum of 50FPS

//-----------------------------------------------------------------------------
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtPlugin>
Q_EXPORT_PLUGIN2(qCjyxSequencesModule, qCjyxSequencesModule);
#endif

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_ExtensionTemplate
class qCjyxSequencesModulePrivate
{
public:
  qCjyxSequencesModulePrivate();

  /// Adds sequence browser toolbar to the application GUI (toolbar and menu)
  virtual void addToolBar();

  virtual ~qCjyxSequencesModulePrivate();
  QTimer UpdateAllVirtualOutputNodesTimer;
  qMRMLSequenceBrowserToolBar* ToolBar;
  bool SequencesModuleOwnsToolBar{true};
  bool AutoShowToolBar{true};
  vtkWeakPointer<vtkMRMLSequenceBrowserNode> SequenceBrowserToShow;

};

//-----------------------------------------------------------------------------
// qCjyxSequencesModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxSequencesModulePrivate::qCjyxSequencesModulePrivate()
{
  this->ToolBar = new qMRMLSequenceBrowserToolBar;
  this->ToolBar->setWindowTitle(QObject::tr("Sequence browser"));
  this->ToolBar->setObjectName("SequenceBrowser");
  this->ToolBar->setVisible(false);
}

//-----------------------------------------------------------------------------
qCjyxSequencesModulePrivate::~qCjyxSequencesModulePrivate()
{
  if (this->SequencesModuleOwnsToolBar)
    {
    // the toolbar has not been added to the main window
    // so it is still owned by this class, therefore
    // we are responsible for deleting it
    delete this->ToolBar;
    this->ToolBar = nullptr;
    }
}

//-----------------------------------------------------------------------------
void qCjyxSequencesModulePrivate::addToolBar()
{
  QMainWindow* mainWindow = qCjyxApplication::application()->mainWindow();
  if (mainWindow==nullptr)
    {
    qDebug("qCjyxSequencesModulePrivate::addToolBar: no main window is available, toolbar is not added");
    return;
    }

  this->ToolBar->setWindowTitle("Sequence browser");
  this->ToolBar->setObjectName("SequenceBrowserToolBar");
  // Add a toolbar break to make the sequence toolbar appear in a separate row
  // (it is a long toolbar and would make many toolbar buttons disappear from
  // all the standard toolbars if they are all displayed in a single row).
  mainWindow->addToolBarBreak();
  mainWindow->addToolBar(this->ToolBar);
  this->SequencesModuleOwnsToolBar = false;
  foreach (QMenu* toolBarMenu,mainWindow->findChildren<QMenu*>())
    {
    if(toolBarMenu->objectName()==QString("WindowToolBarsMenu"))
      {
      toolBarMenu->addAction(this->ToolBar->toggleViewAction());
      break;
      }
    }

  // Main window takes care of saving and restoring toolbar geometry and state.
  // However, when state is restored the sequence browser toolbar was not created yet.
  // We need to restore the main window state again, now, that the Sequences toolbar is available.
  QSettings settings;
  settings.beginGroup("MainWindow");
  bool restore = settings.value("RestoreGeometry", false).toBool();
  if (restore)
  {
    mainWindow->restoreState(settings.value("windowState").toByteArray());
  }
  settings.endGroup();
}


//-----------------------------------------------------------------------------
// qCjyxSequencesModule methods

//-----------------------------------------------------------------------------
qCjyxSequencesModule::qCjyxSequencesModule(QObject* _parent)
: Superclass(_parent)
, d_ptr(new qCjyxSequencesModulePrivate)
{
  Q_D(qCjyxSequencesModule);

  d->UpdateAllVirtualOutputNodesTimer.setSingleShot(true);
  connect(&d->UpdateAllVirtualOutputNodesTimer, SIGNAL(timeout()), this, SLOT(updateAllVirtualOutputNodes()));

  vtkMRMLScene* scene = qCjyxCoreApplication::application()->mrmlScene();
  if (scene)
    {
    // Need to listen for any new sequence browser nodes being added to start/stop timer
    this->qvtkConnect(scene, vtkMRMLScene::NodeAddedEvent, this, SLOT(onNodeAddedEvent(vtkObject*,vtkObject*)));
    this->qvtkConnect(scene, vtkMRMLScene::NodeRemovedEvent, this, SLOT(onNodeRemovedEvent(vtkObject*,vtkObject*)));
    }
}



//-----------------------------------------------------------------------------
qCjyxSequencesModule::~qCjyxSequencesModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxSequencesModule::helpText()const
{
  return "This is a module for creating, recording, and replaying node sequences.";
}

//-----------------------------------------------------------------------------
QString qCjyxSequencesModule::acknowledgementText()const
{
  return "This work was funded by CCO ACRU and OCAIRO grants.";
}

//-----------------------------------------------------------------------------
QStringList qCjyxSequencesModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Andras Lasso (PerkLab, Queen's), Matthew Holden (PerkLab, Queen's), Kevin Wang (PMH)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxSequencesModule::icon()const
{
  return QIcon(":/Icons/Sequences.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxSequencesModule::categories() const
{
  return QStringList() << "Sequences";
}

//-----------------------------------------------------------------------------
QStringList qCjyxSequencesModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qCjyxSequencesModule::setup()
{
  Q_D(qCjyxSequencesModule);
  this->Superclass::setup();
  d->addToolBar();
  // Register IOs
  qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();
  vtkCjyxSequencesLogic* sequencesLogic = vtkCjyxSequencesLogic::SafeDownCast(this->logic());
  ioManager->registerIO(new qCjyxNodeWriter("Sequences", QString("SequenceFile"), QStringList() << "vtkMRMLSequenceNode", true, this));
  ioManager->registerIO(new qCjyxSequencesReader(sequencesLogic, this));
  ioManager->registerIO( new qCjyxNodeWriter( "Sequences", QString( "VolumeSequenceFile" ), QStringList() << "vtkMRMLSequenceNode", true, this ) );
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxSequencesModule
::createWidgetRepresentation()
{
  return new qCjyxSequencesModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxSequencesModule::createLogic()
{
  return vtkCjyxSequencesLogic::New();
}

//-----------------------------------------------------------------------------
void qCjyxSequencesModule::setMRMLScene(vtkMRMLScene* scene)
{
  Q_D(qCjyxSequencesModule);

  vtkMRMLScene* oldScene = this->mrmlScene();
  this->Superclass::setMRMLScene(scene);

  if (scene == nullptr)
    {
    return;
    }

  // Need to listen for any new sequence browser nodes being added to start/stop timer
  this->qvtkReconnect(oldScene, scene, vtkMRMLScene::NodeAddedEvent, this, SLOT(onNodeAddedEvent(vtkObject*,vtkObject*)));
  this->qvtkReconnect(oldScene, scene, vtkMRMLScene::NodeRemovedEvent, this, SLOT(onNodeRemovedEvent(vtkObject*,vtkObject*)));

  d->ToolBar->setMRMLScene(scene);
}

// --------------------------------------------------------------------------
void qCjyxSequencesModule::onNodeAddedEvent(vtkObject*, vtkObject* node)
{
  Q_D(qCjyxSequencesModule);

  vtkMRMLSequenceBrowserNode* sequenceBrowserNode = vtkMRMLSequenceBrowserNode::SafeDownCast(node);
  if (!sequenceBrowserNode)
    {
    return;
    }

  // If the timer is not active, so it should be turned on
  if (!d->UpdateAllVirtualOutputNodesTimer.isActive())
    {
    d->UpdateAllVirtualOutputNodesTimer.start(UPDATE_VIRTUAL_OUTPUT_NODES_PERIOD_SEC*1000.0);
    }

  // If toolbar does not show a valid browser node already then queue the newly added sequence node to be
  // shown in the toolbar.
  if (this->autoShowToolBar() && this->mrmlScene()->IsImporting())
    {
    // If there is a sequence node that is playing then select that, if there is none
    // then select the one that has sequence nodes; otherwise just choose the last newly added
    // sequence node.
    if (!d->SequenceBrowserToShow)
      {
      d->SequenceBrowserToShow = sequenceBrowserNode;
      }
    else
      {
      if (d->SequenceBrowserToShow->GetPlaybackActive())
        {
        // only replace current browser node to show if the new browser node is showing active playback, too
        if (sequenceBrowserNode->GetPlaybackActive())
          {
          d->SequenceBrowserToShow = sequenceBrowserNode;
          }
        }
      else if (d->SequenceBrowserToShow->GetNumberOfSynchronizedSequenceNodes() > 0)
        {
        // only replace current browser node to show if the new browser node has sequences, too
        if (sequenceBrowserNode->GetNumberOfSynchronizedSequenceNodes() > 0)
          {
          d->SequenceBrowserToShow = sequenceBrowserNode;
          }
        }
      else
        {
        d->SequenceBrowserToShow = sequenceBrowserNode;
        }
      }
    // showSequenceBrowser is not called here directly because when the nodes are just being added
    // the toolbar's node selector may not have the newly added browser node in its scene model,
    // and it is more efficient anyway to update the selected browser node when scene loading or batch
    // processing is completed.
    }
}

// --------------------------------------------------------------------------
void qCjyxSequencesModule::onNodeRemovedEvent(vtkObject*, vtkObject* node)
{
  Q_D(qCjyxSequencesModule);

  vtkMRMLSequenceBrowserNode* sequenceBrowserNode = vtkMRMLSequenceBrowserNode::SafeDownCast(node);
  if (sequenceBrowserNode)
    {
    // Check if there is any other sequence browser node left in the Scene
    vtkMRMLScene* scene = qCjyxCoreApplication::application()->mrmlScene();
    if (scene)
      {
      vtkMRMLNode* node;
      node = this->mrmlScene()->GetFirstNodeByClass("vtkMRMLSequenceBrowserNode");
      if (!node)
        {
        // The last sequence browser was removed, so
        // turn off timer refresh and stop any pending timers
        d->UpdateAllVirtualOutputNodesTimer.stop();
        }
      }
    }
}

//-----------------------------------------------------------------------------
void qCjyxSequencesModule::updateAllVirtualOutputNodes()
{
  Q_D(qCjyxSequencesModule);

  vtkMRMLAbstractLogic* l = this->logic();
  vtkCjyxSequencesLogic* sequencesLogic = vtkCjyxSequencesLogic::SafeDownCast(l);
  if (sequencesLogic)
    {
    CjyxRenderBlocker renderBlocker;
    // update proxies then request another singleShot timer
    sequencesLogic->UpdateAllProxyNodes();
    d->UpdateAllVirtualOutputNodesTimer.start(UPDATE_VIRTUAL_OUTPUT_NODES_PERIOD_SEC*1000.0);

    if (d->SequenceBrowserToShow)
      {
      if (this->mrmlScene() && !this->mrmlScene()->IsImporting())
        {
        this->showSequenceBrowser(d->SequenceBrowserToShow);
        d->SequenceBrowserToShow = nullptr;
        }
      }
    }
}

//-----------------------------------------------------------------------------
qMRMLSequenceBrowserToolBar* qCjyxSequencesModule::toolBar()
{
  Q_D(qCjyxSequencesModule);
  return d->ToolBar;
}

//-----------------------------------------------------------------------------
void qCjyxSequencesModule::setToolBarVisible(bool visible)
{
  Q_D(qCjyxSequencesModule);
  d->ToolBar->setVisible(visible);
}

//-----------------------------------------------------------------------------
bool qCjyxSequencesModule::isToolBarVisible()
{
  Q_D(qCjyxSequencesModule);
  return d->ToolBar->isVisible();
}

//-----------------------------------------------------------------------------
void qCjyxSequencesModule::setToolBarActiveBrowserNode(vtkMRMLSequenceBrowserNode* browserNode)
{
  Q_D(qCjyxSequencesModule);
  d->ToolBar->setActiveBrowserNode(browserNode);
}

//-----------------------------------------------------------------------------
bool qCjyxSequencesModule::autoShowToolBar()
{
  Q_D(qCjyxSequencesModule);
  return d->AutoShowToolBar;
}

//-----------------------------------------------------------------------------
void qCjyxSequencesModule::setAutoShowToolBar(bool autoShow)
{
  Q_D(qCjyxSequencesModule);
  d->AutoShowToolBar = autoShow;
}

//-----------------------------------------------------------------------------
bool  qCjyxSequencesModule::showSequenceBrowser(vtkMRMLSequenceBrowserNode* browserNode)
{
  qCjyxCoreApplication* app = qCjyxCoreApplication::application();
  if (!app
    || !app->moduleManager()
    || !dynamic_cast<qCjyxSequencesModule*>(app->moduleManager()->module("Sequences")) )
    {
    qCritical("Sequences module is not available");
    return false;
    }
  qCjyxSequencesModule* sequencesModule = dynamic_cast<qCjyxSequencesModule*>(app->moduleManager()->module("Sequences"));
  if (sequencesModule->autoShowToolBar())
    {
    sequencesModule->setToolBarActiveBrowserNode(browserNode);
    sequencesModule->setToolBarVisible(true);
    }
  return true;
}

//-----------------------------------------------------------------------------
QStringList qCjyxSequencesModule::associatedNodeTypes() const
{
  return QStringList()
    << "vtkMRMLSequenceNode"
    << "vtkMRMLSequenceBrowserNode";
}
