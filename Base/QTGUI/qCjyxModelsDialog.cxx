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

/// Qt includes
#include <QFileDialog>
#include <QStyle>

/// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxIOManager.h"
#include "qCjyxModelsDialog_p.h"

// VTK includes
#include "vtkCollection.h"
#include "vtkMRMLNode.h"
#include "vtkNew.h"

//-----------------------------------------------------------------------------
qCjyxModelsDialogPrivate::qCjyxModelsDialogPrivate(qCjyxModelsDialog& object, QWidget* _parentWidget)
  : QDialog(_parentWidget)
  , q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
qCjyxModelsDialogPrivate::~qCjyxModelsDialogPrivate() = default;

//-----------------------------------------------------------------------------
void qCjyxModelsDialogPrivate::init()
{
  this->setupUi(this);
  this->AddModelToolButton->setIcon(this->style()->standardIcon(QStyle::SP_FileIcon));
  this->AddModelDirectoryToolButton->setIcon(this->style()->standardIcon(QStyle::SP_DirIcon));
  connect(this->AddModelToolButton, SIGNAL(clicked()),
          this, SLOT(openAddModelFileDialog()));
  connect(this->AddModelDirectoryToolButton, SIGNAL(clicked()),
          this, SLOT(openAddModelDirectoryDialog()));
}

//-----------------------------------------------------------------------------
void qCjyxModelsDialogPrivate::openAddModelFileDialog()
{
  Q_Q(qCjyxModelsDialog);
  QStringList filters = qCjyxFileDialog::nameFilters(q->fileType());
  // TODO add last open directory
  this->SelectedFiles = QFileDialog::getOpenFileNames(
    this, "Select Model file(s)", "", filters.join(", "));
  if (this->SelectedFiles.count() < 1)
    {
    return;
    }
  this->accept();
}

//-----------------------------------------------------------------------------
void qCjyxModelsDialogPrivate::openAddModelDirectoryDialog()
{
  Q_Q(qCjyxModelsDialog);
  // TODO add last open directory.
  QString modelDirectory = QFileDialog::getExistingDirectory(
    this, "Select a Model directory", "", QFileDialog::ReadOnly);
  if (modelDirectory.isEmpty())
    {
    return;
    }

  QStringList filters = qCjyxFileDialog::nameFilters(q->fileType());
  this->SelectedFiles = QDir(modelDirectory).entryList(filters);
  this->accept();
}

//-----------------------------------------------------------------------------
qCjyxModelsDialog::qCjyxModelsDialog(QObject* _parent)
  : qCjyxFileDialog(_parent)
  , d_ptr(new qCjyxModelsDialogPrivate(*this, nullptr))
{
  // FIXME give qCjyxModelsDialog as a parent of qCjyxModelsDialogPrivate;
  Q_D(qCjyxModelsDialog);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxModelsDialog::~qCjyxModelsDialog() = default;

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxModelsDialog::fileType()const
{
  return QString("ModelFile");
}

//-----------------------------------------------------------------------------
QString qCjyxModelsDialog::description()const
{
  return tr("Models");
}

//-----------------------------------------------------------------------------
qCjyxFileDialog::IOAction qCjyxModelsDialog::action()const
{
  return qCjyxFileDialog::Read;
}

//-----------------------------------------------------------------------------
bool qCjyxModelsDialog::exec(const qCjyxIO::IOProperties& readerProperties)
{
  Q_D(qCjyxModelsDialog);
  Q_ASSERT(!readerProperties.contains("fileName"));

  d->LoadedNodeIDs.clear();
  bool res = false;
  if (d->exec() != QDialog::Accepted)
    {
    return res;
    }
  QStringList files = d->SelectedFiles;
  foreach(QString file, files)
    {
    qCjyxIO::IOProperties properties = readerProperties;
    properties["fileName"] = file;
    vtkNew<vtkCollection> loadedNodes;
    res = qCjyxCoreApplication::application()->coreIOManager()
      ->loadNodes(this->fileType(),
                  properties, loadedNodes.GetPointer()) || res;
    for (int i = 0; i < loadedNodes->GetNumberOfItems(); ++i)
      {
      d->LoadedNodeIDs << vtkMRMLNode::SafeDownCast(loadedNodes->GetItemAsObject(i))
        ->GetID();
      }
    }
  return res;
}

//-----------------------------------------------------------------------------
QStringList qCjyxModelsDialog::loadedNodes()const
{
  Q_D(const qCjyxModelsDialog);
  return d->LoadedNodeIDs;
}
