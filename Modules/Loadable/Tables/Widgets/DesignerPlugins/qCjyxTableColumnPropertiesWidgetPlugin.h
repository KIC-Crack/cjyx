/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Matthew Holden, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

#ifndef __qCjyxTableColumnPropertiesWidgetPlugin_h
#define __qCjyxTableColumnPropertiesWidgetPlugin_h

#include "qCjyxTablesModuleWidgetsAbstractPlugin.h"

class Q_CJYX_MODULE_TABLES_WIDGETS_PLUGINS_EXPORT
qCjyxTableColumnPropertiesWidgetPlugin
  : public QObject, public qCjyxTablesModuleWidgetsAbstractPlugin
{
  Q_OBJECT

public:
  qCjyxTableColumnPropertiesWidgetPlugin(QObject *_parent = nullptr);

  QWidget *createWidget(QWidget *_parent) override;
  QString domXml() const override;
  QString includeFile() const override;
  bool isContainer() const override;
  QString name() const override;

};

#endif
