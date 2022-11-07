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

#ifndef __qCjyxFileReader_h
#define __qCjyxFileReader_h

// Qt includes
#include <QFileInfo>

// QtCore includes
#include "qCjyxIO.h"
#include "qCjyxBaseQTCoreExport.h"

class qCjyxFileReaderOptions;
class qCjyxFileReaderPrivate;

class Q_CJYX_BASE_QTCORE_EXPORT qCjyxFileReader
  : public qCjyxIO
{
  Q_OBJECT
public:
  typedef qCjyxIO Superclass;
  explicit qCjyxFileReader(QObject* parent = nullptr);
  ~qCjyxFileReader() override;

  /// Return  a list of the supported extensions. Please read
  /// QFileDialog::nameFilters for the allowed formats
  /// Example: "Image (*.jpg *.png *.tiff)", "Model (*.vtk)"
  virtual QStringList extensions()const;

  /// Returns true if the reader can load this file.
  /// Default implementation is a simple and fast, it just checks
  /// if file extension matches any of the wildcards returned by extensions() method.
  virtual bool canLoadFile(const QString& file)const;

  /// Return the matching name filters -> if the fileName is "myimage.nrrd"
  /// and the supported extensions are "Volumes (*.mha *.nrrd *.raw)",
  /// "Images (*.png" *.jpg")", "DICOM (*)" then it returns
  /// "Volumes (*.mha *.nrrd *.raw), DICOM (*)"
  /// \param longestExtensionMatchPtr If non-zero then the method returns
  /// the length of the longest matched extension length in this argument.
  /// It can be used to determine how specifically extension matched.
  QStringList supportedNameFilters(const QString& fileName, int* longestExtensionMatchPtr = nullptr)const;

  /// Properties availables : fileMode, multipleFiles, fileType.
  virtual bool load(const IOProperties& properties);

  /// Return the list of generated nodes from loading the file(s) in load().
  /// Empty list if load() failed
  /// \sa setLoadedNodes(), load()
  virtual QStringList loadedNodes()const;

  /// Implements the file list examination for the corresponding method in the core
  /// IO manager.
  /// \sa qCjyxCoreIOManager
  virtual bool examineFileInfoList(QFileInfoList &fileInfoList, QFileInfo &archetypeFileInfo, qCjyxIO::IOProperties &ioProperties)const;

protected:
  /// Must be called in load() on success with the list of nodes added into the
  /// scene.
  virtual void setLoadedNodes(const QStringList& nodes);

protected:
  QScopedPointer<qCjyxFileReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxFileReader);
  Q_DISABLE_COPY(qCjyxFileReader);
};

#endif
