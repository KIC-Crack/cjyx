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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxCLIModuleFactoryHelper_h
#define __qCjyxCLIModuleFactoryHelper_h

/// Qt includes
#include <QStringList>

#include "qCjyxBaseQTCLIExport.h"

class Q_CJYX_BASE_QTCLI_EXPORT qCjyxCLIModuleFactoryHelper
{
public:

  ///
  /// Convenient method returning a list of module paths that the factory could use
  static const QStringList modulePaths();

  /// Convenient method returning True if the given CLI path corresponds to an installed module
  static bool isInstalled(const QString& path);

  /// Convenient method returning True if the given CLI path corresponds to a built-in module
  static bool isBuiltIn(const QString& path);

private:
  /// Not implemented
  qCjyxCLIModuleFactoryHelper() = default;
  virtual ~qCjyxCLIModuleFactoryHelper() = default;
};

#endif
