/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH)
 All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Cjyx

=========================================================================auto=*/

#ifndef __qCjyxSubjectHierarchyModuleWidgetsPythonQtDecorators_h
#define __qCjyxSubjectHierarchyModuleWidgetsPythonQtDecorators_h

// PythonQt includes
#include <PythonQt.h>

// Cjyx includes
#include "qCjyxSubjectHierarchyAbstractPlugin.h"
#include "qCjyxSubjectHierarchyPluginHandler.h"

#include "qCjyxSubjectHierarchyModuleWidgetsExport.h"

class QAction;

// NOTE:
//
// For decorators it is assumed that the methods will never be called
// with the self argument as nullptr.  The self argument is the first argument
// for non-static methods.
//

class qCjyxSubjectHierarchyModuleWidgetsPythonQtDecorators : public QObject
{
  Q_OBJECT
public:

  qCjyxSubjectHierarchyModuleWidgetsPythonQtDecorators()
    {
    //PythonQt::self()->registerClass(&qCjyxSubjectHierarchyPluginHandler::staticMetaObject);
    // Note: Use registerCPPClassForPythonQt to register pure Cpp classes
    }

public slots:

  //----------------------------------------------------------------------------
  // qCjyxSubjectHierarchyPluginHandler

  //----------------------------------------------------------------------------
  // static methods

  //----------------------------------------------------------------------------
  qCjyxSubjectHierarchyPluginHandler* static_qCjyxSubjectHierarchyPluginHandler_instance()
    {
    return qCjyxSubjectHierarchyPluginHandler::instance();
    }

  void static_qCjyxSubjectHierarchyAbstractPlugin_setActionPosition(QAction* action, int section, int weight = 0, double weightAdjustment = 0.0)
    {
    qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(action, section, weight, weightAdjustment);
    }

  //----------------------------------------------------------------------------
  // instance methods

  //----------------------------------------------------------------------------
  bool registerPlugin(qCjyxSubjectHierarchyPluginHandler* handler,
                      PythonQtPassOwnershipToCPP<qCjyxSubjectHierarchyAbstractPlugin*> plugin)
    {
    return handler->registerPlugin(plugin);
    }
};

//-----------------------------------------------------------------------------
void initqCjyxSubjectHierarchyModuleWidgetsPythonQtDecorators()
{
  PythonQt::self()->addDecorators(new qCjyxSubjectHierarchyModuleWidgetsPythonQtDecorators);
}

#endif
