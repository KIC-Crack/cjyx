/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qCjyxSuperLoadableModuleTemplateModuleWidget_h
#define __qCjyxSuperLoadableModuleTemplateModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

#include "qCjyxSuperLoadableModuleTemplateModuleExport.h"

class qCjyxSuperLoadableModuleTemplateModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Cjyx_QtModules_ExtensionTemplate
class Q_CJYX_QTMODULES_SUPERLOADABLEMODULETEMPLATE_EXPORT qCjyxSuperLoadableModuleTemplateModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxSuperLoadableModuleTemplateModuleWidget(QWidget *parent=0);
  virtual ~qCjyxSuperLoadableModuleTemplateModuleWidget();

public slots:


protected:
  QScopedPointer<qCjyxSuperLoadableModuleTemplateModuleWidgetPrivate> d_ptr;

  void setup() override;

private:
  Q_DECLARE_PRIVATE(qCjyxSuperLoadableModuleTemplateModuleWidget);
  Q_DISABLE_COPY(qCjyxSuperLoadableModuleTemplateModuleWidget);
};

#endif
