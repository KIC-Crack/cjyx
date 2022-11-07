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

  This file was originally developed by Johan Andruejol, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QStringList>
#include <QStyle>

// qMRML includes
#include "qCjyxStyle.h"
#include "qCjyxLightStyle.h"
#include "qCjyxDarkStyle.h"
#include "qCjyxStylePlugin.h"

// --------------------------------------------------------------------------
// qCjyxStylePlugin methods

//-----------------------------------------------------------------------------
qCjyxStylePlugin::qCjyxStylePlugin() = default;

//-----------------------------------------------------------------------------
qCjyxStylePlugin::~qCjyxStylePlugin() = default;

//-----------------------------------------------------------------------------
QStyle* qCjyxStylePlugin::create( const QString & key )
{
  if (key.compare("Cjyx", Qt::CaseInsensitive) == 0)
    {
    return new qCjyxStyle();
    }
  if (key.compare("Light Cjyx", Qt::CaseInsensitive) == 0)
    {
    return new qCjyxLightStyle();
    }
  if (key.compare("Dark Cjyx", Qt::CaseInsensitive) == 0)
    {
    return new qCjyxDarkStyle();
    }
  return nullptr;
}

//-----------------------------------------------------------------------------
QStringList qCjyxStylePlugin::keys() const
{
  return QStringList() << "Cjyx" << "Light Cjyx" << "Dark Cjyx";
}

