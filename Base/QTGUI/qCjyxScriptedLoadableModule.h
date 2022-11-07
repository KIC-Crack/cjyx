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

#ifndef __qCjyxScriptedLoadableModule_h
#define __qCjyxScriptedLoadableModule_h

// Qt includes
#include <QVariantMap>

// Cjyx includes
#include "qCjyxLoadableModule.h"

#include "qCjyxBaseQTGUIExport.h"

class qCjyxScriptedLoadableModulePrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxScriptedLoadableModule : public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PROPERTY(QString title READ title WRITE setTitle)
  /// Reimplemented to expose the setter setCategories
  Q_PROPERTY(QStringList categories READ categories WRITE setCategories)
  Q_PROPERTY(QStringList contributors READ contributors WRITE setContributors)
  Q_PROPERTY(QStringList associatedNodeTypes READ associatedNodeTypes WRITE setAssociatedNodeTypes)
  Q_PROPERTY(QString helpText READ helpText WRITE setHelpText)
  Q_PROPERTY(QString acknowledgementText READ acknowledgementText WRITE setAcknowledgementText)
  Q_PROPERTY(QVariantMap extensions READ extensions WRITE setExtensions)
  Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
  Q_PROPERTY(bool hidden READ isHidden WRITE setHidden)
  Q_PROPERTY(QStringList dependencies READ dependencies WRITE setDependencies)
  Q_PROPERTY(int index READ index WRITE setIndex)

public:

  typedef qCjyxLoadableModule Superclass;
  typedef qCjyxScriptedLoadableModulePrivate Pimpl;
  qCjyxScriptedLoadableModule(QObject *parent=nullptr);
  ~qCjyxScriptedLoadableModule() override;

  QString pythonSource()const;
  bool setPythonSource(const QString& newPythonSource);

  QString title()const override ;
  void setTitle(const QString& newTitle);

  QStringList categories()const override;
  void setCategories(const QStringList& newCategories);

  QStringList contributors()const override;
  void setContributors(const QStringList& newContributors);

  QStringList associatedNodeTypes()const override;
  void setAssociatedNodeTypes(const QStringList& newAssociatedNodeTypes);

  QString helpText()const override;
  void setHelpText(const QString& newHelpText);

  QString acknowledgementText()const override;
  void setAcknowledgementText(const QString& newAcknowledgementText);

  virtual QVariantMap extensions()const;
  void setExtensions(const QVariantMap& extensions);

  QIcon icon()const override;
  void setIcon(const QIcon& newIcon);

  QStringList dependencies() const override;
  void setDependencies(const QStringList& dependencies);

  int index() const override;
  void setIndex(const int index);

  /// Enable/Disable hide state of the module
  /// Needs to be hidden before the module menu is created.
  bool isHidden()const override;
  void setHidden(bool hidden);

protected:

  void setup() override;

  void registerFileDialog();

  void registerIO();

  qCjyxAbstractModuleRepresentation* createWidgetRepresentation() override;

  vtkMRMLAbstractLogic* createLogic() override;

protected:
  QScopedPointer<qCjyxScriptedLoadableModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxScriptedLoadableModule);
  Q_DISABLE_COPY(qCjyxScriptedLoadableModule);
};

#endif
