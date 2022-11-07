/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

=========================================================================auto=*/

// vtkCjyxBaseCLIExport
//
// The vtkCjyxBaseCLIExport captures some system differences between Unix
// and Windows operating systems.

#ifndef __vtkCjyxBaseCLIExport_h
#define __vtkCjyxBaseCLIExport_h

#include <vtkCjyxBaseCLIConfigure.h>

#if defined(WIN32) && !defined(VTKCJYX_STATIC)
#if defined(CjyxBaseCLI_EXPORTS)
#define VTK_CJYX_BASE_CLI_EXPORT __declspec( dllexport )
#else
#define VTK_CJYX_BASE_CLI_EXPORT __declspec( dllimport )
#endif
#else
#define VTK_CJYX_BASE_CLI_EXPORT
#endif

#endif
