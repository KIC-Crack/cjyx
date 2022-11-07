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
#include <QCompleter>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QStyleOptionButton>
#include <QToolButton>

// CTK includes
#include "ctkComboBox.h"
#include "ctkMenuComboBox.h"

// Cjyx includes
#include "qCjyxAbstractModule.h"
#include "qCjyxApplication.h"
#include "qCjyxModuleFactoryManager.h"
#include "qCjyxModuleFinderDialog.h"
#include "qCjyxModuleManager.h"
#include "qCjyxModuleSelectorToolBar.h"
#include "qCjyxModulesMenu.h"

class qCjyxModuleSelectorToolBarPrivate
{
  Q_DECLARE_PUBLIC(qCjyxModuleSelectorToolBar);
protected:
  qCjyxModuleSelectorToolBar* const q_ptr;
public:
  qCjyxModuleSelectorToolBarPrivate(qCjyxModuleSelectorToolBar& object);
  void init();

  void insertActionOnTop(QAction* action, QMenu* menu);
  QAction* lastSelectedAction()const;

  qCjyxModuleFinderDialog* ModuleFinder;
#ifdef Q_OS_WIN32
  Qt::WindowFlags NormalModuleFinderFlags;
#endif
  qCjyxModulesMenu* ModulesMenu;

  QToolButton*      ModuleFinderButton;
  ctkMenuComboBox*  ModulesComboBox;
  QMenu*            HistoryMenu;
  QToolButton*      HistoryButton;
  QToolButton*      PreviousButton;
  QMenu*            PreviousHistoryMenu;
  QToolButton*      NextButton;
  QMenu*            NextHistoryMenu;
};


//---------------------------------------------------------------------------
qCjyxModuleSelectorToolBarPrivate::qCjyxModuleSelectorToolBarPrivate(qCjyxModuleSelectorToolBar& object)
  : q_ptr(&object)
{
  this->ModuleFinder = nullptr;
  this->ModulesMenu = nullptr;
  this->ModuleFinderButton = nullptr;
  this->ModulesComboBox = nullptr;
  this->HistoryMenu = nullptr;
  this->HistoryButton = nullptr;
  this->PreviousButton = nullptr;
  this->PreviousHistoryMenu = nullptr;
  this->NextButton = nullptr;
  this->NextHistoryMenu = nullptr;
}

