/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH)
  All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Cjyx

=========================================================================auto=*/

#include "qCjyxCoreApplication.h"
#include "qCjyxModuleManager.h"
#include "qCjyxModuleFactory.h"

#include <cstdlib>
#include <iostream>

#include "vtkMRMLCoreTestingMacros.h"

int qCjyxModuleManagerTest1(int argc, char * argv [] )
{

  // By design, a ModuleManager should be instantiated only if a
  // qCjyxCoreApplication exists and has been initialized.
  // That we will be sure, an ApplicationLogic and a MRMLScene have also been instantiated
  // This enforced in the constructor of qCjyxModuleManager (using Q_ASSERTs)
  qCjyxCoreApplication app(argc, argv);
  app.initialize();

  qCjyxModuleManager moduleManager;

  moduleManager.factory()->registerCoreModules();
  moduleManager.factory()->instantiateCoreModules();

  moduleManager.printAdditionalInfo();

  qCjyxModuleFactory * factory = moduleManager.factory();

  if( factory == nullptr )
    {
    std::cerr << "Error in factory()" << std::endl;
    return EXIT_FAILURE;
    }

  QString moduleName = "qCjyxTransformsModule";

  bool result0 = moduleManager.isLoaded( moduleName );

  if( result0 != false )
    {
    std::cerr << "Error in isLoaded() " << std::endl;
    return EXIT_FAILURE;
    }

  bool result1 = moduleManager.loadModule( moduleName );

  if( result1 == false )
    {
    std::cerr << "Error in loadModule() " << std::endl;
    return EXIT_FAILURE;
    }

  bool result2 = moduleManager.isLoaded( moduleName );

  if( result2 != true )
    {
    std::cerr << "Error in isLoaded() or loadModule() " << std::endl;
    return EXIT_FAILURE;
    }

  qCjyxAbstractModule * module = moduleManager.module( moduleName );

  if( module == nullptr )
    {
    std::cerr << "Error in getModule() " << std::endl;
    return EXIT_FAILURE;
    }

  QString moduleTitle = moduleManager.moduleTitle( moduleName );

  QString moduleName1 = moduleManager.moduleName( moduleTitle );

  if( moduleName != moduleName1 )
    {
    std::cerr << "Error in moduleName recovery" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Module Name = " << qPrintable( moduleName ) << std::endl;

  std::cout << "Module Title = " << qPrintable( moduleTitle ) << std::endl;


  bool result3 = moduleManager.unLoadModule( moduleName );

  if( result3 == false )
    {
    std::cerr << "Error in unLoadModule() " << std::endl;
    return EXIT_FAILURE;
    }

  bool result4 = moduleManager.isLoaded( moduleName );

  if( result4 != false )
    {
    std::cerr << "Error in isLoaded() or loadModule() " << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

