/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH)
 All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Cjyx

=========================================================================auto=*/

#ifndef __qCjyxBaseQTGUIPythonQtDecorators_h
#define __qCjyxBaseQTGUIPythonQtDecorators_h

// PythonQt includes
#include <PythonQt.h>

// CTK includes
#include <ctkErrorLogModel.h>

#include "vtkCjyxConfigure.h" // For Cjyx_USE_QtTesting

// CTK includes
#ifdef Cjyx_USE_QtTesting
# include <ctkQtTestingUtility.h>
#endif

// Cjyx includes
#include "qCjyxAbstractModuleRepresentation.h"
#include "qCjyxAbstractModuleWidget.h"
#include "qCjyxAbstractModule.h"
#include "qCjyxCommandOptions.h"
#include "qCjyxPythonManager.h"

#include "qCjyxBaseQTGUIExport.h"


// NOTE:
//
// For decorators it is assumed that the methods will never be called
// with the self argument as nullptr.  The self argument is the first argument
// for non-static methods.
//

class qCjyxBaseQTGUIPythonQtDecorators : public QObject
{
  Q_OBJECT

public:

  qCjyxBaseQTGUIPythonQtDecorators()
    {
    PythonQt::self()->registerClass(&qCjyxAbstractModuleWidget::staticMetaObject);
    PythonQt::self()->registerClass(&qCjyxPythonManager::staticMetaObject);
    PythonQt::self()->registerClass(&qCjyxCommandOptions::staticMetaObject);
#ifdef Cjyx_USE_QtTesting
    PythonQt::self()->registerClass(&ctkQtTestingUtility::staticMetaObject);
#endif
    PythonQt::self()->registerClass(&ctkErrorLogModel::staticMetaObject);
    PythonQt::self()->registerClass(&ctkErrorLogTerminalOutput::staticMetaObject);
    // Note: Use registerCPPClassForPythonQt to register pure Cpp classes
    }

public slots:

  //----------------------------------------------------------------------------
  // qCjyxAbstractModule

  //----------------------------------------------------------------------------
  qCjyxAbstractModuleWidget* widgetRepresentation(qCjyxAbstractModule* _module)
    {
    return dynamic_cast<qCjyxAbstractModuleWidget*>(_module->widgetRepresentation());
    }

  //----------------------------------------------------------------------------
  qCjyxAbstractModuleWidget* createNewWidgetRepresentation(qCjyxAbstractModule* _module)
    {
    return dynamic_cast<qCjyxAbstractModuleWidget*>(_module->createNewWidgetRepresentation());
    }

  //----------------------------------------------------------------------------
  // qCjyxAbstractModule

  //----------------------------------------------------------------------------
  qCjyxAbstractModule* module(qCjyxAbstractModuleWidget * _moduleWidget)
    {
    return dynamic_cast<qCjyxAbstractModule*>(_moduleWidget->module());
    }
};

//-----------------------------------------------------------------------------
void initqCjyxBaseQTGUIPythonQtDecorators()
{
  PythonQt::self()->addDecorators(new qCjyxBaseQTGUIPythonQtDecorators);
}

#endif