//---------------------------------------------------------------------------
void qCjyxModuleSelectorToolBarPrivate::init()
{
  Q_Q(qCjyxModuleSelectorToolBar);
  QIcon previousIcon = q->style()->standardIcon(QStyle::SP_ArrowLeft);
  QIcon nextIcon = q->style()->standardIcon(QStyle::SP_ArrowRight);
  QIcon historyIcon(":Icons/ModuleHistory.png");

  // Modules Label
  q->addWidget(new QLabel(qCjyxModuleSelectorToolBar::tr("Modules:"), q));

  this->ModuleFinder = new qCjyxModuleFinderDialog(q);
#ifdef Q_OS_WIN32
  this->NormalModuleFinderFlags = this->ModuleFinder->windowFlags();
#endif

  // Module finder
  this->ModuleFinderButton = new QToolButton(q);
  const QIcon searchIcon = QIcon::fromTheme("edit-find", QPixmap(":/Icons/Search.png"));
  QAction* ViewFindModuleAction = new QAction(searchIcon, qCjyxModuleSelectorToolBar::tr("Module Finder"));
  ViewFindModuleAction->setObjectName("ViewFindModuleAction");
  ViewFindModuleAction->setToolTip(qCjyxModuleSelectorToolBar::tr("Find module"));
  ViewFindModuleAction->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_F));
  QObject::connect(ViewFindModuleAction, SIGNAL(triggered()),
                   q, SLOT(showModuleFinder()));
  QMainWindow* mainWindow = qCjyxApplication::application()->mainWindow();
  foreach(QMenu * toolBarMenu, mainWindow->findChildren<QMenu*>())
    {
    if (toolBarMenu->objectName() == QString("ViewMenu"))
      {
      foreach(QAction * action, toolBarMenu->actions())
      {
        if (action->objectName() == QString("ViewExtensionsManagerAction"))
          {
          toolBarMenu->insertAction(action, ViewFindModuleAction);
          break;
          }
      }
      break;
      }
    }
  this->ModuleFinderButton->setDefaultAction(ViewFindModuleAction);
  q->addWidget(this->ModuleFinderButton);
  QObject::connect(q, SIGNAL(toolButtonStyleChanged(Qt::ToolButtonStyle)),
    this->ModuleFinderButton, SLOT(setToolButtonStyle(Qt::ToolButtonStyle)));

  // Modules comboBox
  this->ModulesComboBox = new ctkMenuComboBox(q);
  this->ModulesComboBox->setToolTip(qCjyxModuleSelectorToolBar::tr("Select a module from the module list"));
  this->ModulesComboBox->setMinimumContentsLength(20);
  q->addWidget(this->ModulesComboBox);
  QObject::connect(q, SIGNAL(toolButtonStyleChanged(Qt::ToolButtonStyle)),
                   this->ModulesComboBox,SLOT(setToolButtonStyle(Qt::ToolButtonStyle)));

  // Modules menu
  this->ModulesMenu = new qCjyxModulesMenu(qCjyxModuleSelectorToolBar::tr("Modules"), this->ModulesComboBox);
  QObject::connect(this->ModulesMenu, SIGNAL(currentModuleChanged(QString)),
                   q, SLOT(onModuleSelected(QString)));
  this->ModulesComboBox->setMenu(this->ModulesMenu);
  this->ModulesComboBox->setSearchIconVisible(false); // we use the module finder instead;

  // History
  this->HistoryMenu = new QMenu(qCjyxModuleSelectorToolBar::tr("Modules history"), q);
  this->HistoryButton = new QToolButton;
  this->HistoryButton->setText(qCjyxModuleSelectorToolBar::tr("History"));
  this->HistoryButton->setIcon(historyIcon);
  this->HistoryButton->setToolTip(qCjyxModuleSelectorToolBar::tr("Modules history"));
  this->HistoryButton->setMenu(this->HistoryMenu);
  this->HistoryButton->setPopupMode(QToolButton::InstantPopup);
  q->addWidget(this->HistoryButton);
  QObject::connect(q, SIGNAL(toolButtonStyleChanged(Qt::ToolButtonStyle)),
                   this->HistoryButton,SLOT(setToolButtonStyle(Qt::ToolButtonStyle)));

  // Previous button
  this->PreviousHistoryMenu = new QMenu(qCjyxModuleSelectorToolBar::tr("Modules Previous History"), q);
  this->PreviousButton = new QToolButton(q);
  this->PreviousButton->setIcon(previousIcon);
  this->PreviousButton->setText(qCjyxModuleSelectorToolBar::tr("Previous"));
  this->PreviousButton->setToolTip(qCjyxModuleSelectorToolBar::tr("Previous modules"));
  this->PreviousButton->setMenu(this->PreviousHistoryMenu);
  // selectPreviousModule is called only if the toolbutton is clicked not if an
  // action in the history is triggered
  QObject::connect(this->PreviousButton, SIGNAL(clicked(bool)),
                   q, SLOT(selectPreviousModule()));
  q->addWidget(this->PreviousButton);
  QObject::connect(q, SIGNAL(toolButtonStyleChanged(Qt::ToolButtonStyle)),
                   this->PreviousButton,SLOT(setToolButtonStyle(Qt::ToolButtonStyle)));
  this->PreviousButton->setEnabled(this->PreviousHistoryMenu->actions().size() > 0);
  this->PreviousButton->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_Left));

  // Next button
  this->NextHistoryMenu = new QMenu(qCjyxModuleSelectorToolBar::tr("Modules Next History"), q);
  this->NextButton = new QToolButton(q);
  this->NextButton->setIcon(nextIcon);
  this->NextButton->setText(qCjyxModuleSelectorToolBar::tr("Next"));
  this->NextButton->setToolTip(qCjyxModuleSelectorToolBar::tr("Next modules"));
  this->NextButton->setMenu(this->NextHistoryMenu);
  // selectNextModule is called only if the toolbutton is clicked not if an
  // action in the history is triggered
  QObject::connect(this->NextButton, SIGNAL(clicked(bool)),
                   q, SLOT(selectNextModule()));
  q->addWidget(this->NextButton);
  QObject::connect(q, SIGNAL(toolButtonStyleChanged(Qt::ToolButtonStyle)),
                   this->NextButton,SLOT(setToolButtonStyle(Qt::ToolButtonStyle)));
  this->NextButton->setEnabled(this->NextHistoryMenu->actions().size() > 0);
  this->NextButton->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_Right));
}

//---------------------------------------------------------------------------
void qCjyxModuleSelectorToolBarPrivate::insertActionOnTop(QAction* action, QMenu* menu)
{
  menu->removeAction(action);
  QAction* before = menu->actions().isEmpty() ? 0 : menu->actions().first();
  menu->insertAction(before, action);
  QList<QAction*> actions = menu->actions();
  for (int i = 8; i < actions.size(); ++i)
    {
    menu->removeAction(actions[i]);
    }
}

