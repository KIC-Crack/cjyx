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
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#include "qCjyxStyle.h"
#include "qCjyxWidgetPlugin.h"
#include "qCjyxWidget.h"

// --------------------------------------------------------------------------
qCjyxWidgetPlugin::qCjyxWidgetPlugin(QObject* parent)
  : QObject(parent)
{
}

// --------------------------------------------------------------------------
QWidget *qCjyxWidgetPlugin::createWidget(QWidget* parentWidget)
{
  qCjyxWidget* widget = new qCjyxWidget(parentWidget);
  QPalette cjyxPalette = widget->palette();

  // Apply Cjyx Palette using the non-member function defined in qCjyxApplication
  //qCjyxApplyPalette(cjyxPalette);
  qCjyxStyle style;

  widget->setPalette(style.standardPalette());
  widget->setAutoFillBackground(true);
  return widget;
}

// --------------------------------------------------------------------------
QString qCjyxWidgetPlugin::domXml() const
{
  return "<widget class=\"qCjyxWidget\" name=\"CjyxWidget\">\n"
    " <property name=\"geometry\">\n"
    "  <rect>\n"
    "   <x>0</x>\n"
    "   <y>0</y>\n"
    "   <width>100</width>\n"
    "   <height>100</height>\n"
    "  </rect>\n"
    " </property>\n"
    "</widget>\n";
}

// --------------------------------------------------------------------------
QIcon qCjyxWidgetPlugin::icon() const
{
  return QIcon(":/Icons/widget.png");
}

// --------------------------------------------------------------------------
QString qCjyxWidgetPlugin::includeFile() const
{
  return "qCjyxWidget.h";
}

// --------------------------------------------------------------------------
bool qCjyxWidgetPlugin::isContainer() const
{
  return true;
}

// --------------------------------------------------------------------------
QString qCjyxWidgetPlugin::name() const
{
  return "qCjyxWidget";
}
