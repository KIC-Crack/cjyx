/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Brigham and Women's Hospital

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Laurent Chauvin, Brigham and Women's
  Hospital, based on a template developed by Jean-Christophe Fillion-Robin,
  Kitware Inc. The project was supported by grants 5P01CA067165,
  5R01CA124377, 5R01CA138586, 2R44DE019322, 7R01CA124377,
  5R42CA137886, 5R42CA137886

==============================================================================*/

#ifndef __qMRMLAnnotationFiducialProjectionPropertyWidgetPlugin_h
#define __qMRMLAnnotationFiducialProjectionPropertyWidgetPlugin_h

#include "qCjyxAnnotationModuleWidgetsAbstractPlugin.h"

class Q_CJYX_MODULE_ANNOTATIONS_WIDGETS_PLUGINS_EXPORT
qMRMLAnnotationFiducialProjectionPropertyWidgetPlugin
  : public QObject, public qCjyxAnnotationModuleWidgetsAbstractPlugin
{
  Q_OBJECT

public:
  qMRMLAnnotationFiducialProjectionPropertyWidgetPlugin(QObject *_parent = nullptr);

  QWidget *createWidget(QWidget *_parent) override;
  QString domXml() const override;
  QString includeFile() const override;
  bool isContainer() const override;
  QString name() const override;

};

#endif
