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

==============================================================================*/

// STD includes
#include <algorithm>

// Qt includes
#include <QKeyEvent>
#include <QPushButton>

// Cjyx includes
#include "qCjyxAbstractCoreModule.h"
#include "qCjyxAbstractModule.h"
#include "qCjyxApplication.h"
#include "qCjyxModuleFactoryFilterModel.h"
#include "qCjyxModuleFactoryManager.h"
#include "qCjyxModuleManager.h"
#include "qCjyxModuleFinderDialog.h"
#include "qCjyxUtils.h"
#include "ui_qCjyxModuleFinderDialog.h"

// --------------------------------------------------------------------------
// qCjyxModuleFinderDialogPrivate

//-----------------------------------------------------------------------------
class qCjyxModuleFinderDialogPrivate: public Ui_qCjyxModuleFinderDialog
{
  Q_DECLARE_PUBLIC(qCjyxModuleFinderDialog);
protected:
  qCjyxModuleFinderDialog* const q_ptr;

public:
  qCjyxModuleFinderDialogPrivate(qCjyxModuleFinderDialog& object);

  /// Convenient method regrouping all initialization code
  void init();

  void makeSelectedItemVisible();

  QString CurrentModuleName;
};

// --------------------------------------------------------------------------
// qCjyxModuleFinderDialogPrivate methods

