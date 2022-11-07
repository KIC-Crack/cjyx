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

#ifndef __qCjyxLightStyle_h
#define __qCjyxLightStyle_h

// Cjyx includes
#include "qCjyxStyle.h"
#include "qCjyxBaseQTGUIExport.h"

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxLightStyle : public qCjyxStyle
{
  Q_OBJECT
public:
  /// Superclass typedef
  typedef qCjyxStyle Superclass;

  /// Constructors
  qCjyxLightStyle();
  ~qCjyxLightStyle() override;

  QPalette standardPalette() const override;
};

#endif
