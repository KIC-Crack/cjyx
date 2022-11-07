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

#ifndef __qCjyxScriptedLoadableModuleFactory_h
#define __qCjyxScriptedLoadableModuleFactory_h

// CTK includes
#include <ctkPimpl.h>
#include <ctkAbstractFileBasedFactory.h>

// Cjyx includes
#include "qCjyxAbstractCoreModule.h"

#include "qCjyxBaseQTGUIExport.h"

class qCjyxScriptedLoadableModuleFactoryPrivate;

//----------------------------------------------------------------------------
class ctkFactoryScriptedItem : public ctkAbstractFactoryFileBasedItem<qCjyxAbstractCoreModule>
{
public:
  bool load() override;
protected:
  qCjyxAbstractCoreModule* instanciator() override;
};

//----------------------------------------------------------------------------
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxScriptedLoadableModuleFactory :
  public ctkAbstractFileBasedFactory<qCjyxAbstractCoreModule>
{
public:

  typedef ctkAbstractFileBasedFactory<qCjyxAbstractCoreModule> Superclass;
  qCjyxScriptedLoadableModuleFactory();
  ~qCjyxScriptedLoadableModuleFactory() override;

  bool registerScript(const QString& key, const QFileInfo& file);

  ///
  void registerItems() override;

protected:
  QScopedPointer<qCjyxScriptedLoadableModuleFactoryPrivate> d_ptr;

  bool isValidFile(const QFileInfo& file)const override;
  ctkAbstractFactoryItem<qCjyxAbstractCoreModule>*
    createFactoryFileBasedItem() override;

private:
  Q_DECLARE_PRIVATE(qCjyxScriptedLoadableModuleFactory);
  Q_DISABLE_COPY(qCjyxScriptedLoadableModuleFactory);
};

#endif
