/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH)
 All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Cjyx

=========================================================================auto=*/

#ifndef __qCjyxSegmentationsEditorEffectsPythonQtDecorators_h
#define __qCjyxSegmentationsEditorEffectsPythonQtDecorators_h

// PythonQt includes
#include <PythonQt.h>

// Cjyx includes
#include "qCjyxSegmentEditorEffectFactory.h"

#include "qCjyxSegmentationsEditorEffectsExport.h"

// NOTE:
//
// For decorators it is assumed that the methods will never be called
// with the self argument as nullptr.  The self argument is the first argument
// for non-static methods.
//

class qCjyxSegmentationsEditorEffectsPythonQtDecorators : public QObject
{
  Q_OBJECT
public:

  qCjyxSegmentationsEditorEffectsPythonQtDecorators()
    {
    //PythonQt::self()->registerClass(&qCjyxSegmentEditorEffectFactory::staticMetaObject);
    // Note: Use registerCPPClassForPythonQt to register pure Cpp classes
    }

public slots:

  //----------------------------------------------------------------------------
  // qCjyxSegmentEditorEffectFactory

  //----------------------------------------------------------------------------
  // static methods

  //----------------------------------------------------------------------------
  qCjyxSegmentEditorEffectFactory* static_qCjyxSegmentEditorEffectFactory_instance()
    {
    return qCjyxSegmentEditorEffectFactory::instance();
    }

  //----------------------------------------------------------------------------
  // instance methods

  //----------------------------------------------------------------------------
  bool registerEffect(qCjyxSegmentEditorEffectFactory* factory,
                      PythonQtPassOwnershipToCPP<qCjyxSegmentEditorAbstractEffect*> plugin)
    {
    return factory->registerEffect(plugin);
    }
};

//-----------------------------------------------------------------------------
void initqCjyxSegmentationsEditorEffectsPythonQtDecorators()
{
  PythonQt::self()->addDecorators(new qCjyxSegmentationsEditorEffectsPythonQtDecorators);
}

#endif