//---------------------------------------------------------------------------
QAction* qCjyxModuleSelectorToolBarPrivate::lastSelectedAction()const
{
  QList<QAction*> actions = this->HistoryMenu->actions();
  return actions.size() ? actions[0] : 0;
}

//---------------------------------------------------------------------------
qCjyxModuleSelectorToolBar::qCjyxModuleSelectorToolBar(const QString& title,
                                                           QWidget* parentWidget)
  : Superclass(title, parentWidget)
  , d_ptr(new qCjyxModuleSelectorToolBarPrivate(*this))
{
  Q_D(qCjyxModuleSelectorToolBar);
  d->init();
}

//---------------------------------------------------------------------------
qCjyxModuleSelectorToolBar::qCjyxModuleSelectorToolBar(QWidget* parentWidget)
  : Superclass(parentWidget)
  , d_ptr(new qCjyxModuleSelectorToolBarPrivate(*this))
{
  Q_D(qCjyxModuleSelectorToolBar);
  d->init();
}

//---------------------------------------------------------------------------
qCjyxModuleSelectorToolBar::~qCjyxModuleSelectorToolBar() = default;

//---------------------------------------------------------------------------
qCjyxModulesMenu* qCjyxModuleSelectorToolBar::modulesMenu()const
{
  Q_D(const qCjyxModuleSelectorToolBar);
  return d->ModulesMenu;
}

//---------------------------------------------------------------------------
ctkMenuComboBox* qCjyxModuleSelectorToolBar::modulesMenuComboBox() const
{
  Q_D(const qCjyxModuleSelectorToolBar);
  return d->ModulesComboBox;
}

//---------------------------------------------------------------------------
QString qCjyxModuleSelectorToolBar::selectedModule()const
{
  Q_D(const qCjyxModuleSelectorToolBar);
  return d->ModulesMenu->currentModule();
}

//---------------------------------------------------------------------------
void qCjyxModuleSelectorToolBar::setModuleManager(qCjyxModuleManager* moduleManager)
{
  Q_D(qCjyxModuleSelectorToolBar);

  if (d->ModulesMenu->moduleManager())
    {
    QObject::disconnect(d->ModulesMenu->moduleManager(),
                        SIGNAL(moduleAboutToBeUnloaded(QString)),
                        this, SLOT(moduleRemoved(QString)));
    }
  d->ModulesMenu->setModuleManager(moduleManager);
  d->ModuleFinder->setFactoryManager(moduleManager->factoryManager());

  if (moduleManager)
    {
    QObject::connect(moduleManager,
                     SIGNAL(moduleAboutToBeUnloaded(QString)),
                     this, SLOT(moduleRemoved(QString)));
    }
}

//---------------------------------------------------------------------------
void qCjyxModuleSelectorToolBar::moduleRemoved(const QString& moduleName)
{
  Q_D(qCjyxModuleSelectorToolBar);
  qCjyxAbstractModule* module = qobject_cast<qCjyxAbstractModule*>(
    d->ModulesMenu->moduleManager()->module(moduleName));
  if (!module)
    {
    return;
    }
  QAction* moduleAction = module->action();
  // removing a module consists in retrieving the unique action of the module
  // and removing it from all the possible menus
  d->HistoryMenu->removeAction(moduleAction);
  d->PreviousHistoryMenu->removeAction(moduleAction);
  d->NextHistoryMenu->removeAction(moduleAction);
  // TBD: what if the module is the current module ?
}

//---------------------------------------------------------------------------
void qCjyxModuleSelectorToolBar::selectModule(const QString& moduleName)
{
  Q_D(qCjyxModuleSelectorToolBar);
  d->ModulesMenu->setCurrentModule(moduleName);
}

//---------------------------------------------------------------------------
void qCjyxModuleSelectorToolBar::onModuleSelected(const QString& name)
{
  Q_D(qCjyxModuleSelectorToolBar);
  this->actionSelected(d->ModulesMenu->moduleAction(name));
}

