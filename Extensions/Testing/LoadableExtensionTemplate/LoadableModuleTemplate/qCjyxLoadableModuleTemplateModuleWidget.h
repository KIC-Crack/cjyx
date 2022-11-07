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

#ifndef __qCjyxLoadableModuleTemplateModuleWidget_h
#define __qCjyxLoadableModuleTemplateModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

#include "qCjyxLoadableModuleTemplateModuleExport.h"

class qCjyxLoadableModuleTemplateModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Cjyx_QtModules_ExtensionTemplate
class Q_CJYX_QTMODULES_LOADABLEMODULETEMPLATE_EXPORT qCjyxLoadableModuleTemplateModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxLoadableModuleTemplateModuleWidget(QWidget *parent=0);
  virtual ~qCjyxLoadableModuleTemplateModuleWidget();

public slots:


protected:
  QScopedPointer<qCjyxLoadableModuleTemplateModuleWidgetPrivate> d_ptr;

  void setup() override;

private:
  Q_DECLARE_PRIVATE(qCjyxLoadableModuleTemplateModuleWidget);
  Q_DISABLE_COPY(qCjyxLoadableModuleTemplateModuleWidget);
};

#endif
