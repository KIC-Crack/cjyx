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

#ifndef __qCjyxPresetComboBoxPlugin_h
#define __qCjyxPresetComboBoxPlugin_h

#include "qCjyxVolumeRenderingModuleWidgetsAbstractPlugin.h"

class Q_CJYX_MODULE_VOLUMERENDERING_WIDGETS_PLUGINS_EXPORT qCjyxPresetComboBoxPlugin
  : public QObject
  , public qCjyxVolumeRenderingModuleWidgetsAbstractPlugin
{
  Q_OBJECT

public:
  qCjyxPresetComboBoxPlugin(QObject *_parent = nullptr);

  QWidget *createWidget(QWidget *_parent) override;
  QString  domXml() const override;
  QString  includeFile() const override;
  bool     isContainer() const override;
  QString  name() const override;

};

#endif
