/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Adam Rankin, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

#ifndef __qCjyxTerminologiesReader_h
#define __qCjyxTerminologiesReader_h

// Cjyx includes
#include "qCjyxFileReader.h"
class qCjyxTerminologiesReaderPrivate;

// Cjyx includes
class vtkCjyxTerminologiesModuleLogic;

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Terminologies
class qCjyxTerminologiesReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxTerminologiesReader(vtkCjyxTerminologiesModuleLogic* terminologiesLogic = nullptr, QObject* parent = nullptr);
  ~qCjyxTerminologiesReader() override;

  void setTerminologiesLogic(vtkCjyxTerminologiesModuleLogic* terminologiesLogic);
  vtkCjyxTerminologiesModuleLogic* terminologiesLogic()const;

  QString description()const override;
  IOFileType fileType()const override;
  QStringList extensions()const override;

  bool load(const IOProperties& properties) override;

protected:
  QScopedPointer<qCjyxTerminologiesReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxTerminologiesReader);
  Q_DISABLE_COPY(qCjyxTerminologiesReader);
};

#endif
