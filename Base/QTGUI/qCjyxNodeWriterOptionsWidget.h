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

#ifndef __qCjyxNodeWriterOptionsWidget_h
#define __qCjyxNodeWriterOptionsWidget_h

/// QtCore includes
#include "qCjyxFileWriterOptionsWidget.h"
class qCjyxNodeWriterOptionsWidgetPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxNodeWriterOptionsWidget
  : public qCjyxFileWriterOptionsWidget
{
  Q_OBJECT
  /// If the storage node doesn't support compression, the option can be hidden
  Q_PROPERTY(bool showUseCompression READ showUseCompression WRITE setShowUseCompression)

public:
  typedef qCjyxFileWriterOptionsWidget Superclass;
  explicit qCjyxNodeWriterOptionsWidget(QWidget* parent = nullptr);
  ~qCjyxNodeWriterOptionsWidget() override;

  bool showUseCompression() const;
  void setShowUseCompression(bool show);

  bool isValid()const override;

public slots:
  void setObject(vtkObject* object) override;

protected slots:
  virtual void setUseCompression(bool use);
  virtual void setCompressionParameter(int index);
  virtual void setCompressionParameter(QString parameter);

protected:
  qCjyxNodeWriterOptionsWidget(qCjyxNodeWriterOptionsWidgetPrivate* pimpl,
    QWidget* parent);

private:
  Q_DECLARE_PRIVATE_D(qGetPtrHelper(qCjyxIOOptions::d_ptr), qCjyxNodeWriterOptionsWidget);
};

#endif
