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

#ifndef __qCjyxCoreModule_h
#define __qCjyxCoreModule_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxAbstractModule.h"

#include "qCjyxModulesCoreExport.h"

class qCjyxCoreModulePrivate;

class Q_CJYX_MODULES_CORE_EXPORT qCjyxCoreModule : public qCjyxAbstractModule
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModule Superclass;
  qCjyxCoreModule(QObject *parent=nullptr);
  ~qCjyxCoreModule() override;

protected:
  void setup() override;

protected:
  QScopedPointer<qCjyxCoreModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxCoreModule);
  Q_DISABLE_COPY(qCjyxCoreModule);
};

#endif
