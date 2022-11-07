/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

#ifndef __qCjyxSegmentEditorScriptedPaintEffect_h
#define __qCjyxSegmentEditorScriptedPaintEffect_h

// Segmentations Editor Effects includes
#include "qCjyxSegmentEditorPaintEffect.h"

#include "qCjyxSegmentationsEditorEffectsExport.h"

// Forward Declare PyObject*
#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif
class qCjyxSegmentEditorScriptedPaintEffectPrivate;

/// \ingroup CjyxRt_QtModules_Segmentations
/// \brief Scripted abstract effect for implementing paint effects in python
///
/// This class provides an interface to paint effects implemented in python.
/// Paint effects are a subtype of general effects that operate on a subregion of the segment
/// defined by brush strokes.
///
/// USAGE: Subclass AbstractScriptedSegmentEditorPaintEffect in EditorEffects/Python subfolder,
///   and register effect by creating this class and setting python source to implemented
///   effect subclass. One example is the DrawEffect.
///
class Q_CJYX_SEGMENTATIONS_EFFECTS_EXPORT qCjyxSegmentEditorScriptedPaintEffect
  : public qCjyxSegmentEditorPaintEffect
{
  Q_OBJECT

public:
  typedef qCjyxSegmentEditorPaintEffect Superclass;
  qCjyxSegmentEditorScriptedPaintEffect(QObject* parent = nullptr);
  ~qCjyxSegmentEditorScriptedPaintEffect() override;

  Q_INVOKABLE QString pythonSource()const;

  /// Set python source for the implemented effect
  /// \param newPythonSource Python file path
  Q_INVOKABLE bool setPythonSource(const QString newPythonSource);

  /// Convenience method allowing to retrieve the associated scripted instance
  Q_INVOKABLE PyObject* self() const;

  /// Set the name property value.
  /// \sa name
  void setName(QString name) override;

// API: Methods that are to be reimplemented in the effect subclasses
public:
  /// Get icon for effect to be displayed in segment editor
  QIcon icon() override;

  /// Get help text for effect to be displayed in the help box
  const QString helpText()const override;

  /// Clone editor effect. Override to return a new instance of the effect sub-class
  qCjyxSegmentEditorAbstractEffect* clone() override;

  /// Perform actions to activate the effect (show options frame, etc.)
  void activate() override;

  /// Perform actions to deactivate the effect (hide options frame, destroy actors, etc.)
  void deactivate() override;

  /// Create options frame widgets, make connections, and add them to the main options frame using \sa addOptionsWidget
  void setupOptionsFrame() override;

  /// Create a cursor customized for the given effect, potentially for each view
  QCursor createCursor(qMRMLWidget* viewWidget) override;

  /// Paint labelmap
  void paintApply(qMRMLWidget* viewWidget) override;

  /// Set default parameters in the parameter MRML node
  void setMRMLDefaults() override;

  /// Simple mechanism to let the effects know that reference geometry change has changed
  void referenceGeometryChanged() override;
  /// Simple mechanism to let the effects know that master volume has changed
  void masterVolumeNodeChanged() override;
  /// Simple mechanism to let the effects know that the layout has changed
  void layoutChanged() override;

public slots:
  /// Update user interface from parameter set node
  void updateGUIFromMRML() override;

  /// Update parameter set node from user interface
  void updateMRMLFromGUI() override;

protected:
  QScopedPointer<qCjyxSegmentEditorScriptedPaintEffectPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSegmentEditorScriptedPaintEffect);
  Q_DISABLE_COPY(qCjyxSegmentEditorScriptedPaintEffect);
};

#endif
