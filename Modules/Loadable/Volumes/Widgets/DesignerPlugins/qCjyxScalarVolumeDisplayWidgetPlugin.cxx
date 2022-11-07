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

#include "qCjyxScalarVolumeDisplayWidgetPlugin.h"
#include "qCjyxScalarVolumeDisplayWidget.h"

//------------------------------------------------------------------------------
qCjyxScalarVolumeDisplayWidgetPlugin
::qCjyxScalarVolumeDisplayWidgetPlugin(QObject *_parent)
  : QObject(_parent)
{

}

//------------------------------------------------------------------------------
QWidget *qCjyxScalarVolumeDisplayWidgetPlugin
::createWidget(QWidget *_parent)
{
  qCjyxScalarVolumeDisplayWidget* _widget
    = new qCjyxScalarVolumeDisplayWidget(_parent);
  return _widget;
}

//------------------------------------------------------------------------------
QString qCjyxScalarVolumeDisplayWidgetPlugin
::domXml() const
{
  return "<widget class=\"qCjyxScalarVolumeDisplayWidget\" \
          name=\"VolumeDisplayWidget\">\n"
          "</widget>\n";
}

//------------------------------------------------------------------------------
QString qCjyxScalarVolumeDisplayWidgetPlugin
::includeFile() const
{
  return "qCjyxScalarVolumeDisplayWidget.h";
}

//------------------------------------------------------------------------------
bool qCjyxScalarVolumeDisplayWidgetPlugin
::isContainer() const
{
  return false;
}

//------------------------------------------------------------------------------
QString qCjyxScalarVolumeDisplayWidgetPlugin
::name() const
{
  return "qCjyxScalarVolumeDisplayWidget";
}
