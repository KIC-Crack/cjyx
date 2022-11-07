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

#ifndef __qCjyxQTCLIPlugins_h
#define __qCjyxQTCLIPlugins_h

// Qt includes
#include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>

// QtCLI includes
#include "qCjyxCLIProgressBarPlugin.h"

// \class Group the plugins in one library
class Q_CJYX_BASE_QTCLI_PLUGINS_EXPORT qCjyxQTCLIPlugins
  : public QObject
  , public QDesignerCustomWidgetCollectionInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
  Q_INTERFACES(QDesignerCustomWidgetCollectionInterface);

public:
  QList<QDesignerCustomWidgetInterface*> customWidgets() const override
    {
    QList<QDesignerCustomWidgetInterface *> plugins;
    plugins << new qCjyxCLIProgressBarPlugin;
    return plugins;
    }
};

#endif

