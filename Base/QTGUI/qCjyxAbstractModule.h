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

#ifndef __qCjyxAbstractModule_h
#define __qCjyxAbstractModule_h

// Qt includes
#include <QIcon>

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxAbstractCoreModule.h"
#include "qCjyxBaseQTGUIExport.h"
#include "vtkCjyxVersionConfigure.h" // For Cjyx_VERSION_MAJOR, Cjyx_VERSION_MINOR

class QAction;
class qCjyxAbstractModulePrivate;

/// Overrides qCjyxAbstractCoreModule and adds an icon property to the
/// module and associates a QAction to it.
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxAbstractModule
  : public qCjyxAbstractCoreModule
{
  Q_OBJECT
  /// This property holds the module's icon.
  /// It is also the icon of the module QAction (see \a action()).
  Q_PROPERTY(QIcon icon READ icon)
  Q_PROPERTY(QImage logo READ logo)
public:

  typedef qCjyxAbstractCoreModule Superclass;
  qCjyxAbstractModule(QObject *parent=nullptr);
  ~qCjyxAbstractModule() override;

  /// Icon of the module. Anytime a graphical representation of the module
  /// is needed, the icon is used. It's the icon shown in the module selector
  /// as well as in the frequently used module toolbar (if any).
  virtual QIcon icon()const;

  /// The logo of the module, the credits given by the grants or instution
  virtual QImage logo()const;

  /// Returns then associated QAction of the module. It contains all the
  /// information relative to the module. The text (QAction::text()) and icon
  /// (QAction::icon()) are the module title and icon; the name of the module
  ///  is saved in the QAction's data (QAction::data().toString()) and the
  /// module index is the QAction property "index"
  /// (QAction::property("index").toInt()).
  /// It is typically used in the module selector menu;
  /// triggering the QAction will make the module current.
  Q_INVOKABLE QAction * action();
protected:
  QScopedPointer<qCjyxAbstractModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxAbstractModule);
  Q_DISABLE_COPY(qCjyxAbstractModule);
};

#endif
