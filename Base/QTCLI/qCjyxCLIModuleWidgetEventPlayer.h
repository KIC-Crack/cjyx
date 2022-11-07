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

  This file was originally developed by Benjamin LONG, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxCLIModuleWidgetEventPlayer_h
#define __qCjyxCLIModuleWidgetEventPlayer_h

// QtTesting includes
#include <pqWidgetEventPlayer.h>

// QtCLI includes
#include "qCjyxBaseQTCLIExport.h"

class Q_CJYX_BASE_QTCLI_EXPORT qCjyxCLIModuleWidgetEventPlayer : public pqWidgetEventPlayer
{
  Q_OBJECT

public:
  typedef pqWidgetEventPlayer Superclass;
  qCjyxCLIModuleWidgetEventPlayer(QObject* parent = nullptr);

  using Superclass::playEvent;
  bool playEvent(QObject *Object, const QString &Command, const QString &Arguments, bool &Error) override;

private:
  qCjyxCLIModuleWidgetEventPlayer(const qCjyxCLIModuleWidgetEventPlayer&); // NOT implemented
  qCjyxCLIModuleWidgetEventPlayer& operator=(const qCjyxCLIModuleWidgetEventPlayer&); // NOT implemented
};

#endif // __qCjyxCLIModuleWidgetEventPlayer_h
