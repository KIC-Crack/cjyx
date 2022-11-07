/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Kyle Sunderland, PerkLab, Queen's University
  and was supported through CANARIE's Research Software Program, Cancer
  Care Ontario, OpenAnatomy, and Brigham and Women�s Hospital through NIH grant R01MH112748.

==============================================================================*/

#ifndef __qCjyxMarkupsWriter_h
#define __qCjyxMarkupsWriter_h

// QtCore includes
#include "qCjyxMarkupsModuleExport.h"
#include "qCjyxNodeWriter.h"

class vtkMRMLNode;
class vtkMRMLStorableNode;

/// Utility class that offers writing of markups in both json format, regardless of the current storage node.
class Q_CJYX_QTMODULES_MARKUPS_EXPORT qCjyxMarkupsWriter
  : public qCjyxNodeWriter
{
  Q_OBJECT
public:
  typedef qCjyxNodeWriter Superclass;
  qCjyxMarkupsWriter(QObject* parent);
  ~qCjyxMarkupsWriter() override;

  QStringList extensions(vtkObject* object)const override;

  bool write(const qCjyxIO::IOProperties& properties) override;

  void setStorageNodeClass(vtkMRMLStorableNode* storableNode, const QString& storageNodeClassName);

private:
  Q_DISABLE_COPY(qCjyxMarkupsWriter);
};

#endif
