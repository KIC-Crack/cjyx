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

  This file was originally developed by Steve Pieper, Isomics Inc.
  and was partially funded by NSF grant DBI 1759883

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWebEngineDownloadItem>

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "qCjyxDataDialog.h"
#include "qCjyxWebDownloadWidget.h"

// --------------------------------------------------------------------------
qCjyxWebDownloadWidget::qCjyxWebDownloadWidget(QWidget *parent)
  : QDialog(parent)
{
}

// --------------------------------------------------------------------------
void qCjyxWebDownloadWidget::handleDownload(QWebEngineDownloadItem *download)
{
  // need to use a modal dialog here because 'download' will be deleted
  // if we don't return 'accept from this slot
  QMessageBox *messageBox = new QMessageBox(this);
  messageBox->setWindowTitle(tr("Web download"));
  messageBox->setText(tr("Load data into Cjyx or download to separate location."));
  QPushButton *loadButton = messageBox->addButton(tr("Load..."), QMessageBox::AcceptRole);
  messageBox->setDefaultButton(loadButton);
  loadButton->setToolTip(tr("Download the data and open in Cjyx"));
  QPushButton *saveButton = messageBox->addButton(tr("Save..."), QMessageBox::AcceptRole);
  saveButton->setToolTip(tr("Save to an arbitrary location"));
  QPushButton *abortlButton = messageBox->addButton(tr("Cancel"), QMessageBox::AcceptRole);

  this->hide();
  messageBox->exec();

  if (messageBox->clickedButton() == loadButton)
    {
    // start the download into Cjyx's temp directory
    qDebug() << "Load...";
    QString fileName = QFileInfo(download->path()).fileName();
    QDir directory = QDir(qCjyxCoreApplication::application()->temporaryPath());
    download->setPath(QFileInfo(directory, fileName).absoluteFilePath());
    }
  else if (messageBox->clickedButton() == saveButton)
    {
    qDebug() << "Save...";
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), download->path());
    if (filePath.isEmpty())
      {
      download->cancel();
      return;
      }
    download->setPath(filePath);
    this->show();
  } else if (messageBox->clickedButton() == abortlButton) {
      qDebug() << "Cancel download...";
      download->cancel();
      return;
  }

  qDebug() << "Saving to " << download->path();
  download->accept();
  this->show();

  //
  // setup progress and cancel UI and callbacks
  //
  QVBoxLayout *layout = new QVBoxLayout();
  this->setLayout(layout);

  QLabel *label = new QLabel();
  label->setText(QString("Downloading %1").arg(QFileInfo(download->path()).fileName()));
  layout->addWidget(label);

  QProgressBar *progressBar = new QProgressBar();
  layout->addWidget(progressBar);

  QWidget *buttons = new QWidget(this);
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttons->setLayout(buttonLayout);
  layout->addWidget(buttons);

  QPushButton *cancelButton = new QPushButton("Cancel");
  buttonLayout->addWidget(cancelButton);

  // Progress
  connect(download, &QWebEngineDownloadItem::downloadProgress, [=](qint64 bytesReceived, qint64 bytesTotal) {
    progressBar->setRange(0, bytesTotal);
    progressBar->setValue(bytesReceived);
  });

  // Cancel
  connect(cancelButton, &QPushButton::clicked, [=]() {
    qDebug() << "Download canceled";
    download->cancel();
    this->hide();
  });

  // Finish
  connect(download, &QWebEngineDownloadItem::finished, [=]() {
    this->hide();
    if (messageBox->clickedButton() == loadButton)
      {
      qCjyxDataDialog *dataDialog = new qCjyxDataDialog(this->parent());
      dataDialog->addFile(download->path());
      dataDialog->exec();
      }
    else
      {
      QMessageBox::information(this, tr("Web download"), tr("Download complete"));
      }
  });
}
