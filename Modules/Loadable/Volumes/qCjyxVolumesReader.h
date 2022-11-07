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

#ifndef __qCjyxVolumesReader_h
#define __qCjyxVolumesReader_h

// Cjyx includes
#include "qCjyxFileReader.h"
class qCjyxVolumesReaderPrivate;
class vtkCjyxVolumesLogic;

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Volumes
class qCjyxVolumesReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxVolumesReader(QObject* parent = nullptr);
  qCjyxVolumesReader(vtkCjyxVolumesLogic* logic, QObject* parent = nullptr);
  ~qCjyxVolumesReader() override;

  vtkCjyxVolumesLogic* logic()const;
  void setLogic(vtkCjyxVolumesLogic* logic);

  QString description()const override;
  IOFileType fileType()const override;
  QStringList extensions()const override;
  qCjyxIOOptions* options()const override;

  bool load(const IOProperties& properties) override;

  /// Implements the file list examination for the corresponding method in the core
  /// IO manager.
  /// \sa qCjyxCoreIOManager
  bool examineFileInfoList(QFileInfoList &fileInfoList, QFileInfo &archetypeFileInfo, qCjyxIO::IOProperties &ioProperties)const override;

protected:
  QScopedPointer<qCjyxVolumesReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxVolumesReader);
  Q_DISABLE_COPY(qCjyxVolumesReader);
};

#endif
