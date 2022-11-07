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

#ifndef __qCjyxIOOptionsWidget_h
#define __qCjyxIOOptionsWidget_h

/// QtCore includes
#include "qCjyxBaseQTGUIExport.h"
#include "qCjyxIOOptions.h"
#include "qCjyxWidget.h"
class qCjyxIOOptionsWidgetPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxIOOptionsWidget
  : public qCjyxWidget
  , public qCjyxIOOptions
{
  Q_OBJECT
public:
  typedef qCjyxIOOptions Superclass;
  explicit qCjyxIOOptionsWidget(QWidget* parent = nullptr);
  ~qCjyxIOOptionsWidget() override;

  /// Returns true if the options have been set and if they are
  /// meaningful
  bool isValid()const override;

  // Update GUI widgets based on properties.
  // Derived classes can override this method to set default
  // options on the GUI.
  virtual void updateGUI(const qCjyxIO::IOProperties& ioProperties);

public slots:
  virtual void setFileName(const QString& fileName);
  virtual void setFileNames(const QStringList& fileNames);

signals:
  void validChanged(bool);

protected:
  using Superclass::d_ptr;
  Q_DECLARE_PRIVATE_D(qGetPtrHelper(qCjyxIOOptions::d_ptr), qCjyxIOOptions);
  qCjyxIOOptionsWidget(qCjyxIOOptionsPrivate* pimpl, QWidget* parent);
  void updateValid() override;
};

#endif
