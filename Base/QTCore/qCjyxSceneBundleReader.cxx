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
#include <QDebug>
#include <QDir>
#include <QDateTime>

// QtCore includes
#include "qCjyxSceneBundleReader.h"

// CTK includes
#include <ctkUtils.h>

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLStorageNode.h>
#include <vtkMRMLMessageCollection.h>

// MRML Logic includes
#include <vtkMRMLApplicationLogic.h>

// VTK includes
#include <vtkNew.h>

// VTKSYS includes
#include <vtksys/SystemTools.hxx>

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "qCjyxCoreIOManager.h"

//-----------------------------------------------------------------------------
qCjyxSceneBundleReader::qCjyxSceneBundleReader(QObject* _parent)
  : Superclass(_parent)
{
}

//-----------------------------------------------------------------------------
QString qCjyxSceneBundleReader::description()const
{
  return "MRB Cjyx Data Bundle";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxSceneBundleReader::fileType()const
{
  return QString("SceneFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxSceneBundleReader::extensions()const
{
  return QStringList() << "*.mrb" << "*.zip" << "*.xar";
}

//-----------------------------------------------------------------------------
bool qCjyxSceneBundleReader::load(const qCjyxIO::IOProperties& properties)
{
  Q_ASSERT(properties.contains("fileName"));
  QString file = properties["fileName"].toString();

  // check for a relative path as the unzip will need an absolute one
  QFileInfo fileInfo(file);
  if (fileInfo.isRelative())
    {
    fileInfo = QFileInfo(QDir::currentPath(), file);
    file = fileInfo.absoluteFilePath();
    }
  bool clear = false;
  if (properties.contains("clear"))
    {
    clear = properties["clear"].toBool();
    }

  bool success = this->mrmlScene()->ReadFromMRB(file.toUtf8(), clear, this->userMessages());
  if (success)
    {
    // Set default scene file format to mrb
    qCjyxCoreIOManager* coreIOManager = qCjyxCoreApplication::application()->coreIOManager();
    coreIOManager->setDefaultSceneFileType("Medical Reality Bundle (.mrb)");
    }

  return success;
}
