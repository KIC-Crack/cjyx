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

#include "qCjyxVolumeDisplayWidgetPlugin.h"
#include "qCjyxVolumeDisplayWidget.h"

//------------------------------------------------------------------------------
qCjyxVolumeDisplayWidgetPlugin
::qCjyxVolumeDisplayWidgetPlugin(QObject *_parent)
  : QObject(_parent)
{

}

//------------------------------------------------------------------------------
QWidget *qCjyxVolumeDisplayWidgetPlugin
::createWidget(QWidget *_parent)
{
  qCjyxVolumeDisplayWidget* _widget
    = new qCjyxVolumeDisplayWidget(_parent);
  return _widget;
}

//------------------------------------------------------------------------------
QString qCjyxVolumeDisplayWidgetPlugin
::domXml() const
{
  return "<widget class=\"qCjyxVolumeDisplayWidget\" \
          name=\"VolumeDisplayWidget\">\n"
          "</widget>\n";
}

//------------------------------------------------------------------------------
QString qCjyxVolumeDisplayWidgetPlugin
::includeFile() const
{
  return "qCjyxVolumeDisplayWidget.h";
}

//------------------------------------------------------------------------------
bool qCjyxVolumeDisplayWidgetPlugin
::isContainer() const
{
  return false;
}

//------------------------------------------------------------------------------
QString qCjyxVolumeDisplayWidgetPlugin
::name() const
{
  return "qCjyxVolumeDisplayWidget";
}
