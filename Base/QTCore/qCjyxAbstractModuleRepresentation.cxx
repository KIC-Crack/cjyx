/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH)
 All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Cjyx

=========================================================================auto=*/

// Qt includes
#include <QDebug>

// Cjyx includes
#include "qCjyxAbstractCoreModule.h"
#include "qCjyxAbstractModuleRepresentation.h"

// MRMLLogic includes
#include "vtkMRMLAbstractLogic.h"

// VTK includes
#include <vtkWeakPointer.h>

//-----------------------------------------------------------------------------
class qCjyxAbstractModuleRepresentationPrivate
{
public:
  qCjyxAbstractModuleRepresentationPrivate();
  vtkWeakPointer<vtkMRMLAbstractLogic> Logic;
  qCjyxAbstractCoreModule*           Module;
};

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentationPrivate
::qCjyxAbstractModuleRepresentationPrivate()
{
  this->Module = nullptr;
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation::qCjyxAbstractModuleRepresentation()
  : d_ptr(new qCjyxAbstractModuleRepresentationPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation::~qCjyxAbstractModuleRepresentation()
{
  Q_D(qCjyxAbstractModuleRepresentation);
  if (d->Module)
    {
    d->Module->representationDeleted(this);
    }
}

//-----------------------------------------------------------------------------
QString qCjyxAbstractModuleRepresentation::moduleName()const
{
  Q_D(const qCjyxAbstractModuleRepresentation);
  return d->Module->name();
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxAbstractModuleRepresentation::logic()const
{
  Q_D(const qCjyxAbstractModuleRepresentation);
  return d->Logic.GetPointer();
}

//-----------------------------------------------------------------------------
qCjyxAbstractCoreModule* qCjyxAbstractModuleRepresentation::module()const
{
  Q_D(const qCjyxAbstractModuleRepresentation);
  return d->Module;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleRepresentation::setModule(qCjyxAbstractCoreModule* module)
{
  Q_D(qCjyxAbstractModuleRepresentation);
  d->Module = module;
  d->Logic = module ? module->logic() : nullptr;
  this->setup();
}

//-----------------------------------------------------------
bool qCjyxAbstractModuleRepresentation::setEditedNode(vtkMRMLNode* node,
                                                        QString role /* = QString()*/,
                                                        QString context /* = QString() */)
{
  Q_UNUSED(node);
  Q_UNUSED(role);
  Q_UNUSED(context);
  qWarning() << Q_FUNC_INFO << " failed: method is not implemented in " << this->moduleName();
  return false;
}

//-----------------------------------------------------------
double qCjyxAbstractModuleRepresentation::nodeEditable(vtkMRMLNode* node)
{
  Q_UNUSED(node);
  // It is assumed that only associated nodes will be tried to be edited,
  // so most of the time using the recommended neutral confidence value is
  // reasonable. If a module is more or less confident than default
  // then that module has to override this method.
  return 0.5;
}
