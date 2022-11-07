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

#ifndef __qCjyxSegmentEditorEffectFactory_h
#define __qCjyxSegmentEditorEffectFactory_h

// Segmentations Editor Effects includes
#include "qCjyxSegmentationsEditorEffectsExport.h"

// Qt includes
#include <QObject>
#include <QList>

class qCjyxSegmentEditorAbstractEffect;
class qCjyxSegmentEditorEffectFactoryCleanup;

/// \ingroup CjyxRt_QtModules_Segmentations
/// \class qCjyxSegmentEditorEffectFactory
/// \brief Singleton class managing segment editor effect plugins
class Q_CJYX_SEGMENTATIONS_EFFECTS_EXPORT qCjyxSegmentEditorEffectFactory : public QObject
{
  Q_OBJECT

public:
  /// Instance getter for the singleton class
  /// \return Instance object
  Q_INVOKABLE static qCjyxSegmentEditorEffectFactory* instance();

public:
  /// Register a effect
  /// \return True if effect registered successfully, false otherwise
  Q_INVOKABLE bool registerEffect(qCjyxSegmentEditorAbstractEffect* effect);

  /// Get list of registered effects
  Q_INVOKABLE QList<qCjyxSegmentEditorAbstractEffect*> registeredEffects() { return m_RegisteredEffects; };

  /// Copy list of registered effects to the container in a segment editor widget.
  /// Effects that are already in the list (have the same name) will not be modified.
  /// \return List of added effects (does not include effects that were already in the effects argument).
  Q_INVOKABLE QList<qCjyxSegmentEditorAbstractEffect*> copyEffects(QList<qCjyxSegmentEditorAbstractEffect*>& effects);

signals:
  /// Signals that a new effect has been registered.
  void effectRegistered(QString);

protected:
  /// List of registered effect instances
  QList<qCjyxSegmentEditorAbstractEffect*> m_RegisteredEffects;

private:
  /// Allows cleanup of the singleton at application exit
  static void cleanup();

private:
  qCjyxSegmentEditorEffectFactory(QObject* parent=nullptr);
  ~qCjyxSegmentEditorEffectFactory() override;

  Q_DISABLE_COPY(qCjyxSegmentEditorEffectFactory);
  friend class qCjyxSegmentEditorEffectFactoryCleanup;
  friend class PythonQtWrapper_qCjyxSegmentEditorEffectFactory; // Allow Python wrapping without enabling direct instantiation

private:
  /// Instance of the singleton
  static qCjyxSegmentEditorEffectFactory* m_Instance;
};

#endif
