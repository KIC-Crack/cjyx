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

#include "qCjyxLabelMapVolumeDisplayWidgetPlugin.h"
#include "qCjyxLabelMapVolumeDisplayWidget.h"

//------------------------------------------------------------------------------
qCjyxLabelMapVolumeDisplayWidgetPlugin
::qCjyxLabelMapVolumeDisplayWidgetPlugin(QObject *_parent)
  : QObject(_parent)
{

}

//------------------------------------------------------------------------------
QWidget *qCjyxLabelMapVolumeDisplayWidgetPlugin
::createWidget(QWidget *_parent)
{
  qCjyxLabelMapVolumeDisplayWidget* _widget
    = new qCjyxLabelMapVolumeDisplayWidget(_parent);
  return _widget;
}

//------------------------------------------------------------------------------
QString qCjyxLabelMapVolumeDisplayWidgetPlugin
::domXml() const
{
  return "<widget class=\"qCjyxLabelMapVolumeDisplayWidget\" \
          name=\"VolumeDisplayWidget\">\n"
          "</widget>\n";
}

//------------------------------------------------------------------------------
QString qCjyxLabelMapVolumeDisplayWidgetPlugin
::includeFile() const
{
  return "qCjyxLabelMapVolumeDisplayWidget.h";
}

//------------------------------------------------------------------------------
bool qCjyxLabelMapVolumeDisplayWidgetPlugin
::isContainer() const
{
  return false;
}

//------------------------------------------------------------------------------
QString qCjyxLabelMapVolumeDisplayWidgetPlugin
::name() const
{
  return "qCjyxLabelMapVolumeDisplayWidget";
}
