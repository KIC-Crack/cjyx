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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 1U24CA194354-01

==============================================================================*/

#include "qCjyxDirectoryListViewPlugin.h"
#include "qCjyxDirectoryListView.h"

qCjyxDirectoryListViewPlugin::qCjyxDirectoryListViewPlugin(QObject* parent)
  : QObject(parent)
{
}

QWidget *qCjyxDirectoryListViewPlugin::createWidget(QWidget* parentWidget)
{
  qCjyxDirectoryListView* widget = new qCjyxDirectoryListView(parentWidget);
  return widget;
}

QString qCjyxDirectoryListViewPlugin::domXml() const
{
  return "<widget class=\"qCjyxDirectoryListView\" \
          name=\"CjyxDirectoryListView\">\n"
          "</widget>\n";
}

QString qCjyxDirectoryListViewPlugin::includeFile() const
{
  return "qCjyxDirectoryListView.h";
}

bool qCjyxDirectoryListViewPlugin::isContainer() const
{
  return false;
}

QString qCjyxDirectoryListViewPlugin::name() const
{
  return "qCjyxDirectoryListView";
}
