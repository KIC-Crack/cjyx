#ifndef __qCjyxAnnotationsModule_h
#define __qCjyxAnnotationsModule_h

// Cjyx includes
#include "qCjyxLoadableModule.h"

// CTK includes
#include <ctkPimpl.h>

#include "qCjyxAnnotationsModuleExport.h"

class qCjyxAbstractModuleWidget;
class qCjyxAnnotationsModulePrivate;

/// \ingroup Cjyx_QtModules_Annotation
class Q_CJYX_QTMODULES_ANNOTATIONS_EXPORT qCjyxAnnotationsModule :
  public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");
  Q_INTERFACES(qCjyxLoadableModule);
public:

  typedef qCjyxLoadableModule Superclass;
  qCjyxAnnotationsModule(QObject *parent=nullptr);
  ~qCjyxAnnotationsModule() override;

  /// Return the help and acknowledgement text for the Annotation module.
  QString helpText()const override;
  QString acknowledgementText()const override;
  QStringList contributors()const override;
  QStringList dependencies() const override;

  /// Return the icon of the Annotation module.
  QIcon icon()const override;

  QStringList categories()const override;

  /// Specify editable node types
  QStringList associatedNodeTypes()const override;

  qCjyxGetTitleMacro(QTMODULE_TITLE);

protected:

  /// All initialization code should be done in the setup
  void setup() override;

  // Description:
  // Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation* createWidgetRepresentation() override;

  // Description:
  // Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

public slots:
  /// a public slot to open up the screen  capture
  /// dialog (get the module manager, get the module annotation, get the
  /// widget representation, then invoke this method
  /// \sa qCjyxIOManager::openScreenshotDialog
  void showScreenshotDialog();

protected:
  QScopedPointer<qCjyxAnnotationsModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxAnnotationsModule);
  Q_DISABLE_COPY(qCjyxAnnotationsModule);
};

#endif