//---------------------------------------------------------------------------
void qCjyxModuleSelectorToolBar::actionSelected(QAction* action)
{
  Q_D(qCjyxModuleSelectorToolBar);
  QAction* lastAction = d->lastSelectedAction();
  if (action == lastAction)
    {
    return;
    }
  QList<QAction*> previousActions = d->PreviousHistoryMenu->actions();
  QList<QAction*> nextActions = d->NextHistoryMenu->actions();
  // Remove the activated module from the prev/next list
  // to make sure that one module can appear only once
  int actionIndexInPreviousMenu = previousActions.indexOf(action);
  int actionIndexInNextMenu = nextActions.indexOf(action);
  if ( actionIndexInNextMenu >= 0)
    {
    nextActions.removeAt(actionIndexInNextMenu);
    }
  else if ( actionIndexInPreviousMenu >= 0)
    {
    previousActions.removeAt(actionIndexInPreviousMenu);
    }
  // Add the last active module to the previous list if it's not there already
  // (it's already there if the prev/next button was used for module switching)
  if (lastAction)
    {
    if (nextActions.indexOf(lastAction)<0 && previousActions.indexOf(lastAction)<0)
      {
      previousActions.push_front(lastAction);
      }
    }
  // don't keep more than X history
  previousActions = previousActions.mid(0, 8);
  nextActions = nextActions.mid(0, 8);

  d->PreviousHistoryMenu->clear();
  d->PreviousHistoryMenu->addActions(previousActions);
  d->NextHistoryMenu->clear();
  d->NextHistoryMenu->addActions(nextActions);

  d->PreviousButton->setEnabled(d->PreviousHistoryMenu->actions().size());
  d->NextButton->setEnabled(d->NextHistoryMenu->actions().size());

  if (action)
    {
    d->insertActionOnTop(action, d->HistoryMenu);
    }
  if (action == nullptr)
    {
    // Because the NoModuleAction is not observed by ctkMenuComboBox, the
    // toolbar shall clear the text of the current action manually.
    d->ModulesComboBox->clearActiveAction();
    }
  emit moduleSelected(action ? action->data().toString() : QString());
}

//---------------------------------------------------------------------------
void qCjyxModuleSelectorToolBar::showModuleFinder()
{
  Q_D(qCjyxModuleSelectorToolBar);
#ifdef Q_OS_WIN32
  d->ModuleFinder->setWindowFlags(d->NormalModuleFinderFlags);
#endif
  d->ModuleFinder->setFocusToModuleTitleFilter();
  int result = d->ModuleFinder->exec();
  if (result == QMessageBox::Accepted && !d->ModuleFinder->currentModuleName().isEmpty())
    {
    this->selectModule(d->ModuleFinder->currentModuleName());
    }
#ifdef Q_OS_WIN32
  // On Windows, dialog boxes that are just hidden but not deleted appear in
  // taskbar preview (hover over the application icon in the taskbar, wait for the
  // small preview window to appear, then hover over the preview window), which
  // is quite confusing.
  // Deleting and recreating the module finder for each module switch would solve
  // the problem, but rebuilding the view can take noticeable amount of time, so
  // it is better to create the window only once.
  // Setting window style to Qt::Tool excludes the window from taskbar preview,
  // but it also interferes with style and focus behavior of the window.
  // Therefore we set this style, but only while the finder is hidden.
  d->ModuleFinder->setWindowFlag(Qt::Tool, false);
#endif
}

//---------------------------------------------------------------------------
void qCjyxModuleSelectorToolBar::selectNextModule()
{
  Q_D(qCjyxModuleSelectorToolBar);
  // selectNextModule() is not called when an action from the next history menu
  // is triggered. selectNextModule() is called only if the next toolbutton is
  // clicked.
  QList<QAction*> actions = d->NextHistoryMenu->actions();
  QAction* nextAction = actions.size() ? actions.first() : 0;
  if (nextAction)
    {
    // Add last active module to the previous list
    // (to prevent default placement in actionSelected() )
    QAction* lastAction = d->lastSelectedAction();
    if (lastAction)
      {
      QList<QAction*> previousActions = d->PreviousHistoryMenu->actions();
      previousActions.push_front(lastAction);
      d->PreviousHistoryMenu->clear();
      d->PreviousHistoryMenu->addActions(previousActions);
      }
    // triggering the action will eventually call actionSelected()
    nextAction->trigger();
    }
}

//---------------------------------------------------------------------------
void qCjyxModuleSelectorToolBar::selectPreviousModule()
{
  Q_D(qCjyxModuleSelectorToolBar);
  // selectPreviousModule() is not called when an action from the Previous
  // history menu is triggered. selectPreviousModule() is called only if the
  // previous toolbutton is clicked.
  QList<QAction*> actions = d->PreviousHistoryMenu->actions();
  QAction* previousAction = actions.size() ? actions.first() : 0;
  if (previousAction)
    {
    // Add last active module to the next list
    // (to prevent default placement in actionSelected() )
    QAction* lastAction = d->lastSelectedAction();
    if (lastAction)
      {
      QList<QAction*> nextActions = d->NextHistoryMenu->actions();
      nextActions.push_front(lastAction);
      d->NextHistoryMenu->clear();
      d->NextHistoryMenu->addActions(nextActions);
      }
    // triggering the action will eventually call actionSelected()
    previousAction->trigger();
    }
}
