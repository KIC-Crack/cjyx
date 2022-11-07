/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Brigham and Women's Hospital

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qCjyxVolumeDisplayWidgetPlugin_h
#define __qCjyxVolumeDisplayWidgetPlugin_h

#include "qCjyxVolumesModuleWidgetsAbstractPlugin.h"

class Q_CJYX_MODULE_VOLUMES_WIDGETS_PLUGINS_EXPORT
qCjyxVolumeDisplayWidgetPlugin
  : public QObject, public qCjyxVolumesModuleWidgetsAbstractPlugin
{
  Q_OBJECT

public:
  qCjyxVolumeDisplayWidgetPlugin(QObject *_parent = nullptr);

  QWidget *createWidget(QWidget *_parent) override;
  QString domXml() const override;
  QString includeFile() const override;
  bool isContainer() const override;
  QString name() const override;

};

#endif
