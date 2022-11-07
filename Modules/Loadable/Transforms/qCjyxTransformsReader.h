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

#ifndef __qCjyxTransformsReader_h
#define __qCjyxTransformsReader_h

// Cjyx includes
#include "qCjyxFileReader.h"
class qCjyxTransformsReaderPrivate;

// Cjyx includes
class vtkCjyxTransformLogic;

//-----------------------------------------------------------------------------
class qCjyxTransformsReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxTransformsReader(vtkCjyxTransformLogic* transformLogic, QObject* parent = nullptr);
  ~qCjyxTransformsReader() override;

  void setTransformLogic(vtkCjyxTransformLogic* transformLogic);
  vtkCjyxTransformLogic* transformLogic()const;

  QString description()const override;
  IOFileType fileType()const override;
  QStringList extensions()const override;

  bool load(const IOProperties& properties) override;

protected:
  QScopedPointer<qCjyxTransformsReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxTransformsReader);
  Q_DISABLE_COPY(qCjyxTransformsReader);
};

#endif
