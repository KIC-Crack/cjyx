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

#include "qCjyxDTISliceDisplayWidgetPlugin.h"
#include "qCjyxDTISliceDisplayWidget.h"

//------------------------------------------------------------------------------
qCjyxDTISliceDisplayWidgetPlugin
::qCjyxDTISliceDisplayWidgetPlugin(QObject *_parent)
  : QObject(_parent)
{

}

//------------------------------------------------------------------------------
QWidget *qCjyxDTISliceDisplayWidgetPlugin
::createWidget(QWidget *_parent)
{
  qCjyxDTISliceDisplayWidget* _widget
    = new qCjyxDTISliceDisplayWidget(_parent);
  return _widget;
}

//------------------------------------------------------------------------------
QString qCjyxDTISliceDisplayWidgetPlugin
::domXml() const
{
  return "<widget class=\"qCjyxDTISliceDisplayWidget\" \
          name=\"VolumeDisplayWidget\">\n"
          "</widget>\n";
}

//------------------------------------------------------------------------------
QString qCjyxDTISliceDisplayWidgetPlugin
::includeFile() const
{
  return "qCjyxDTISliceDisplayWidget.h";
}

//------------------------------------------------------------------------------
bool qCjyxDTISliceDisplayWidgetPlugin
::isContainer() const
{
  return false;
}

//------------------------------------------------------------------------------
QString qCjyxDTISliceDisplayWidgetPlugin
::name() const
{
  return "qCjyxDTISliceDisplayWidget";
}
