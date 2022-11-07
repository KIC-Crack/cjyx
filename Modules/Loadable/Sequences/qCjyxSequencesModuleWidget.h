/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qCjyxSequencesModuleWidget_h
#define __qCjyxSequencesModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

#include "qCjyxSequencesModuleExport.h"

#include <QtGui>

class qCjyxSequencesModuleWidgetPrivate;
class vtkMRMLNode;
class vtkMRMLSequenceNode;
class vtkMRMLSequenceBrowserNode;

/// \ingroup Cjyx_QtModules_ExtensionTemplate
class Q_CJYX_QTMODULES_SEQUENCES_EXPORT qCjyxSequencesModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxSequencesModuleWidget(QWidget *parent=0);
  ~qCjyxSequencesModuleWidget() override;

  /// Set up the GUI from mrml when entering
  void enter() override;
  /// Disconnect from scene when exiting
  void exit() override;

  Q_INVOKABLE void setActiveBrowserNode(vtkMRMLSequenceBrowserNode* browserNode);
  Q_INVOKABLE void setMasterSequenceNode(vtkMRMLSequenceNode* sequenceNode);

  Q_INVOKABLE bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;

public slots:

  void setActiveSequenceNode(vtkMRMLSequenceNode* newActiveSequenceNode);
  void onSequenceNodeSelectionChanged();
  void onSequenceNodeModified();

  void onIndexNameEdited();
  void onIndexUnitEdited();
  void onIndexTypeEdited(QString indexTypeString);

  void onDataNodeEdited( int row, int column );

  void onAddDataNodeButtonClicked();
  void onRemoveDataNodeButtonClicked();

  /// Respond to the scene events
  void onNodeAddedEvent(vtkObject* scene, vtkObject* node);
  void onNodeRemovedEvent(vtkObject* scene, vtkObject* node);
  void onMRMLSceneEndImportEvent();
  void onMRMLSceneEndRestoreEvent();
  void onMRMLSceneEndBatchProcessEvent();
  void onMRMLSceneEndCloseEvent();

protected:
  void updateWidgetFromMRML();

  /// Refresh synchronized sequence nodes table from MRML
  void refreshSynchronizedSequenceNodesTable();

  QScopedPointer<qCjyxSequencesModuleWidgetPrivate> d_ptr;

  void setup() override;

  void setEnableWidgets(bool enable);

public slots:
  void setMRMLScene(vtkMRMLScene* scene) override;

protected slots:
  void activeBrowserNodeChanged(vtkMRMLNode* node);
  void sequenceNodeChanged(vtkMRMLNode*);
  void playbackItemSkippingEnabledChanged(bool enabled);
  void recordMasterOnlyChanged(bool enabled);
  void recordingSamplingModeChanged(int index);
  void indexDisplayModeChanged(int index);
  void indexDisplayFormatChanged(const QString& format);
  void onMRMLInputSequenceInputNodeModified(vtkObject* caller);
  void onActiveBrowserNodeModified(vtkObject* caller);
  void updateChart();

  void sequenceNodeNameEdited(int row, int column);

  void onAddSequenceNodeButtonClicked();
  void onRemoveSequenceNodesButtonClicked();

  void synchronizedSequenceNodePlaybackStateChanged(int aState);
  void synchronizedSequenceNodeRecordingStateChanged(int aState);
  void synchronizedSequenceNodeOverwriteProxyNameStateChanged(int aState);
  void synchronizedSequenceNodeSaveChangesStateChanged(int aState);

  void onProxyNodeChanged(vtkMRMLNode* newProxyNode);

  void updateSequenceItemWidgetFromMRML();
  void updateCandidateNodesWidgetFromMRML(bool forceUpdate = false);

private:
  Q_DECLARE_PRIVATE(qCjyxSequencesModuleWidget);
  Q_DISABLE_COPY(qCjyxSequencesModuleWidget);
};

#endif
