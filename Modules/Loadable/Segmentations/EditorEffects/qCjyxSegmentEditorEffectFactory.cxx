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

// Segmentations EditorEffects includes
#include "qCjyxSegmentEditorEffectFactory.h"
#include "qCjyxSegmentEditorAbstractEffect.h"

// Qt includes
#include <QDebug>

//----------------------------------------------------------------------------
qCjyxSegmentEditorEffectFactory *qCjyxSegmentEditorEffectFactory::m_Instance = nullptr;

//----------------------------------------------------------------------------
/// \ingroup CjyxRt_QtModules_Segmentations
class qCjyxSegmentEditorEffectFactoryCleanup
{
public:
  inline void use()   {   }

  ~qCjyxSegmentEditorEffectFactoryCleanup()
    {
    if (qCjyxSegmentEditorEffectFactory::m_Instance)
      {
      qCjyxSegmentEditorEffectFactory::cleanup();
      }
    }
};
static qCjyxSegmentEditorEffectFactoryCleanup qCjyxSegmentEditorEffectFactoryCleanupGlobal;

//-----------------------------------------------------------------------------
qCjyxSegmentEditorEffectFactory* qCjyxSegmentEditorEffectFactory::instance()
{
  if(!qCjyxSegmentEditorEffectFactory::m_Instance)
    {
    qCjyxSegmentEditorEffectFactoryCleanupGlobal.use();
    qCjyxSegmentEditorEffectFactory::m_Instance = new qCjyxSegmentEditorEffectFactory();
    }
  // Return the instance
  return qCjyxSegmentEditorEffectFactory::m_Instance;
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorEffectFactory::cleanup()
{
  if (qCjyxSegmentEditorEffectFactory::m_Instance)
    {
    delete qCjyxSegmentEditorEffectFactory::m_Instance;
    qCjyxSegmentEditorEffectFactory::m_Instance = nullptr;
    }
}

//-----------------------------------------------------------------------------
qCjyxSegmentEditorEffectFactory::qCjyxSegmentEditorEffectFactory(QObject* parent)
  : QObject(parent)
{
  this->m_RegisteredEffects.clear();
}

//-----------------------------------------------------------------------------
qCjyxSegmentEditorEffectFactory::~qCjyxSegmentEditorEffectFactory()
{
  foreach(qCjyxSegmentEditorAbstractEffect* effect, m_RegisteredEffects)
  {
    delete effect;
  }
  this->m_RegisteredEffects.clear();
}

//---------------------------------------------------------------------------
bool qCjyxSegmentEditorEffectFactory::registerEffect(qCjyxSegmentEditorAbstractEffect* effectToRegister)
{
  if (effectToRegister == nullptr)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid effect to register!";
    return false;
    }
  if (effectToRegister->name().isEmpty())
    {
    qCritical() << Q_FUNC_INFO << ": Segment editor effect cannot be registered with empty name!";
    return false;
    }

  // Check if the same effect has already been registered
  qCjyxSegmentEditorAbstractEffect* currentEffect = nullptr;
  foreach (currentEffect, this->m_RegisteredEffects)
    {
    if (effectToRegister->name().compare(currentEffect->name()) == 0)
      {
      return false;
      }
    }

  // Add the effect to the list
  this->m_RegisteredEffects << effectToRegister;
  emit effectRegistered(effectToRegister->name());
  return true;
}

//---------------------------------------------------------------------------
QList<qCjyxSegmentEditorAbstractEffect*> qCjyxSegmentEditorEffectFactory::copyEffects(QList<qCjyxSegmentEditorAbstractEffect*>& effects)
{
  QList<qCjyxSegmentEditorAbstractEffect*> copiedEffects;
  foreach(qCjyxSegmentEditorAbstractEffect* effect, m_RegisteredEffects)
    {
    // If effect is added already then skip it
    bool effectAlreadyAdded = false;
    foreach(qCjyxSegmentEditorAbstractEffect* existingEffect, effects)
      {
      if (existingEffect->name() == effect->name())
        {
        // already in the list
        effectAlreadyAdded = true;
        break;
        }
      }
    if (effectAlreadyAdded)
      {
      continue;
      }

    // Effect not in the list yet, clone it and add it
    qCjyxSegmentEditorAbstractEffect* clonedEffect = effect->clone();
    if (!clonedEffect)
      {
      // make sure we don't put a nullptr pointer in the effect list
      qCritical() << Q_FUNC_INFO << " failed to clone effect: " << effect->name();
      continue;
      }
    effects << clonedEffect;
    copiedEffects << clonedEffect;
    }
  return copiedEffects;
}
