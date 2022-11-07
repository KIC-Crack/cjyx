/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright 2015 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Andras Lasso (PerkLab, Queen's
  University) and Kevin Wang (Princess Margaret Hospital, Toronto) and was
  supported through OCAIRO and the Applied Cancer Research Unit program of
  Cancer Care Ontario.

==============================================================================*/

#ifndef __qCjyxTablesReader
#define __qCjyxTablesReader

// Cjyx includes
#include "qCjyxFileReader.h"

class qCjyxTablesReaderPrivate;
class vtkCjyxTablesLogic;

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Tables
class qCjyxTablesReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxTablesReader(QObject* parent = nullptr);
  qCjyxTablesReader(vtkCjyxTablesLogic* logic,
                       QObject* parent = nullptr);
  ~qCjyxTablesReader() override;

  vtkCjyxTablesLogic* logic()const;
  void setLogic(vtkCjyxTablesLogic* logic);

  QString description()const override;
  IOFileType fileType()const override;
  QStringList extensions()const override;

  bool load(const IOProperties& properties) override;
protected:
  QScopedPointer<qCjyxTablesReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxTablesReader);
  Q_DISABLE_COPY(qCjyxTablesReader);
};

#endif
