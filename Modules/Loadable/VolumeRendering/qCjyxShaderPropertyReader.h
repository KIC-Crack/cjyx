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

  This file was originally developed by Simon Drouin, Brigham and Women's
  Hospital, Boston, MA.

==============================================================================*/

#ifndef __qCjyxShaderPropertyReader_h
#define __qCjyxShaderPropertyReader_h

// Cjyx includes
#include <qCjyxFileReader.h>

// Volume Rendering includes
class qCjyxShaderPropertyReaderPrivate;
class vtkCjyxVolumeRenderingLogic;

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_VolumeRendering
class qCjyxShaderPropertyReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxShaderPropertyReader(QObject* parent = nullptr);
  qCjyxShaderPropertyReader(vtkCjyxVolumeRenderingLogic* logic, QObject* parent = nullptr);
  ~qCjyxShaderPropertyReader() override;

  void setVolumeRenderingLogic(vtkCjyxVolumeRenderingLogic* logic);
  vtkCjyxVolumeRenderingLogic* volumeRenderingLogic()const;

  // Reimplemented for IO specific description
  QString description() const override;
  IOFileType fileType() const override;
  QStringList extensions() const override;

  bool load(const IOProperties& properties) override ;

protected:
  QScopedPointer<qCjyxShaderPropertyReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxShaderPropertyReader);
  Q_DISABLE_COPY(qCjyxShaderPropertyReader);
};

#endif
