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

==============================================================================*/

#ifndef __qMRMLColorLegendDisplayNodeWidgetPlugin_h
#define __qMRMLColorLegendDisplayNodeWidgetPlugin_h

#include <QtGlobal>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#include "qCjyxColorsModuleWidgetsPluginsExport.h"

class Q_CJYX_QTMODULES_COLORS_WIDGETS_PLUGINS_EXPORT qMRMLColorLegendDisplayNodeWidgetPlugin
  : public QObject, QDesignerCustomWidgetInterface
{
  Q_OBJECT

  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")
  Q_INTERFACES(QDesignerCustomWidgetInterface);
public:


  qMRMLColorLegendDisplayNodeWidgetPlugin(QObject * newParent = nullptr);

  // Don't reimplement this method.
  QString group() const override;

  // You can reimplement these methods
  QString toolTip() const override;
  QString whatsThis() const override;

  QWidget *createWidget(QWidget *newParent) override;
  QString  domXml() const override;
  QIcon    icon() const override;
  QString  includeFile() const override;
  bool     isContainer() const override;
  QString  name() const override;

};

#endif