// --------------------------------------------------------------------------
qCjyxModuleFinderDialogPrivate::qCjyxModuleFinderDialogPrivate(qCjyxModuleFinderDialog& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxModuleFinderDialogPrivate::init()
{
  Q_Q(qCjyxModuleFinderDialog);

  this->setupUi(q);

  qCjyxModuleFactoryFilterModel* filterModel = this->ModuleListView->filterModel();

  // Hide modules that do not have GUI (user cannot switch to them)
  filterModel->setShowHidden(false);
  // Hide testing modules by default
  filterModel->setShowTesting(false);

  QObject::connect(this->SearchInAllTextCheckBox, SIGNAL(toggled(bool)),
    q, SLOT(setSearchInAllText(bool)));
  QObject::connect(this->ShowBuiltInCheckBox, SIGNAL(toggled(bool)),
    q, SLOT(setShowBuiltInModules(bool)));
  QObject::connect(this->ShowTestingCheckBox, SIGNAL(toggled(bool)),
    q, SLOT(setShowTestingModules(bool)));
  QObject::connect(this->FilterTitleSearchBox, SIGNAL(textChanged(QString)),
    q, SLOT(onModuleTitleFilterTextChanged()));

  QObject::connect(this->ModuleListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
    q, SLOT(onSelectionChanged(QItemSelection, QItemSelection)));

  this->FilterTitleSearchBox->installEventFilter(q);
  this->ModuleListView->viewport()->installEventFilter(q);

  QPushButton* okButton = this->ButtonBox->button(QDialogButtonBox::Ok);
  okButton->setText(q->tr("Switch to module"));

  if (filterModel->rowCount() > 0)
    {
    // select first item
    this->ModuleListView->setCurrentIndex(filterModel->index(0, 0));
    }
}

// --------------------------------------------------------------------------
void qCjyxModuleFinderDialogPrivate::makeSelectedItemVisible()
{
  qCjyxModuleFactoryFilterModel* filterModel = this->ModuleListView->filterModel();

  // Make sure that an item is selected
  if (!this->ModuleListView->currentIndex().isValid())
    {
    if (filterModel->rowCount() > 0)
      {
      // select first item
      this->ModuleListView->setCurrentIndex(filterModel->index(0, 0));
      }
    }
  // Make sure that the selected item is visible
  if (this->ModuleListView->currentIndex().isValid())
    {
    this->ModuleListView->scrollTo(this->ModuleListView->currentIndex());
    }
}

// --------------------------------------------------------------------------
// qCjyxModuleFinderDialog methods

// --------------------------------------------------------------------------
qCjyxModuleFinderDialog::qCjyxModuleFinderDialog(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxModuleFinderDialogPrivate(*this))
{
  Q_D(qCjyxModuleFinderDialog);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxModuleFinderDialog::~qCjyxModuleFinderDialog() = default;

void qCjyxModuleFinderDialog::setFactoryManager(qCjyxAbstractModuleFactoryManager* factoryManager)
{
  Q_D(qCjyxModuleFinderDialog);
  d->ModuleListView->setFactoryManager(factoryManager);
}

//------------------------------------------------------------------------------
void qCjyxModuleFinderDialog::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
  Q_UNUSED(deselected);
  Q_D(qCjyxModuleFinderDialog);

  QString moduleName;
  qCjyxAbstractCoreModule* module = nullptr;
  if (!selected.indexes().empty())
    {
    moduleName = selected.indexes().first().data(Qt::UserRole).toString();
    qCjyxCoreApplication* coreApp = qCjyxCoreApplication::application();
    qCjyxModuleManager* moduleManager = coreApp->moduleManager();
    qCjyxModuleFactoryManager* factoryManager = moduleManager->factoryManager();
    if (factoryManager->isLoaded(moduleName))
      {
      module = moduleManager->module(moduleName);
      }
    }

  d->CurrentModuleName = moduleName;

  if (module)
    {
    d->ModuleDescriptionBrowser->clear();
    QString html;

    // Title
    html.append(QString("<h2>%1</h2>").arg(module->title()));

    // Categories
    QStringList categories = module->categories();
    QStringList filteredCategories;
    foreach(QString category, categories)
      {
      if (category.isEmpty())
        {
        category = QLatin1String("[main]");
        }
      else
        {
        category.replace(".", "->");
        }
      filteredCategories << category;
      }
    html.append(QString("<p><b>Category:</b> %1</p>").arg(filteredCategories.join(", ")));

    // Help
    QString help = module->helpText();
    qCjyxCoreApplication* app = qCjyxCoreApplication::application();
    if (app)
      {
      help = qCjyxUtils::replaceDocumentationUrlVersion(module->helpText(),
        QUrl(app->documentationBaseUrl()).host(), app->documentationVersion());
      }
    help.replace("\\n", "<br>");
    help = help.trimmed();
    if (!help.isEmpty())
      {
      html.append(help.trimmed());
      }

    // Acknowledgments
    qCjyxAbstractModule* guiModule = qobject_cast<qCjyxAbstractModule*>(module);
    if (guiModule && !guiModule->logo().isNull())
      {
      d->ModuleDescriptionBrowser->document()->addResource(QTextDocument::ImageResource,
        QUrl("module://logo.png"), QVariant(guiModule->logo()));
      html.append(
        QString("<center><img src=\"module://logo.png\"/></center><br>"));
      }
    QString acknowledgement = module->acknowledgementText();
    if (!acknowledgement.isEmpty())
      {
      acknowledgement.replace("\\n", "<br>");
      acknowledgement = acknowledgement.trimmed();
      html.append("<p>");
      html.append(acknowledgement.trimmed());
      }

    // Contributors
    if (!module->contributors().isEmpty())
      {
      QString contributors = module->contributors().join(", ");
      QString contributorsText = QString("<p><b>Contributors:</b> ") + contributors + "</p>";
      html.append(contributorsText);
      }

    // Internal name
    if (module->name() != module->title())
      {
      html.append(QString("<p><b>Internal name:</b> %1</p>").arg(module->name()));
      }

    // Type
    QString type = QLatin1String("Core");
    // Use "inherits" instead of "qobject_cast" because "qCjyxBaseQTCLI" depends on "qCjyxQTGUI"
    if (module->inherits("qCjyxScriptedLoadableModule"))
      {
      type = QLatin1String("Python Scripted Loadable");
      }
    else if (module->inherits("qCjyxLoadableModule"))
      {
      type = QLatin1String("C++ Loadable");
      }
    else if (module->inherits("qCjyxCLIModule"))
      {
      type = QLatin1String("Command-Line Interface (CLI)");
      }
    if (module->isBuiltIn())
      {
      type += QLatin1String(", built-in");
      }
    html.append(QString("<p><b>Type:</b> %1</p>").arg(type));

    // Dependencies
    if (!module->dependencies().empty())
      {
      html.append(QString("<p><b>Require:</b> %1</p>").arg(module->dependencies().join(", ")));
      }

    // Location
    html.append(QString("<p><b>Location:</b> %1</p>").arg(module->path()));

    d->ModuleDescriptionBrowser->setHtml(html);
    }
  else
    {
    d->ModuleDescriptionBrowser->clear();
    if (!moduleName.isEmpty())
      {
      d->ModuleDescriptionBrowser->setText(QString("%1 module is not loaded").arg(moduleName));
      }
    }

  // scroll to the top
  QTextCursor cursor = d->ModuleDescriptionBrowser->textCursor();
  cursor.movePosition(QTextCursor::Start);
  d->ModuleDescriptionBrowser->setTextCursor(cursor);

  QPushButton* okButton = d->ButtonBox->button(QDialogButtonBox::Ok);
  okButton->setEnabled(module != nullptr);
}

//---------------------------------------------------------------------------
bool qCjyxModuleFinderDialog::eventFilter(QObject* target, QEvent* event)
{
  Q_D(qCjyxModuleFinderDialog);
  if (target == d->FilterTitleSearchBox)
    {
    // Prevent giving the focus to the previous/next widget if arrow keys are used
    // at the edge of the table (without this: if the current cell is in the top
    // row and user press the Up key, the focus goes from the table to the previous
    // widget in the tab order)
    if (event->type() == QEvent::KeyPress)
      {
      QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
      qCjyxModuleFactoryFilterModel* filterModel = d->ModuleListView->filterModel();
      if (keyEvent != nullptr && filterModel->rowCount() > 0)
        {
        int currentRow = d->ModuleListView->currentIndex().row();
        int stepSize = 1;
        if (keyEvent->key() == Qt::Key_PageUp || keyEvent->key() == Qt::Key_PageDown)
          {
          stepSize = 5;
          }
        else if (keyEvent->key() == Qt::Key_Home || keyEvent->key() == Qt::Key_End)
          {
          stepSize = 10000;
          }
        if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_PageUp || keyEvent->key() == Qt::Key_Home)
          {
          if (currentRow > 0)
            {
            d->ModuleListView->setCurrentIndex(filterModel->index(std::max(0, currentRow - stepSize), 0));
            d->ModuleListView->scrollTo(d->ModuleListView->currentIndex());
            }
          return true;
          }
        else if (keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_PageDown || keyEvent->key() == Qt::Key_End)
          {
          if (currentRow + 1 < filterModel->rowCount())
            {
            d->ModuleListView->setCurrentIndex(filterModel->index(std::min(currentRow + stepSize, filterModel->rowCount()-1), 0));
            d->ModuleListView->scrollTo(d->ModuleListView->currentIndex());
            }
          return true;
          }
        }
      }
    }
  else if (target == d->ModuleListView->viewport() && event->type() == QEvent::MouseButtonDblClick)
    {
    // accept selection on double-click
    this->accept();
    return true;
    }
  return this->Superclass::eventFilter(target, event);
}

