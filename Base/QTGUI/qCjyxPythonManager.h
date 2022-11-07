/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH)
 All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Cjyx

=========================================================================auto=*/

#ifndef __qCjyxPythonManager_h
#define __qCjyxPythonManager_h

// Cjyx includes
#include "qCjyxCorePythonManager.h"
#include "qCjyxBaseQTGUIExport.h"

class vtkObject;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxPythonManager : public qCjyxCorePythonManager
{
  Q_OBJECT

public:
  typedef qCjyxCorePythonManager Superclass;
  qCjyxPythonManager(QObject* parent=nullptr);
  ~qCjyxPythonManager() override;

protected:

  void preInitialization() override;
  void executeInitializationScripts() override;

private:

  /// This is the callback helper function that isolates the event broker from
  /// knowing about any particular scripting implementation of observations code.
  static void eventBrokerScriptHandler(const char *script, void *clientData);

};

#endif
