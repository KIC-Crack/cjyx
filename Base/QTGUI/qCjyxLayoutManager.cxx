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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QVariant>

// MRMLWidgets includes
#include <qMRMLSliceWidget.h>
#include <qMRMLSliceControllerWidget.h>

// Cjyx includes
#include "qCjyxLayoutManager.h"
#include "qCjyxApplication.h"

// qMRMLCjyx
#include <qMRMLLayoutManager_p.h>

// MRMLDisplayableManager includes
#include <vtkMRMLSliceViewDisplayableManagerFactory.h>
#include <vtkMRMLThreeDViewDisplayableManagerFactory.h>

// MRML includes
#include <vtkMRMLSliceNode.h>
#include <vtkMRMLSliceLogic.h>

// VTK includes
#include <vtkInteractorObserver.h>
#include <vtkCornerAnnotation.h>

//-----------------------------------------------------------------------------
class qCjyxLayoutManagerPrivate: public qMRMLLayoutManagerPrivate
{
public:
  qCjyxLayoutManagerPrivate(qCjyxLayoutManager& object);

public:
  QString            ScriptedDisplayableManagerDirectory;
};

// --------------------------------------------------------------------------
qCjyxLayoutManagerPrivate::qCjyxLayoutManagerPrivate(qCjyxLayoutManager& object)
  : qMRMLLayoutManagerPrivate(object)
{
}

//------------------------------------------------------------------------------
// qCjyxLayoutManager methods

// -----------------------------------------------------------------------------
qCjyxLayoutManager::qCjyxLayoutManager(QWidget* widget)
  : qMRMLLayoutManager(new qCjyxLayoutManagerPrivate(*this), widget, widget)
{
}

//------------------------------------------------------------------------------
void qCjyxLayoutManager::setScriptedDisplayableManagerDirectory(
  const QString& scriptedDisplayableManagerDirectory)
{
#ifdef Cjyx_USE_PYTHONQT
  if (qCjyxCoreApplication::testAttribute(
        qCjyxCoreApplication::AA_DisablePython))
    {
    return;
    }
  Q_D(qCjyxLayoutManager);

  Q_ASSERT(QFileInfo(scriptedDisplayableManagerDirectory).isDir());
  d->ScriptedDisplayableManagerDirectory = scriptedDisplayableManagerDirectory;
  // Disable for now as we don't have any displayable managers and
  // loading the python file on Windows 64b in Debug crashes.
  //vtkMRMLSliceViewDisplayableManagerFactory* sliceFactory
  //  = vtkMRMLSliceViewDisplayableManagerFactory::GetInstance();
  //sliceFactory->RegisterDisplayableManager(
  //  QFileInfo(QDir(scriptedDisplayableManagerDirectory),
  //            "vtkScriptedExampleDisplayableManager.py")
  //    .absoluteFilePath().toUtf8());
  //vtkMRMLThreeDViewDisplayableManagerFactory* threeDFactory
  //  = vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance();
  //threeDFactory->RegisterDisplayableManager(
  //  QFileInfo(QDir(scriptedDisplayableManagerDirectory),
  //            "vtkScriptedExampleDisplayableManager.py")
  //    .absoluteFilePath().toUtf8());
#else
  Q_UNUSED(scriptedDisplayableManagerDirectory);
#endif
}

//------------------------------------------------------------------------------
void qCjyxLayoutManager::setCurrentModule(const QString& moduleName)
{
  emit this->selectModule(moduleName);
}
