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

#include "qCjyxQTCLIAbstractPlugin.h"

qCjyxQTCLIAbstractPlugin::qCjyxQTCLIAbstractPlugin() = default;

QString qCjyxQTCLIAbstractPlugin::group() const
{
  return "Cjyx [CLI Widgets]";
}

QIcon qCjyxQTCLIAbstractPlugin::icon() const
{
  return QIcon();
}

QString qCjyxQTCLIAbstractPlugin::toolTip() const
{
  return QString();
}

QString qCjyxQTCLIAbstractPlugin::whatsThis() const
{
  return QString();
}


