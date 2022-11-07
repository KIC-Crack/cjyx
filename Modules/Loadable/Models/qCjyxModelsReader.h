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

#ifndef __qCjyxModelsReader_h
#define __qCjyxModelsReader_h

// Cjyx includes
#include "qCjyxFileReader.h"
class qCjyxModelsReaderPrivate;

// Cjyx includes
class vtkCjyxModelsLogic;

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Models
class qCjyxModelsReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxModelsReader(vtkCjyxModelsLogic* modelsLogic = nullptr, QObject* parent = nullptr);
  ~qCjyxModelsReader() override;

  void setModelsLogic(vtkCjyxModelsLogic* modelsLogic);
  vtkCjyxModelsLogic* modelsLogic()const;

  QString description()const override;
  IOFileType fileType()const override;
  QStringList extensions()const override;
  qCjyxIOOptions* options()const override;

  bool load(const IOProperties& properties) override;

protected:
  QScopedPointer<qCjyxModelsReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxModelsReader);
  Q_DISABLE_COPY(qCjyxModelsReader);
};

#endif
