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

#ifndef __qCjyxAboutDialog_h
#define __qCjyxAboutDialog_h

// Qt includes
#include <QDialog>

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxBaseQTAppExport.h"

class qCjyxAboutDialogPrivate;

/// Pre-request that a qCjyxApplication has been instanced
class Q_CJYX_BASE_QTAPP_EXPORT qCjyxAboutDialog :
  public QDialog
{
  Q_OBJECT
public:
  qCjyxAboutDialog(QWidget *parentWidget = nullptr);
  ~qCjyxAboutDialog() override;

  Q_INVOKABLE void setLogo(const QPixmap& newLogo);

protected:
  QScopedPointer<qCjyxAboutDialogPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxAboutDialog);
  Q_DISABLE_COPY(qCjyxAboutDialog);
};

#endif
