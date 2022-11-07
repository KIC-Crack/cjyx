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

#include "qCjyxModulesListViewPlugin.h"
#include "qCjyxModulesListView.h"

qCjyxModulesListViewPlugin::qCjyxModulesListViewPlugin(QObject* parent)
  : QObject(parent)
{
}

QWidget *qCjyxModulesListViewPlugin::createWidget(QWidget* parentWidget)
{
  qCjyxModulesListView* widget = new qCjyxModulesListView(parentWidget);
  return widget;
}

QString qCjyxModulesListViewPlugin::domXml() const
{
  return "<widget class=\"qCjyxModulesListView\" \
          name=\"CjyxModulesListView\">\n"
          "</widget>\n";
}

QString qCjyxModulesListViewPlugin::includeFile() const
{
  return "qCjyxModulesListView.h";
}

bool qCjyxModulesListViewPlugin::isContainer() const
{
  return false;
}

QString qCjyxModulesListViewPlugin::name() const
{
  return "qCjyxModulesListView";
}
