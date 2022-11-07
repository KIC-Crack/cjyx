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
#include <QLinearGradient>
#include <QPalette>

// qMRML includes
#include "qCjyxLightStyle.h"

// --------------------------------------------------------------------------
// qCjyxStyle methods

// --------------------------------------------------------------------------
qCjyxLightStyle::qCjyxLightStyle() = default;

// --------------------------------------------------------------------------
qCjyxLightStyle::~qCjyxLightStyle() = default;

//------------------------------------------------------------------------------
QPalette qCjyxLightStyle::standardPalette()const
{
  QPalette palette = this->Superclass::standardLightPalette();
  return palette;
}
