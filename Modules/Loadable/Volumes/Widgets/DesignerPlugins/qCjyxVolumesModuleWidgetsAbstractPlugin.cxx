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

==============================================================================*/

#include "qCjyxVolumesModuleWidgetsAbstractPlugin.h"

//-----------------------------------------------------------------------------
qCjyxVolumesModuleWidgetsAbstractPlugin::qCjyxVolumesModuleWidgetsAbstractPlugin() = default;

//-----------------------------------------------------------------------------
QString qCjyxVolumesModuleWidgetsAbstractPlugin::group() const
{
  return "Cjyx [Volumes Widgets]";
}

//-----------------------------------------------------------------------------
QIcon qCjyxVolumesModuleWidgetsAbstractPlugin::icon() const
{
  return QIcon();
}

//-----------------------------------------------------------------------------
QString qCjyxVolumesModuleWidgetsAbstractPlugin::toolTip() const
{
  return QString();
}

//-----------------------------------------------------------------------------
QString qCjyxVolumesModuleWidgetsAbstractPlugin::whatsThis() const
{
  return QString();
}
