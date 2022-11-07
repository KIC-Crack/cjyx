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

#ifndef __qCjyxSceneBundleReader_h
#define __qCjyxSceneBundleReader_h

// QtCore includes
#include "qCjyxFileReader.h"

///
/// qCjyxSceneBundleReader is the IO class that handle MRML scene
/// embedded in a zip file (called a Cjyx Data Bundle).  The extension
/// is mrb (for Medical Reality Bundle)
/// It internally calls vtkMRMLScene::Connect() or vtkMRMLScene::Import()
/// depending on the clear flag.
class Q_CJYX_BASE_QTCORE_EXPORT qCjyxSceneBundleReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxSceneBundleReader(QObject* _parent = nullptr);

  QString description()const override;
  /// Support QString("SceneFile")
  qCjyxIO::IOFileType fileType()const override;

  /// Support only .mrb files
  QStringList extensions()const override;

  /// the supported properties are:
  /// QString fileName: the path of the mrml scene to load
  /// bool clear: whether the current should be cleared or not
  bool load(const qCjyxIO::IOProperties& properties) override;
};


#endif
