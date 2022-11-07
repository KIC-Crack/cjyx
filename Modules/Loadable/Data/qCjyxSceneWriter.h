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

#ifndef __qCjyxSceneWriter_h
#define __qCjyxSceneWriter_h

// QtCore includes
#include "qCjyxFileWriter.h"

// Data includes
#include "qCjyxDataModuleExport.h"

class Q_CJYX_QTMODULES_DATA_EXPORT qCjyxSceneWriter
  : public qCjyxFileWriter
{
  Q_OBJECT
public:
  typedef qCjyxFileWriter Superclass;
  qCjyxSceneWriter(QObject* parent = nullptr);
  ~qCjyxSceneWriter() override;

  QString description()const override;
  IOFileType fileType()const override;

  /// Return true if the object is handled by the writer.
  bool canWriteObject(vtkObject* object)const override;

  /// Return  a list of the supported extensions for a particuliar object.
  /// Please read QFileDialog::nameFilters for the allowed formats
  /// Example: "Image (*.jpg *.png *.tiff)", "Model (*.vtk)"
  QStringList extensions(vtkObject* object)const override;

  /// Write the node identified by nodeID into the fileName file.
  /// Returns true on success.
  bool write(const qCjyxIO::IOProperties& properties) override;

protected:
  bool writeToMRML(const qCjyxIO::IOProperties& properties);
  bool writeToMRB(const qCjyxIO::IOProperties& properties);
  bool writeToDirectory(const qCjyxIO::IOProperties& properties);

private:
  Q_DISABLE_COPY(qCjyxSceneWriter);
};

#endif
