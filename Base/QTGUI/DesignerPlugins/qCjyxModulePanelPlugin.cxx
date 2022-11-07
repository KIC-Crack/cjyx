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

#include "qCjyxModulePanelPlugin.h"
#include "qCjyxModulePanel.h"

qCjyxModulePanelPlugin::qCjyxModulePanelPlugin(QObject* parent)
  : QObject(parent)
{
}

QWidget *qCjyxModulePanelPlugin::createWidget(QWidget* parentWidget)
{
  qCjyxModulePanel* widget = new qCjyxModulePanel(parentWidget);
  return widget;
}

QString qCjyxModulePanelPlugin::domXml() const
{
  return "<widget class=\"qCjyxModulePanel\" \
          name=\"CjyxModulePanel\">\n"
          "</widget>\n";
}

QString qCjyxModulePanelPlugin::includeFile() const
{
  return "qCjyxModulePanel.h";
}

bool qCjyxModulePanelPlugin::isContainer() const
{
  return false;
}

QString qCjyxModulePanelPlugin::name() const
{
  return "qCjyxModulePanel";
}
