#ifndef __qCjyxCropVolumeModule_h
#define __qCjyxCropVolumeModule_h

// Cjyx includes
#include "qCjyxLoadableModule.h"

#include "qCjyxCropVolumeModuleExport.h"

class qCjyxCropVolumeModulePrivate;

/// \ingroup Cjyx_QtModules_CropVolume
class Q_CJYX_QTMODULES_CROPVOLUME_EXPORT qCjyxCropVolumeModule :
  public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");
  Q_INTERFACES(qCjyxLoadableModule);

public:

  typedef qCjyxLoadableModule Superclass;
  explicit qCjyxCropVolumeModule(QObject *parent=nullptr);
  ~qCjyxCropVolumeModule() override;

  qCjyxGetTitleMacro(QTMODULE_TITLE);

  /// Return a custom icon for the module
  QIcon icon()const override;
  QStringList categories() const override;

  QString helpText()const override;
  QString acknowledgementText()const override;
  QStringList contributors()const override;

  QStringList dependencies()const override;

  /// Specify editable node types
  QStringList associatedNodeTypes()const override;

protected:
  /// Initialize the module. Register the volumes reader/writer
  void setup() override;

  /// Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation * createWidgetRepresentation() override;

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

protected:
  QScopedPointer<qCjyxCropVolumeModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxCropVolumeModule);
  Q_DISABLE_COPY(qCjyxCropVolumeModule);

};

#endif
