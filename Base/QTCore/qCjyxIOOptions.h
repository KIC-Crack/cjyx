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

#ifndef __qCjyxIOOptions_h
#define __qCjyxIOOptions_h

// Qt includes
#include <QScopedPointer>

/// QtCore includes
#include "qCjyxIO.h"
#include "qCjyxBaseQTCoreExport.h"
class qCjyxIOOptionsPrivate;

class Q_CJYX_BASE_QTCORE_EXPORT qCjyxIOOptions
{
public:
  /// Constructor initialize empty properties
  explicit qCjyxIOOptions();
  virtual ~qCjyxIOOptions();

  /// Returns true if the options have been set and if they are
  /// meaningful. By default, checks that there is at least 1 option.
  /// To be reimplemented in subclasses
  virtual bool isValid()const;

  const qCjyxIO::IOProperties& properties()const;
protected:
  qCjyxIOOptions(qCjyxIOOptionsPrivate* pimpl);
  QScopedPointer<qCjyxIOOptionsPrivate> d_ptr;

  /// Must be called anytime the result of isValid() can change
  virtual void updateValid();

private:
  Q_DECLARE_PRIVATE(qCjyxIOOptions);
  Q_DISABLE_COPY(qCjyxIOOptions);
};

#endif
