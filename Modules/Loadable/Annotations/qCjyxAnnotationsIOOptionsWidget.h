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

#ifndef __qCjyxAnnotationsIOOptionsWidget_h
#define __qCjyxAnnotationsIOOptionsWidget_h

// Cjyx includes
#include "qCjyxIOOptionsWidget.h"

// Annotations includes
#include "qCjyxAnnotationsModuleExport.h"

class qCjyxAnnotationsIOOptionsWidgetPrivate;
class QButtonGroup;
/// \ingroup Cjyx_QtModules_Annotations
class qCjyxAnnotationsIOOptionsWidget :
  public qCjyxIOOptionsWidget
{
  Q_OBJECT
public:
  qCjyxAnnotationsIOOptionsWidget(QWidget *parent=nullptr);
  ~qCjyxAnnotationsIOOptionsWidget() override;

  QButtonGroup* FileTypeButtonGroup;

public slots:
  void setFileName(const QString& fileName) override;
  void setFileNames(const QStringList& fileNames) override;

protected slots:
  void updateProperties();

private:
  Q_DECLARE_PRIVATE_D(qGetPtrHelper(qCjyxIOOptions::d_ptr),
                      qCjyxAnnotationsIOOptionsWidget);
  Q_DISABLE_COPY(qCjyxAnnotationsIOOptionsWidget);
};

#endif
