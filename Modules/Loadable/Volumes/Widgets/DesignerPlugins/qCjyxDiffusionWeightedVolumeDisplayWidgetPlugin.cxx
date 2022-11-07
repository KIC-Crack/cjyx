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

#include "qCjyxDiffusionWeightedVolumeDisplayWidgetPlugin.h"
#include "qCjyxDiffusionWeightedVolumeDisplayWidget.h"

//------------------------------------------------------------------------------
qCjyxDiffusionWeightedVolumeDisplayWidgetPlugin
::qCjyxDiffusionWeightedVolumeDisplayWidgetPlugin(QObject *_parent)
  : QObject(_parent)
{

}

//------------------------------------------------------------------------------
QWidget *qCjyxDiffusionWeightedVolumeDisplayWidgetPlugin
::createWidget(QWidget *_parent)
{
  qCjyxDiffusionWeightedVolumeDisplayWidget* _widget
    = new qCjyxDiffusionWeightedVolumeDisplayWidget(_parent);
  return _widget;
}

//------------------------------------------------------------------------------
QString qCjyxDiffusionWeightedVolumeDisplayWidgetPlugin
::domXml() const
{
  return "<widget class=\"qCjyxDiffusionWeightedVolumeDisplayWidget\" \
          name=\"VolumeDisplayWidget\">\n"
          "</widget>\n";
}

//------------------------------------------------------------------------------
QString qCjyxDiffusionWeightedVolumeDisplayWidgetPlugin
::includeFile() const
{
  return "qCjyxDiffusionWeightedVolumeDisplayWidget.h";
}

//------------------------------------------------------------------------------
bool qCjyxDiffusionWeightedVolumeDisplayWidgetPlugin
::isContainer() const
{
  return false;
}

//------------------------------------------------------------------------------
QString qCjyxDiffusionWeightedVolumeDisplayWidgetPlugin
::name() const
{
  return "qCjyxDiffusionWeightedVolumeDisplayWidget";
}
