/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Brigham and Women's Hospital

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qMRMLSequenceBrowserSeekWidget_h
#define __qMRMLSequenceBrowserSeekWidget_h

// CTK includes
#include <ctkPimpl.h>
#include <ctkVTKObject.h>

// Cjyx includes
#include "qMRMLWidget.h"

#include "qCjyxSequencesModuleWidgetsExport.h"

class qMRMLSequenceBrowserSeekWidgetPrivate;
class vtkMRMLNode;
class vtkMRMLSequenceBrowserNode;
class QSlider;

/// \ingroup Cjyx_QtModules_Markups
class Q_CJYX_MODULE_SEQUENCES_WIDGETS_EXPORT qMRMLSequenceBrowserSeekWidget
: public qMRMLWidget
{
  Q_OBJECT
  QVTK_OBJECT

public:
  typedef qMRMLWidget Superclass;
  qMRMLSequenceBrowserSeekWidget(QWidget *newParent = 0);
  ~qMRMLSequenceBrowserSeekWidget() override;

  /// Get access to the internal slider widget.
  /// This allows fine-tuning of parameters such as page step.
  Q_INVOKABLE QSlider* slider() const;

public slots:
  void setMRMLSequenceBrowserNode(vtkMRMLNode* browserNode);
  void setMRMLSequenceBrowserNode(vtkMRMLSequenceBrowserNode* browserNode);
  void setSelectedItemNumber(int itemNumber);

protected slots:
  void onIndexDisplayFormatModified();
  void updateWidgetFromMRML();

protected:
  QScopedPointer<qMRMLSequenceBrowserSeekWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qMRMLSequenceBrowserSeekWidget);
  Q_DISABLE_COPY(qMRMLSequenceBrowserSeekWidget);

};

#endif
