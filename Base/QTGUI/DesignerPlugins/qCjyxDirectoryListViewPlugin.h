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
  and was partially funded by NIH grant 1U24CA194354-01

==============================================================================*/

#ifndef __qCjyxDirectoryListViewPlugin_h
#define __qCjyxDirectoryListViewPlugin_h

#include "qCjyxQTGUIAbstractPlugin.h"

class Q_CJYX_DESIGNER_PLUGINS_EXPORT qCjyxDirectoryListViewPlugin
  : public QObject,
    public qCjyxQTGUIAbstractPlugin
{
  Q_OBJECT
public:
  qCjyxDirectoryListViewPlugin(QObject* parent = nullptr);

  QWidget *createWidget(QWidget *_parent) override;
  QString domXml() const override;
  QString includeFile() const override;
  bool isContainer() const override;
  QString name() const override;
};

#endif
