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
#include <QClipboard>
#include <QDesktopServices>
#include <QFile>
#include <QTextStream>
#include <QUrl>

// CjyxApp includes
#include "qCjyxErrorReportDialog.h"
#include "qCjyxApplication.h"
#include "ui_qCjyxErrorReportDialog.h"

//-----------------------------------------------------------------------------
class qCjyxErrorReportDialogPrivate: public Ui_qCjyxErrorReportDialog
{
public:
};

//-----------------------------------------------------------------------------
// qCjyxErrorReportDialogPrivate methods

//-----------------------------------------------------------------------------
// qCjyxErrorReportDialog methods
qCjyxErrorReportDialog::qCjyxErrorReportDialog(QWidget* parentWidget)
 :QDialog(parentWidget)
  , d_ptr(new qCjyxErrorReportDialogPrivate)
{
  Q_D(qCjyxErrorReportDialog);
  d->setupUi(this);

  QString instructionsText = d->InstructionsLabel->text();
  QString appNameVersionPlatform = QString("%1 %2 %3").arg(
    qCjyxApplication::application()->applicationName()).arg(
    qCjyxApplication::application()->applicationVersion()).arg(
    qCjyxApplication::application()->platform());
  instructionsText.replace(QString("[appname-version-platform]"), QUrl::toPercentEncoding(appNameVersionPlatform));
  d->InstructionsLabel->setText(instructionsText);

  QStringList logFilePaths = qCjyxApplication::application()->recentLogFiles();
  d->RecentLogFilesComboBox->addPaths(logFilePaths);
  if (d->RecentLogFilesComboBox->count() > 0)
    {
    d->RecentLogFilesComboBox->setCurrentIndex(d->RecentLogFilesComboBox->model()->index(0, 0));
    }


  //QObject::connect(d->RecentLogFilesComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onLogFileSelectionChanged()));
  QObject::connect(d->RecentLogFilesComboBox, SIGNAL(currentPathChanged(QString,QString)), this, SLOT(onLogFileSelectionChanged()));
  QObject::connect(d->LogCopyToClipboardPushButton, SIGNAL(clicked()), this, SLOT(onLogCopy()));
  QObject::connect(d->LogFileOpenPushButton, SIGNAL(clicked()), this, SLOT(onLogFileOpen()));
  QObject::connect(d->LogFileEditCheckBox, SIGNAL(clicked(bool)), this, SLOT(onLogFileEditClicked(bool)));

  connect(d->ButtonBox, SIGNAL(rejected()), this, SLOT(close()));

  onLogFileSelectionChanged(); // update log messages textbox

}

//-----------------------------------------------------------------------------
qCjyxErrorReportDialog::~qCjyxErrorReportDialog() = default;

// --------------------------------------------------------------------------
void qCjyxErrorReportDialog::onLogCopy()
{
  Q_D(qCjyxErrorReportDialog);
  QApplication::clipboard()->setText(d->LogText->toPlainText());
}

// --------------------------------------------------------------------------
void qCjyxErrorReportDialog::onLogFileSelectionChanged()
{
  Q_D(qCjyxErrorReportDialog);
  QFile f(d->RecentLogFilesComboBox->currentPath());
  if (f.open(QFile::ReadOnly | QFile::Text))
    {
    QTextStream in(&f);
    QString logText = in.readAll();
    d->LogText->setPlainText(logText);
    }
  else
    {
    d->LogText->clear();
    }
}

// --------------------------------------------------------------------------
void qCjyxErrorReportDialog::onLogFileOpen()
{
  Q_D(qCjyxErrorReportDialog);
  QDesktopServices::openUrl(QUrl("file:///"+d->RecentLogFilesComboBox->currentPath(), QUrl::TolerantMode));
}

// --------------------------------------------------------------------------
void qCjyxErrorReportDialog::onLogFileEditClicked(bool editable)
{
  Q_D(qCjyxErrorReportDialog);
  d->LogText->setReadOnly(!editable);
}
