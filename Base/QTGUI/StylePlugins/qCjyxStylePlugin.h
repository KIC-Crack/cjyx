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

#ifndef __qCjyxStylePlugin_h
#define __qCjyxStylePlugin_h

// Qt includes
#include <QStylePlugin>
#include <QtPlugin>

class QStyle;

// Cjyx includes
#include "qCjyxBaseQTGUIStylePluginsExport.h"

class Q_CJYX_STYLES_PLUGINS_EXPORT qCjyxStylePlugin : public QStylePlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QStyleFactoryInterface_iid FILE "CjyxStyle.json")
public:
  /// Superclass typedef
  typedef QStylePlugin Superclass;

  /// Constructors
  qCjyxStylePlugin();
  ~qCjyxStylePlugin() override;

  // QStyle plugin classes to overloaded when creating custom style plugins
  QStyle* create(const QString & key) override;
  virtual QStringList keys() const;
};

#endif
