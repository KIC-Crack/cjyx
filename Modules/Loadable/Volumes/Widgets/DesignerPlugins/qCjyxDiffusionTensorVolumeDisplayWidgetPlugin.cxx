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

#include "qCjyxDiffusionTensorVolumeDisplayWidgetPlugin.h"
#include "qCjyxDiffusionTensorVolumeDisplayWidget.h"

//------------------------------------------------------------------------------
qCjyxDiffusionTensorVolumeDisplayWidgetPlugin
::qCjyxDiffusionTensorVolumeDisplayWidgetPlugin(QObject *_parent)
  : QObject(_parent)
{

}

//------------------------------------------------------------------------------
QWidget *qCjyxDiffusionTensorVolumeDisplayWidgetPlugin
::createWidget(QWidget *_parent)
{
  qCjyxDiffusionTensorVolumeDisplayWidget* _widget
    = new qCjyxDiffusionTensorVolumeDisplayWidget(_parent);
  return _widget;
}

//------------------------------------------------------------------------------
QString qCjyxDiffusionTensorVolumeDisplayWidgetPlugin
::domXml() const
{
  return "<widget class=\"qCjyxDiffusionTensorVolumeDisplayWidgetPlugin\" \
          name=\"VolumeDisplayWidget\">\n"
          "</widget>\n";
}

//------------------------------------------------------------------------------
QString qCjyxDiffusionTensorVolumeDisplayWidgetPlugin
::includeFile() const
{
  return "qCjyxDiffusionTensorVolumeDisplayWidget.h";
}

//------------------------------------------------------------------------------
bool qCjyxDiffusionTensorVolumeDisplayWidgetPlugin
::isContainer() const
{
  return false;
}

//------------------------------------------------------------------------------
QString qCjyxDiffusionTensorVolumeDisplayWidgetPlugin
::name() const
{
  return "qCjyxDiffusionTensorVolumeDisplayWidget";
}
