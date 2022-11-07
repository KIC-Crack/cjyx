/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through CANARIE's Research Software Program, and Cancer
  Care Ontario.

==============================================================================*/

#include "qCjyxVolumeRenderingPresetComboBoxPlugin.h"
#include "qCjyxVolumeRenderingPresetComboBox.h"

//-----------------------------------------------------------------------------
qCjyxVolumeRenderingPresetComboBoxPlugin::qCjyxVolumeRenderingPresetComboBoxPlugin(QObject *objectParent)
  : QObject(objectParent)
{
}

//-----------------------------------------------------------------------------
QWidget *qCjyxVolumeRenderingPresetComboBoxPlugin::createWidget(QWidget *widgetParent)
{
  qCjyxVolumeRenderingPresetComboBox* newWidget = new qCjyxVolumeRenderingPresetComboBox(widgetParent);
  return newWidget;
}

//-----------------------------------------------------------------------------
QString qCjyxVolumeRenderingPresetComboBoxPlugin::domXml() const
{
  return "<widget class=\"qCjyxVolumeRenderingPresetComboBox\" \
          name=\"VolumeRenderingPresetComboBox\">\n"
          "</widget>\n";
}

//-----------------------------------------------------------------------------
QString qCjyxVolumeRenderingPresetComboBoxPlugin::includeFile() const
{
  return "qCjyxVolumeRenderingPresetComboBox.h";
}

//-----------------------------------------------------------------------------
bool qCjyxVolumeRenderingPresetComboBoxPlugin::isContainer() const
{
  return false;
}

//-----------------------------------------------------------------------------
QString qCjyxVolumeRenderingPresetComboBoxPlugin::name() const
{
  return "qCjyxVolumeRenderingPresetComboBox";
}
