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

// qCjyx includes
#include "qCjyxIOOptions_p.h"
#include "qCjyxIOOptionsWidget.h"

//------------------------------------------------------------------------------
qCjyxIOOptionsWidget::qCjyxIOOptionsWidget(QWidget* parentWidget)
  : qCjyxWidget(parentWidget)
{
}

//------------------------------------------------------------------------------
qCjyxIOOptionsWidget
::qCjyxIOOptionsWidget(qCjyxIOOptionsPrivate* pimpl, QWidget* parentWidget)
  : qCjyxWidget(parentWidget)
  , qCjyxIOOptions(pimpl)
{
}
//------------------------------------------------------------------------------
qCjyxIOOptionsWidget::~qCjyxIOOptionsWidget() = default;

//------------------------------------------------------------------------------
bool qCjyxIOOptionsWidget::isValid()const
{
  Q_D(const qCjyxIOOptions);
  return d->Properties.contains("fileName") || d->Properties.contains("fileNames");
}

//------------------------------------------------------------------------------
void qCjyxIOOptionsWidget::updateValid()
{
  Q_D(const qCjyxIOOptions);
  bool wasValid = d->ArePropertiesValid;
  this->Superclass::updateValid();
  if (wasValid != d->ArePropertiesValid)
    {
    emit this->validChanged(d->ArePropertiesValid);
    }
}

//------------------------------------------------------------------------------
void qCjyxIOOptionsWidget::setFileName(const QString& fileName)
{
  Q_D(qCjyxIOOptions);
  // replace the old filename if any
  if (!fileName.isEmpty())
    {
    d->Properties["fileName"] = fileName;
    }
  else
    {
    d->Properties.remove("fileName");
    }
  d->Properties.remove("fileNames");
  this->updateValid();
}

//------------------------------------------------------------------------------
void qCjyxIOOptionsWidget::setFileNames(const QStringList& fileNames)
{
  Q_D(qCjyxIOOptions);
  /*
  if (fileNames.count())
    {
    this->Properties["fileNames"] = fileNames;
    }
  else
    {
    this->Properties.remove("fileNames");
    }
  this->Properties.remove("fileName");
  */
  d->Properties["fileName"] = fileNames;
  this->updateValid();
}

//------------------------------------------------------------------------------
void qCjyxIOOptionsWidget::updateGUI(const qCjyxIO::IOProperties& ioProperties)
{
  Q_UNUSED(ioProperties);
  // derived classes should implement update of GUI based on provided properties
}
