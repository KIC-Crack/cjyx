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

#include "qCjyxSequencesModuleWidgetsAbstractPlugin.h"

//-----------------------------------------------------------------------------
qCjyxSequencesModuleWidgetsAbstractPlugin::qCjyxSequencesModuleWidgetsAbstractPlugin() = default;

//-----------------------------------------------------------------------------
QString qCjyxSequencesModuleWidgetsAbstractPlugin::group() const
{
  return "Cjyx [Sequence Browser Widgets]";
}

//-----------------------------------------------------------------------------
QIcon qCjyxSequencesModuleWidgetsAbstractPlugin::icon() const
{
  return QIcon();
}

//-----------------------------------------------------------------------------
QString qCjyxSequencesModuleWidgetsAbstractPlugin::toolTip() const
{
  return QString();
}

//-----------------------------------------------------------------------------
QString qCjyxSequencesModuleWidgetsAbstractPlugin::whatsThis() const
{
  return QString();
}
