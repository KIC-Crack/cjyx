/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) BWH

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qCjyxMarkupsReader
#define __qCjyxMarkupsReader

// Cjyx includes
#include "qCjyxFileReader.h"

class qCjyxMarkupsReaderPrivate;
class vtkCjyxMarkupsLogic;

//----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Markups
class qCjyxMarkupsReader
  : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxMarkupsReader(QObject* parent = nullptr);
  qCjyxMarkupsReader(vtkCjyxMarkupsLogic* logic, QObject* parent = nullptr);
  ~qCjyxMarkupsReader() override;

  vtkCjyxMarkupsLogic* markupsLogic()const;
  void setMarkupsLogic(vtkCjyxMarkupsLogic* logic);

  QString description()const override;
  IOFileType fileType()const override;
  QStringList extensions()const override;

  bool load(const IOProperties& properties) override;

protected:
  QScopedPointer<qCjyxMarkupsReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxMarkupsReader);
  Q_DISABLE_COPY(qCjyxMarkupsReader);
};

#endif
