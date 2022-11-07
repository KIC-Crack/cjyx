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

// Cjyx includes
#include "qCjyxAbstractModulePanel.h"
#include "qCjyxModuleManager.h"

//---------------------------------------------------------------------------
class qCjyxAbstractModulePanelPrivate
{
public:
  qCjyxAbstractModulePanelPrivate() = default;
  qCjyxModuleManager* ModuleManager{nullptr};
};

//---------------------------------------------------------------------------
qCjyxAbstractModulePanel::qCjyxAbstractModulePanel(QWidget* _parent, Qt::WindowFlags f)
  : qCjyxWidget(_parent, f)
  , d_ptr(new qCjyxAbstractModulePanelPrivate)
{
}

//---------------------------------------------------------------------------
qCjyxAbstractModulePanel::~qCjyxAbstractModulePanel() = default;

//---------------------------------------------------------------------------
void qCjyxAbstractModulePanel::setModuleManager(qCjyxModuleManager* moduleManager)
{
  Q_D(qCjyxAbstractModulePanel);
  d->ModuleManager = moduleManager;
}

//---------------------------------------------------------------------------
qCjyxModuleManager* qCjyxAbstractModulePanel::moduleManager()const
{
  Q_D(const qCjyxAbstractModulePanel);
  return d->ModuleManager;
}

//---------------------------------------------------------------------------
void qCjyxAbstractModulePanel::addModule(const QString& moduleName)
{
  if (!this->moduleManager())
    {
    return;
    }
  qCjyxAbstractCoreModule* module = this->moduleManager()->module(moduleName);
  Q_ASSERT(module);
  this->addModule(module);
}

//---------------------------------------------------------------------------
void qCjyxAbstractModulePanel::removeModule(const QString& moduleName)
{
  if (!this->moduleManager())
    {
    return;
    }
  qCjyxAbstractCoreModule* module = this->moduleManager()->module(moduleName);
  Q_ASSERT(module);
  this->removeModule(module);
}
