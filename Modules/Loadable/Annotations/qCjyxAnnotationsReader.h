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

#ifndef __qCjyxAnnotationsReader_h
#define __qCjyxAnnotationsReader_h

// Cjyx includes
#include "qCjyxFileReader.h"

class qCjyxAnnotationsReaderPrivate;
class vtkCjyxAnnotationModuleLogic;

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Annotations
class qCjyxAnnotationsReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxAnnotationsReader(QObject* parent = nullptr);
  qCjyxAnnotationsReader(vtkCjyxAnnotationModuleLogic* logic, QObject* parent = nullptr);
  ~qCjyxAnnotationsReader() override;

  vtkCjyxAnnotationModuleLogic* annotationLogic()const;
  void setAnnotationLogic(vtkCjyxAnnotationModuleLogic* logic);

  QString description()const override;
  IOFileType fileType()const override;
  QStringList extensions()const override;
  qCjyxIOOptions* options()const override;

  bool load(const IOProperties& properties) override;
protected:
  QScopedPointer<qCjyxAnnotationsReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxAnnotationsReader);
  Q_DISABLE_COPY(qCjyxAnnotationsReader);

};

#endif
