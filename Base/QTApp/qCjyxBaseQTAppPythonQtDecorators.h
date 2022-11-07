/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH)
 All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Cjyx

=========================================================================auto=*/

#ifndef __qCjyxBaseQTAppPythonQtDecorators_h
#define __qCjyxBaseQTAppPythonQtDecorators_h

// PythonQt includes
#include <PythonQt.h>

// Cjyx includes
#include "qCjyxApplicationHelper.h"

#include "qCjyxBaseQTAppExport.h"


// NOTE:
//
// For decorators it is assumed that the methods will never be called
// with the self argument as nullptr.  The self argument is the first argument
// for non-static methods.
//

class qCjyxBaseQTAppPythonQtDecorators : public QObject
{
  Q_OBJECT

public:

  qCjyxBaseQTAppPythonQtDecorators() = default;

public slots:

  //----------------------------------------------------------------------------
  // qCjyxApplicationHelper

  //----------------------------------------------------------------------------
  void static_qCjyxApplicationHelper_setupModuleFactoryManager(qCjyxModuleFactoryManager * moduleFactoryManager)
    {
    qCjyxApplicationHelper::setupModuleFactoryManager(moduleFactoryManager);
    }
};

//-----------------------------------------------------------------------------
void initqCjyxBaseQTAppPythonQtDecorators()
{
  PythonQt::self()->addDecorators(new qCjyxBaseQTAppPythonQtDecorators);
}

#endif
