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

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#include "qCjyxPresetComboBoxPlugin.h"
#include "qCjyxPresetComboBox.h"

//-----------------------------------------------------------------------------
qCjyxPresetComboBoxPlugin::qCjyxPresetComboBoxPlugin(QObject *objectParent)
  : QObject(objectParent)
{
}

//-----------------------------------------------------------------------------
QWidget *qCjyxPresetComboBoxPlugin::createWidget(QWidget *widgetParent)
{
  qCjyxPresetComboBox* newWidget = new qCjyxPresetComboBox(widgetParent);
  return newWidget;
}

//-----------------------------------------------------------------------------
QString qCjyxPresetComboBoxPlugin::domXml() const
{
  return "<widget class=\"qCjyxPresetComboBox\" \
          name=\"PresetComboBox\">\n"
          "</widget>\n";
}

//-----------------------------------------------------------------------------
QString qCjyxPresetComboBoxPlugin::includeFile() const
{
  return "qCjyxPresetComboBox.h";
}

//-----------------------------------------------------------------------------
bool qCjyxPresetComboBoxPlugin::isContainer() const
{
  return false;
}

//-----------------------------------------------------------------------------
QString qCjyxPresetComboBoxPlugin::name() const
{
  return "qCjyxPresetComboBox";
}
