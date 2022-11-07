/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// DICOMLib includes
#include "qCjyxDICOMExportable.h"
#include "vtkCjyxDICOMExportable.h"

// CTK includes
#include <ctkPimpl.h>

//-----------------------------------------------------------------------------
class qCjyxDICOMExportablePrivate
{
public:
  qCjyxDICOMExportablePrivate();
  virtual ~qCjyxDICOMExportablePrivate();

  /// Name exposed to the user for the export method
  QString Name;
  /// Extra information the user sees on mouse over of the export option
  QString Tooltip;
  /// ID of the subject hierarchy item to be exported
  vtkIdType SubjectHierarchyItemID;
  /// Class of the plugin that created this exportable
  QString PluginClass;
  /// Target directory to export this exportable
  QString Directory;
  /// Confidence - from 0 to 1 where 0 means that the plugin
  /// cannot export the given node, up to 1 that means that the
  /// plugin considers itself the best plugin to export the node
  /// (in case of specialized objects, e.g. RT dose volume)
  double Confidence;
  /// Pseudo-tags offered by the plugin that are to be filled out for export.
  /// The pseudo-tags are translated into real DICOM tags at the time of export.
  /// It tag is a pair of strings (name, value). When the exportable is created
  /// by the DICOM plugin, value is the default value that is set in the editor widget
  QMap<QString,QString> Tags;
};

//-----------------------------------------------------------------------------
// qCjyxDICOMExportablePrivate methods

//-----------------------------------------------------------------------------
qCjyxDICOMExportablePrivate::qCjyxDICOMExportablePrivate()
{
  this->Name = QString("Unknown exporter");
  this->Tooltip = QString("Creates a DICOM file from the selected data");
  this->Confidence = 0.0;
  this->SubjectHierarchyItemID = 0;
}

//-----------------------------------------------------------------------------
qCjyxDICOMExportablePrivate::~qCjyxDICOMExportablePrivate() = default;


//-----------------------------------------------------------------------------
// qCjyxDICOMExportable methods

//-----------------------------------------------------------------------------
qCjyxDICOMExportable::qCjyxDICOMExportable(QObject* parentObject)
  : Superclass(parentObject)
  , d_ptr(new qCjyxDICOMExportablePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxDICOMExportable::~qCjyxDICOMExportable() = default;

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxDICOMExportable, const QString&, setName, Name)
CTK_GET_CPP(qCjyxDICOMExportable, QString, name, Name)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxDICOMExportable, const QString&, setTooltip, Tooltip)
CTK_GET_CPP(qCjyxDICOMExportable, QString, tooltip, Tooltip)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxDICOMExportable, const vtkIdType&, setSubjectHierarchyItemID, SubjectHierarchyItemID)
CTK_GET_CPP(qCjyxDICOMExportable, vtkIdType, subjectHierarchyItemID, SubjectHierarchyItemID)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxDICOMExportable, const QString&, setPluginClass, PluginClass)
CTK_GET_CPP(qCjyxDICOMExportable, QString, pluginClass, PluginClass)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxDICOMExportable, const QString&, setDirectory, Directory)
CTK_GET_CPP(qCjyxDICOMExportable, QString, directory, Directory)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxDICOMExportable, const double, setConfidence, Confidence)
CTK_GET_CPP(qCjyxDICOMExportable, double, confidence, Confidence)

//-----------------------------------------------------------------------------
QMap<QString,QString> qCjyxDICOMExportable::tags()const
{
  Q_D(const qCjyxDICOMExportable);
  return d->Tags;
}
//-----------------------------------------------------------------------------
void qCjyxDICOMExportable::setTags(const QMap<QString,QString>& var)
{
  Q_D(qCjyxDICOMExportable);
  d->Tags = var;
}

//-----------------------------------------------------------------------------
QString qCjyxDICOMExportable::tag(QString tagName)
{
  Q_D(qCjyxDICOMExportable);
  // Returns QString() if tagName is not in the tags map, which contains Null value for QString
  return d->Tags[tagName];
}
//-----------------------------------------------------------------------------
void qCjyxDICOMExportable::setTag(QString tagName, QString tagValue)
{
  Q_D(qCjyxDICOMExportable);
  d->Tags[tagName] = tagValue;
}

//-----------------------------------------------------------------------------
void qCjyxDICOMExportable::copyToVtkExportable(vtkCjyxDICOMExportable* vtkExportable)
{
  Q_D(qCjyxDICOMExportable);

  if (!vtkExportable)
    {
    return;
    }

  vtkExportable->SetName(d->Name.toUtf8().constData());
  vtkExportable->SetTooltip(d->Tooltip.toUtf8().constData());
  vtkExportable->SetSubjectHierarchyItemID(d->SubjectHierarchyItemID);
  vtkExportable->SetPluginClass(d->PluginClass.toUtf8().constData());
  vtkExportable->SetDirectory(d->Directory.toUtf8().constData());
  vtkExportable->SetConfidence(d->Confidence);

  QMapIterator<QString,QString> tagsIt(d->Tags);
  while (tagsIt.hasNext())
    {
    tagsIt.next();
    vtkExportable->SetTag(tagsIt.key().toUtf8().constData(), tagsIt.value().toUtf8().constData());
    }
}

//-----------------------------------------------------------------------------
void qCjyxDICOMExportable::copyFromVtkExportable(vtkCjyxDICOMExportable* vtkExportable)
{
  Q_D(qCjyxDICOMExportable);

  if (!vtkExportable)
    {
    return;
    }

  d->Name = QString(vtkExportable->GetName());
  d->Tooltip = QString(vtkExportable->GetTooltip());
  d->SubjectHierarchyItemID = vtkExportable->GetSubjectHierarchyItemID();
  d->PluginClass = QString(vtkExportable->GetPluginClass());
  d->Directory = QString(vtkExportable->GetDirectory());
  d->Confidence = vtkExportable->GetConfidence();

  std::map<std::string, std::string> vtkTags = vtkExportable->GetTags();
  for ( std::map<std::string, std::string>::iterator it=vtkTags.begin(); it != vtkTags.end(); ++it )
    {
    this->setTag(it->first.c_str(), it->second.c_str());
    }
}