//---------------------------------------------------------------------------
QString qCjyxModuleFinderDialog::currentModuleName()const
{
  Q_D(const qCjyxModuleFinderDialog);
  return d->CurrentModuleName;
}

//---------------------------------------------------------------------------
void qCjyxModuleFinderDialog::setFocusToModuleTitleFilter()
{
  Q_D(qCjyxModuleFinderDialog);
  d->FilterTitleSearchBox->setFocus();
  d->makeSelectedItemVisible();
}

//---------------------------------------------------------------------------
void qCjyxModuleFinderDialog::setModuleTitleFilterText(const QString& text)
{
  Q_D(qCjyxModuleFinderDialog);
  d->FilterTitleSearchBox->setText(text);
}

//---------------------------------------------------------------------------
void qCjyxModuleFinderDialog::onModuleTitleFilterTextChanged()
{
  Q_D(qCjyxModuleFinderDialog);
  qCjyxModuleFactoryFilterModel* filterModel = d->ModuleListView->filterModel();
  filterModel->setFilterFixedString(d->FilterTitleSearchBox->text());
  d->makeSelectedItemVisible();
}

//---------------------------------------------------------------------------
void qCjyxModuleFinderDialog::setSearchInAllText(bool searchAll)
{
  Q_D(qCjyxModuleFinderDialog);
  qCjyxModuleFactoryFilterModel* filterModel = d->ModuleListView->filterModel();
  if (searchAll)
    {
    // qModuleListViewPrivate::FullTextSearchRole = Qt::UserRole + 4
    filterModel->setFilterRole(Qt::UserRole + 4);
    }
  else
    {
    // search in displayed module title
    filterModel->setFilterRole(Qt::DisplayRole);
    }
  d->makeSelectedItemVisible();
}

//---------------------------------------------------------------------------
void qCjyxModuleFinderDialog::setShowBuiltInModules(bool show)
{
  Q_D(qCjyxModuleFinderDialog);
  qCjyxModuleFactoryFilterModel* filterModel = d->ModuleListView->filterModel();
  filterModel->setShowBuiltIn(show);
  d->makeSelectedItemVisible();
}

//---------------------------------------------------------------------------
void qCjyxModuleFinderDialog::setShowTestingModules(bool show)
{
  Q_D(qCjyxModuleFinderDialog);
  qCjyxModuleFactoryFilterModel* filterModel = d->ModuleListView->filterModel();
  filterModel->setShowTesting(show);
  d->makeSelectedItemVisible();
}
