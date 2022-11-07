/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxSettingsCachePanel_h
#define __qCjyxSettingsCachePanel_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkSettingsPanel.h>
#include <ctkVTKObject.h>

// QtGUI includes
#include "qCjyxBaseQTGUIExport.h"

class qCjyxSettingsCachePanelPrivate;
class vtkCacheManager;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxSettingsCachePanel
  : public ctkSettingsPanel
{
  Q_OBJECT
  QVTK_OBJECT
public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;

  /// Constructor
  explicit qCjyxSettingsCachePanel(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxSettingsCachePanel() override;

  virtual void setCacheManager(vtkCacheManager* cacheManager);

public slots:
  void setCachePath(const QString& path);
  void setCacheSize(int sizeInMB);
  void setCacheFreeBufferSize(int sizeInMB);
  void setForceRedownload(bool force);
  void clearCache();

protected slots:
  void updateFromCacheManager();

protected:
  QScopedPointer<qCjyxSettingsCachePanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSettingsCachePanel);
  Q_DISABLE_COPY(qCjyxSettingsCachePanel);
};

#endif
