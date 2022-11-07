#include <QApplication>

// Cjyx includes
#include "vtkCjyxConfigure.h"

// VTK includes
#include "qMRMLWidget.h"

// STD includes
#include <cstdlib>

int qMRMLWidgetsExportTest1( int argc, char * argv [] )
{
  qMRMLWidget::preInitializeApplication();
  QApplication app(argc, argv);
  qMRMLWidget::postInitializeApplication();

  // qMRMLWidgetsExport   mrmlItem;

  return EXIT_SUCCESS;
}
