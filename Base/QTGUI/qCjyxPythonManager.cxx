/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH)
 All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Cjyx

=========================================================================auto=*/

// Qt includes
#include <QDebug>
#include <QVariant>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxPythonManager.h"

//-----------------------------------------------------------------------------
qCjyxPythonManager::qCjyxPythonManager(QObject* _parent) : Superclass(_parent)
{
}

//-----------------------------------------------------------------------------
qCjyxPythonManager::~qCjyxPythonManager() = default;

//-----------------------------------------------------------------------------
void qCjyxPythonManager::preInitialization()
{
  Superclass::preInitialization();
}

//-----------------------------------------------------------------------------
void qCjyxPythonManager::executeInitializationScripts()
{
  qCjyxApplication* app = qCjyxApplication::application();
  if (!app)
    {
    return;
    }

  // Evaluate application script
  this->executeFile(app->cjyxHome() + "/bin/Python/cjyx/cjyxqt.py");
}

//-----------------------------------------------------------------------------
void qCjyxPythonManager::eventBrokerScriptHandler(const char *script, void *clientData)
{
  Q_UNUSED(script);
  Q_UNUSED(clientData);
}
