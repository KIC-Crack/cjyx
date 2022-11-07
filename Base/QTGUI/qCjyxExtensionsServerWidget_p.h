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

#ifndef __qCjyxExtensionsServerWidgetPrivate_p_h
#define __qCjyxExtensionsServerWidgetPrivate_p_h

// Qt includes
#include <QObject>
#include <QUrl>
#include <QWebChannel>

// Cjyx includes
class qCjyxExtensionsServerWidget;
class qCjyxExtensionsManagerModel;
#include "qCjyxWebWidget_p.h"

// --------------------------------------------------------------------------
class ExtensionInstallWidgetWebChannelProxy : public QObject
{
  Q_OBJECT
public:
  ExtensionInstallWidgetWebChannelProxy() = default;
  qCjyxExtensionsServerWidget* InstallWidget{nullptr};
public slots:
  void refresh();
private:
  Q_DISABLE_COPY(ExtensionInstallWidgetWebChannelProxy);
};

//-----------------------------------------------------------------------------
class qCjyxExtensionsServerWidgetPrivate : public qCjyxWebWidgetPrivate
{
  Q_DECLARE_PUBLIC(qCjyxExtensionsServerWidget);
protected:
  qCjyxExtensionsServerWidget* const q_ptr;

public:
  typedef qCjyxWebWidgetPrivate Superclass;
  qCjyxExtensionsServerWidgetPrivate(qCjyxExtensionsServerWidget& object);
  ~qCjyxExtensionsServerWidgetPrivate() override;

  void setFailurePage(const QStringList &errors);

  void updateTheme();
  void setDarkThemeEnabled(bool enabled);

  void initializeWebChannel(QWebChannel* webChannel) override;
  void initializeWebChannelTransport(QByteArray& webChannelScript) override;
  void registerExtensionsManagerModel(qCjyxExtensionsManagerModel* oldModel, qCjyxExtensionsManagerModel* newModel);

  qCjyxExtensionsManagerModel * ExtensionsManagerModel;

  bool BrowsingEnabled;

  ExtensionInstallWidgetWebChannelProxy* InstallWidgetForWebChannel;
};

#endif
