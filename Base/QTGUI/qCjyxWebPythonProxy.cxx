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
#include <QMainWindow>

// CTK includes
#include "ctkMessageBox.h"

// Cjyx includes
#include "qCjyxApplication.h"
#ifdef Cjyx_USE_PYTHONQT
# include "qCjyxPythonManager.h"
#endif
#include "qCjyxWebPythonProxy.h"

// --------------------------------------------------------------------------
qCjyxWebPythonProxy::qCjyxWebPythonProxy(QObject *parent)
  : QObject(parent)
{
  this->pythonEvaluationAllowed = false;
}

// --------------------------------------------------------------------------
bool qCjyxWebPythonProxy::isPythonEvaluationAllowed()
{
#ifdef Cjyx_USE_PYTHONQT
  if (this->pythonEvaluationAllowed)
    {
    return true;
    }

  qCjyxCoreApplication * app = qCjyxCoreApplication::application();
  if (!app || qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::AA_DisablePython))
    {
    return false;
    }

  ctkMessageBox* confirmationBox = new ctkMessageBox(qCjyxApplication::application()->mainWindow());
  confirmationBox->setAttribute(Qt::WA_DeleteOnClose);
  confirmationBox->setWindowTitle(tr("Allow Python execution?"));
  confirmationBox->setText("Allow the web page has asked to execute code using Cjyx's python?");

  confirmationBox->addButton(tr("Allow"), QMessageBox::AcceptRole);
  confirmationBox->addButton(tr("Reject"), QMessageBox::RejectRole);

  confirmationBox->setDontShowAgainVisible(true);
  confirmationBox->setDontShowAgainSettingsKey("WebEngine/AllowPythonExecution");
  confirmationBox->setIcon(QMessageBox::Question);
  int resultCode = confirmationBox->exec();

  if (resultCode == QMessageBox::AcceptRole)
    {
    this->pythonEvaluationAllowed = true;
    }
#endif
  return this->pythonEvaluationAllowed;
}

// --------------------------------------------------------------------------
QString qCjyxWebPythonProxy::evalPython(const QString &python)
{

  QString result;
#ifdef Cjyx_USE_PYTHONQT
  if (this->isPythonEvaluationAllowed())
    {
    qCjyxPythonManager *pythonManager = qCjyxApplication::application()->pythonManager();
    result = pythonManager->executeString(python).toString();
    qDebug() << "Running " << python << " result is " << result;
    }
#else
  Q_UNUSED(python);
#endif
  return result;
}
