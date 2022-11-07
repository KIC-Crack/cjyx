/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH)
 All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Cjyx

=========================================================================auto=*/

#ifndef __qCjyxBaseQTCorePythonQtDecorators_h
#define __qCjyxBaseQTCorePythonQtDecorators_h

// PythonQt includes
#include <PythonQt.h>

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "qCjyxAbstractCoreModule.h"
#include "qCjyxUtils.h"

#include "qCjyxBaseQTCoreExport.h"

// NOTE:
//
// For decorators it is assumed that the methods will never be called
// with the self argument as nullptr.  The self argument is the first argument
// for non-static methods.
//

class qCjyxBaseQTBasePythonQtDecorators : public QObject
{
  Q_OBJECT
public:

  qCjyxBaseQTBasePythonQtDecorators()
    {
    PythonQt::self()->registerClass(&qCjyxCoreApplication::staticMetaObject);
    PythonQt::self()->registerClass(&qCjyxAbstractCoreModule::staticMetaObject);
    PythonQt::self()->registerCPPClass("qCjyxUtils", nullptr, "qCjyxBaseQTCore");
    // Note: Use registerCPPClassForPythonQt to register pure Cpp classes
    }

public slots:

  //----------------------------------------------------------------------------
  // qCjyxCoreApplication

  //----------------------------------------------------------------------------
  // static methods

  //----------------------------------------------------------------------------
  bool static_qCjyxCoreApplication_testingEnabled()
    {
    return qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::AA_EnableTesting);
    }

  //----------------------------------------------------------------------------
  // instance methods

  //----------------------------------------------------------------------------
  void sendEvent(qCjyxCoreApplication* app, QObject* _receiver, QEvent* _event)
    {
    app->sendEvent(_receiver, _event);
    }

  //----------------------------------------------------------------------------
  void processEvents(qCjyxCoreApplication* app)
    {
    app->processEvents();
    }

  //----------------------------------------------------------------------------
  // qCjyxUtils

  //----------------------------------------------------------------------------
  // static methods

  /// This method is only for documentation stored on the legacy Cjyx wiki.
  /// Instead, use cjyx.readthedocs.io and replaceDcumentationUrlVersion.
  QString static_qCjyxUtils_replaceWikiUrlVersion(const QString& text,
                                                    const QString& version)
  {
    return qCjyxUtils::replaceWikiUrlVersion(text, version);
  }

  QString static_qCjyxUtils_replaceDocumentationUrlVersion(const QString& text,
                                                             const QString& hostname,
                                                             const QString& version)
  {
    return qCjyxUtils::replaceDocumentationUrlVersion(text, hostname, version);
  }
};

//-----------------------------------------------------------------------------
void initqCjyxBaseQTCorePythonQtDecorators()
{
  PythonQt::self()->addDecorators(new qCjyxBaseQTBasePythonQtDecorators);
}

#endif
