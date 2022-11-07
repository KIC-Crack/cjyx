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

#ifndef __qCjyxTemplateKeyModuleWidget_h
#define __qCjyxTemplateKeyModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

#include "qCjyxTemplateKeyModuleExport.h"

class qCjyxTemplateKeyModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Cjyx_QtModules_ExtensionTemplate
class Q_CJYX_QTMODULES_TEMPLATEKEY_EXPORT qCjyxTemplateKeyModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxTemplateKeyModuleWidget(QWidget *parent=0);
  virtual ~qCjyxTemplateKeyModuleWidget();

public slots:


protected:
  QScopedPointer<qCjyxTemplateKeyModuleWidgetPrivate> d_ptr;

  void setup() override;

private:
  Q_DECLARE_PRIVATE(qCjyxTemplateKeyModuleWidget);
  Q_DISABLE_COPY(qCjyxTemplateKeyModuleWidget);
};

#endif
