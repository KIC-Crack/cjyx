#ifndef __qCjyxSceneViewsModule_h
#define __qCjyxSceneViewsModule_h

// Cjyx includes
#include "qCjyxLoadableModule.h"

// CTK includes
#include <ctkPimpl.h>

#include "qCjyxSceneViewsModuleExport.h"

class qCjyxAbstractModuleWidget;
class qCjyxSceneViewsModulePrivate;

/// \ingroup Cjyx_QtModules_SceneViews
class Q_CJYX_QTMODULES_SCENEVIEWS_EXPORT qCjyxSceneViewsModule :
  public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");
  Q_INTERFACES(qCjyxLoadableModule);
public:

  typedef qCjyxLoadableModule Superclass;
  qCjyxSceneViewsModule(QObject *parent=nullptr);
  ~qCjyxSceneViewsModule() override;

  /// Return the help and acknowledgement text for the SceneViews module.
  QString helpText()const override ;
  QString acknowledgementText()const override;
  QStringList contributors()const override;

  /// Return the icon of the SceneViews module.
  QIcon icon()const override;
  QStringList categories()const override;

  /// Specify editable node types
  QStringList associatedNodeTypes()const override;

  qCjyxGetTitleMacro(QTMODULE_TITLE);

public slots:
    /// a public slot to open up the scene view capture
    /// dialog (get the module manager, get the module sceneviews, get the
    /// widget representation, then invoke this method
    /// \sa qCjyxIOManager::openSceneViewsDialog
    void showSceneViewDialog();

protected:

  /// All initialization code should be done in the setup
  void setup() override;

  // Description:
  // Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation* createWidgetRepresentation() override;

  // Description:
  // Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

protected:
  QScopedPointer<qCjyxSceneViewsModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSceneViewsModule);
  Q_DISABLE_COPY(qCjyxSceneViewsModule);
};

#endif
