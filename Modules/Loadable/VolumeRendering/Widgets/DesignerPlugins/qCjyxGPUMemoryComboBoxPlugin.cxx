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

#include "qCjyxGPUMemoryComboBoxPlugin.h"
#include "qCjyxGPUMemoryComboBox.h"

//-----------------------------------------------------------------------------
qCjyxGPUMemoryComboBoxPlugin::qCjyxGPUMemoryComboBoxPlugin(QObject *objectParent)
  : QObject(objectParent)
{
}

//-----------------------------------------------------------------------------
QWidget *qCjyxGPUMemoryComboBoxPlugin::createWidget(QWidget *widgetParent)
{
  qCjyxGPUMemoryComboBox* newWidget = new qCjyxGPUMemoryComboBox(widgetParent);
  return newWidget;
}

//-----------------------------------------------------------------------------
QString qCjyxGPUMemoryComboBoxPlugin::domXml() const
{
  return "<widget class=\"qCjyxGPUMemoryComboBox\" \
          name=\"GPUMemoryComboBox\">\n"
          "</widget>\n";
}

//-----------------------------------------------------------------------------
QString qCjyxGPUMemoryComboBoxPlugin::includeFile() const
{
  return "qCjyxGPUMemoryComboBox.h";
}

//-----------------------------------------------------------------------------
bool qCjyxGPUMemoryComboBoxPlugin::isContainer() const
{
  return false;
}

//-----------------------------------------------------------------------------
QString qCjyxGPUMemoryComboBoxPlugin::name() const
{
  return "qCjyxGPUMemoryComboBox";
}
