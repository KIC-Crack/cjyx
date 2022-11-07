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

#ifndef __qCjyxSceneReader_h
#define __qCjyxSceneReader_h

// QtCore includes
#include "qCjyxDataModuleExport.h"
#include "qCjyxFileReader.h"

// Logic includes
class vtkCjyxCamerasModuleLogic;
class qCjyxSceneReaderPrivate;

///
/// qCjyxSceneReader is the IO class that handle MRML scene
/// It internally call vtkMRMLScene::Connect() or vtkMRMLScene::Import()
/// depending on the clear flag.
class Q_CJYX_QTMODULES_DATA_EXPORT qCjyxSceneReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxSceneReader(vtkCjyxCamerasModuleLogic* camerasLogic, QObject* _parent = nullptr);
  ~qCjyxSceneReader() override;

  QString description()const override;
  /// Support QString("SceneFile")
  qCjyxIO::IOFileType fileType()const override;

  /// Support only .mrml files
  QStringList extensions()const override;

  /// Options to control scene loading
  qCjyxIOOptions* options()const override;

  /// the supported properties are:
  /// QString fileName: the path of the mrml scene to load
  /// bool clear: whether the current should be cleared or not
  bool load(const qCjyxIO::IOProperties& properties) override;
protected:
  QScopedPointer<qCjyxSceneReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSceneReader);
  Q_DISABLE_COPY(qCjyxSceneReader);
};


#endif
