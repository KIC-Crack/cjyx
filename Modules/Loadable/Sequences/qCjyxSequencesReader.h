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

#ifndef __qCjyxSequencesReader_h
#define __qCjyxSequencesReader_h

// Cjyx includes
#include "qCjyxFileReader.h"
class qCjyxSequencesReaderPrivate;

// Cjyx includes
class vtkCjyxSequencesLogic;

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Sequences
class qCjyxSequencesReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxSequencesReader(vtkCjyxSequencesLogic* sequencesLogic = 0, QObject* parent = 0);
  ~qCjyxSequencesReader() override;

  void setSequencesLogic(vtkCjyxSequencesLogic* sequencesLogic);
  vtkCjyxSequencesLogic* sequencesLogic()const;

  QString description()const override;
  IOFileType fileType()const override;
  QStringList extensions()const override;

  bool load(const IOProperties& properties) override;

protected:
  QScopedPointer<qCjyxSequencesReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSequencesReader);
  Q_DISABLE_COPY(qCjyxSequencesReader);
};

#endif
