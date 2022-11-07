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

#ifndef __qCjyxFileWriterOptionsWidget_h
#define __qCjyxFileWriterOptionsWidget_h

/// QtGUI includes
#include "qCjyxIOOptionsWidget.h"
class qCjyxFileWriterOptionsWidgetPrivate;

/// Base class for all the Writer Options widget.
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxFileWriterOptionsWidget
  : public qCjyxIOOptionsWidget
{
  Q_OBJECT

public:
  typedef qCjyxIOOptionsWidget Superclass;
  explicit qCjyxFileWriterOptionsWidget(QWidget* parent = nullptr);
  ~qCjyxFileWriterOptionsWidget() override;

public slots:
  /// Set the object to write (typically a scene or a MRML node)
  virtual void setObject(vtkObject* object);

protected:
  qCjyxFileWriterOptionsWidget(qCjyxIOOptionsPrivate* pimpl,
                                 QWidget* parent);
};

#endif
