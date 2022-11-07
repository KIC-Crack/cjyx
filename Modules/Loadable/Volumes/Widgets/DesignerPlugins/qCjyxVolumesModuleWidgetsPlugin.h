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

#ifndef __qCjyxVolumesModuleWidgetsPlugin_h
#define __qCjyxVolumesModuleWidgetsPlugin_h

// Qt includes
#include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>

// Volumes includes
#include "qCjyxVolumesModuleWidgetsPlugin.h"
#include "qCjyxDiffusionTensorVolumeDisplayWidgetPlugin.h"
#include "qCjyxDiffusionWeightedVolumeDisplayWidgetPlugin.h"
#include "qCjyxDTISliceDisplayWidgetPlugin.h"
#include "qCjyxLabelMapVolumeDisplayWidgetPlugin.h"
#include "qCjyxScalarVolumeDisplayWidgetPlugin.h"
#include "qCjyxVolumeDisplayWidgetPlugin.h"

// \class Group the plugins in one library
class Q_CJYX_MODULE_VOLUMES_WIDGETS_PLUGINS_EXPORT qCjyxVolumesModuleWidgetsPlugin
  : public QObject
  , public QDesignerCustomWidgetCollectionInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
  Q_INTERFACES(QDesignerCustomWidgetCollectionInterface);

public:
  QList<QDesignerCustomWidgetInterface*> customWidgets() const override
    {
    QList< QDesignerCustomWidgetInterface* > plugins;
    plugins << new qCjyxDiffusionTensorVolumeDisplayWidgetPlugin;
    plugins << new qCjyxDiffusionWeightedVolumeDisplayWidgetPlugin;
    plugins << new qCjyxDTISliceDisplayWidgetPlugin;
    plugins << new qCjyxLabelMapVolumeDisplayWidgetPlugin;
    plugins << new qCjyxScalarVolumeDisplayWidgetPlugin;
    plugins << new qCjyxVolumeDisplayWidgetPlugin;

    return plugins;
    }
};

#endif
