/*==============================================================================

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

#ifndef __qCjyxSegmentEditorEraseEffect_h
#define __qCjyxSegmentEditorEraseEffect_h

// Segmentations Editor Effects includes
#include "qCjyxSegmentationsEditorEffectsExport.h"

#include "qCjyxSegmentEditorPaintEffect.h"

class qCjyxSegmentEditorEraseEffectPrivate;

/// \ingroup CjyxRt_QtModules_Segmentations
class Q_CJYX_SEGMENTATIONS_EFFECTS_EXPORT qCjyxSegmentEditorEraseEffect :
  public qCjyxSegmentEditorPaintEffect
{
public:
  Q_OBJECT

public:
  typedef qCjyxSegmentEditorPaintEffect Superclass;
  qCjyxSegmentEditorEraseEffect(QObject* parent = nullptr);
  ~qCjyxSegmentEditorEraseEffect() override;

public:
  /// Get icon for effect to be displayed in segment editor
  QIcon icon() override;

  /// Get help text for effect to be displayed in the help box
  Q_INVOKABLE const QString helpText()const override;

  /// Clone editor effect
  qCjyxSegmentEditorAbstractEffect* clone() override;

protected:
  QScopedPointer<qCjyxSegmentEditorEraseEffectPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSegmentEditorEraseEffect);
  Q_DISABLE_COPY(qCjyxSegmentEditorEraseEffect);
};

#endif
