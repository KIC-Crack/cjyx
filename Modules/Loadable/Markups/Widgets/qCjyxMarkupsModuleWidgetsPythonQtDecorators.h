/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH)
 All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Cjyx

=========================================================================auto=*/

#ifndef __qCjyxMarkupsModuleWidgetsPythonQtDecorators_h
#define __qCjyxMarkupsModuleWidgetsPythonQtDecorators_h

// PythonQt includes
#include <PythonQt.h>

// Cjyx includes
#include "qMRMLMarkupsOptionsWidgetsFactory.h"

#include "qCjyxMarkupsModuleWidgetsExport.h"

// NOTE:
//
// For decorators it is assumed that the methods will never be called
// with the self argument as nullptr.  The self argument is the first argument
// for non-static methods.
//

class qCjyxMarkupsModuleWidgetsPythonQtDecorators : public QObject
{
  Q_OBJECT
public:

  qCjyxMarkupsModuleWidgetsPythonQtDecorators()
    {
    //PythonQt::self()->registerClass(&qMRMLMarkupsOptionsWidgetsFactory::staticMetaObject);
    // Note: Use registerCPPClassForPythonQt to register pure Cpp classes
    }

public slots:

  //----------------------------------------------------------------------------
  // qMRMLMarkupsOptionsWidgetsFactory

  //----------------------------------------------------------------------------
  // static methods

  //----------------------------------------------------------------------------
  qMRMLMarkupsOptionsWidgetsFactory* static_qMRMLMarkupsOptionsWidgetsFactory_instance()
    {
    return qMRMLMarkupsOptionsWidgetsFactory::instance();
    }

  //----------------------------------------------------------------------------
  // instance methods

  //----------------------------------------------------------------------------
  bool registerOptionsWidget(qMRMLMarkupsOptionsWidgetsFactory* factory,
                                       PythonQtPassOwnershipToCPP<qMRMLMarkupsAbstractOptionsWidget*> plugin)
    {
    return factory->registerOptionsWidget(plugin);
    }
};

//-----------------------------------------------------------------------------
void initqCjyxMarkupsModuleWidgetsPythonQtDecorators()
{
  PythonQt::self()->addDecorators(new qCjyxMarkupsModuleWidgetsPythonQtDecorators);
}

#endif
