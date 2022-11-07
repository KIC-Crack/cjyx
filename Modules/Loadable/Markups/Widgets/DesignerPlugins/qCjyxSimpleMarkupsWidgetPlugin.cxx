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

  This file was originally developed by Laurent Chauvin, Brigham and Women's
  Hospital, based on a template developed by Jean-Christophe Fillion-Robin,
  Kitware Inc. The project was supported by grants 5P01CA067165,
  5R01CA124377, 5R01CA138586, 2R44DE019322, 7R01CA124377,
  5R42CA137886, 5R42CA137886.
  The file was then updated for the Markups module by Nicole Aucoin, BWH.

==============================================================================*/

#include "qCjyxSimpleMarkupsWidgetPlugin.h"
#include "qCjyxSimpleMarkupsWidget.h"

//------------------------------------------------------------------------------
qCjyxSimpleMarkupsWidgetPlugin
::qCjyxSimpleMarkupsWidgetPlugin(QObject *_parent)
  : QObject(_parent)
{

}

//------------------------------------------------------------------------------
QWidget *qCjyxSimpleMarkupsWidgetPlugin
::createWidget(QWidget *_parent)
{
  qCjyxSimpleMarkupsWidget* _widget
    = new qCjyxSimpleMarkupsWidget(_parent);
  return _widget;
}

//------------------------------------------------------------------------------
QString qCjyxSimpleMarkupsWidgetPlugin
::domXml() const
{
  return "<widget class=\"qCjyxSimpleMarkupsWidget\" \
          name=\"SimpleMarkupsWidget\">\n"
          "</widget>\n";
}

//------------------------------------------------------------------------------
QString qCjyxSimpleMarkupsWidgetPlugin
::includeFile() const
{
  return "qCjyxSimpleMarkupsWidget.h";
}

//------------------------------------------------------------------------------
bool qCjyxSimpleMarkupsWidgetPlugin
::isContainer() const
{
  return false;
}

//------------------------------------------------------------------------------
QString qCjyxSimpleMarkupsWidgetPlugin
::name() const
{
  return "qCjyxSimpleMarkupsWidget";
}
