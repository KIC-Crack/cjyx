#ifndef __qCjyxSceneViewsModuleWidget_h
#define __qCjyxSceneViewsModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"
#include "qCjyxSceneViewsModuleExport.h"

// CTK includes
#include <ctkPimpl.h>
#include <ctkVTKObject.h>


class qCjyxSceneViewsModuleDialog;
class vtkMRMLSceneViewNode;
class qCjyxSceneViewsModuleWidgetPrivate;

class vtkMRMLNode;

class QUrl;

/// \ingroup Cjyx_QtModules_SceneViews
class Q_CJYX_QTMODULES_SCENEVIEWS_EXPORT qCjyxSceneViewsModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT
  QVTK_OBJECT
public:
    typedef qCjyxAbstractModuleWidget Superclass;
    qCjyxSceneViewsModuleWidget(QWidget *parent=nullptr);
    ~qCjyxSceneViewsModuleWidget() override;

  /// Set up the GUI from mrml when entering
  void enter() override;
  /// Disconnect from scene when exiting
  void exit() override;

  bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;

public slots:
    /// a public slot allowing other modules to open up the scene view capture
    /// dialog (get the module manager, get the module sceneviews, get the
    /// widget representation, then invoke this method, see qCjyxIOManager openSceneViewsDialog
    void showSceneViewDialog();

    /// User clicked on restore button
    void restoreSceneView(const QString& mrmlId);

    /// User clicked on property edit button
    void editSceneView(const QString& mrmlId);

    /// scene was closed or imported or restored or finished batch
    /// processing, reset as necessary
    void onMRMLSceneReset();

protected slots:

  void onSceneViewDoubleClicked(int row, int column);

  void onRestoreButtonClicked();
  void onEditButtonClicked();
  void onDeleteButtonClicked();

  void moveDownSelected(QString mrmlId);
  void moveUpSelected(QString mrmlId);

  /// Respond to scene events
  void onMRMLSceneEvent(vtkObject*, vtkObject* node);

  /// respond to mrml events
  void updateFromMRMLScene();

protected:
  QScopedPointer<qCjyxSceneViewsModuleWidgetPrivate> d_ptr;

  void setup() override;

private:
  Q_DECLARE_PRIVATE(qCjyxSceneViewsModuleWidget);
  Q_DISABLE_COPY(qCjyxSceneViewsModuleWidget);
};

#endif
