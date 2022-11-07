// Qt includes
#include <QButtonGroup>
#include <QDebug>
#include <QFileDialog>
#include <QFontMetrics>
#include <QList>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QPushButton>

// Cjyx includes
#include "qCjyxCoreApplication.h"

// Annotations includes
#include "qCjyxAnnotationModuleReportDialog.h"
#include "vtkMRMLAnnotationNode.h"
#include "vtkMRMLAnnotationHierarchyNode.h"

// MRML includes
#include <vtkMRMLScene.h>

//---------------------------------------------------------------------------
qCjyxAnnotationModuleReportDialog::qCjyxAnnotationModuleReportDialog()
{
  this->m_Logic = nullptr;

  this->m_Annotations = nullptr;

  this->ui.setupUi(this);

  // The restore button has to be configured since it is the reset button in the buttonbox
  // so we set Icons and Text here
  QPushButton* restoreButton = ui.buttonBox->button(QDialogButtonBox::Reset);
  restoreButton->setText("Print");
  restoreButton->setIcon(QIcon(":/Icons/AnnotationPrint.png"));

  ui.titleEdit->setText("Annotation Report");

  this->createConnection();
}

//---------------------------------------------------------------------------
qCjyxAnnotationModuleReportDialog::~qCjyxAnnotationModuleReportDialog()
{
  if (this->m_Logic)
    {
    this->m_Logic = nullptr;
    }

  if (this->m_Annotations)
    {
    this->m_Annotations->Delete();
    this->m_Annotations = nullptr;
    }
}

//---------------------------------------------------------------------------
void qCjyxAnnotationModuleReportDialog::createConnection()
{
  // connect the OK and CANCEL button to the individual Slots
  this->connect(this, SIGNAL(rejected()), this, SLOT(onDialogRejected()));
  this->connect(this, SIGNAL(accepted()), this, SLOT(onDialogAccepted()));

  QPushButton* cancelButton = ui.buttonBox->button(QDialogButtonBox::Cancel);
  this->connect(cancelButton, SIGNAL(clicked()), this, SLOT(onDialogRejected()));

  QPushButton* saveButton = ui.buttonBox->button(QDialogButtonBox::Save);
  this->connect(saveButton, SIGNAL(clicked()), this, SLOT(onDialogAccepted()));

  // connect the Print button
  QPushButton* printButton = ui.buttonBox->button(QDialogButtonBox::Reset);
  this->connect(printButton, SIGNAL(clicked()), this, SLOT(onPrintButtonClicked()));

  this->connect(ui.titleEdit, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited()));
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleReportDialog::setLogic(vtkCjyxAnnotationModuleLogic* logic)
{
  if (!logic)
    {
    qErrnoWarning("setLogic: We need the Annotation module logic here!");
    return;
    }

  this->m_Logic = logic;
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleReportDialog::setAnnotations(vtkCollection* collection)
{
  if (!collection)
    {
    qErrnoWarning("setAnnotations: we need a vtkCollection here!");
    return;
    }

  this->m_Annotations = vtkCollection::New();

  vtkObject *obj = nullptr;
  vtkCollectionSimpleIterator it;
  for (collection->InitTraversal(it);
    (obj = collection->GetNextItemAsObject(it));)
    {
    this->m_Annotations->AddItem(obj);
    }
}

//---------------------------------------------------------------------------
/*void qCjyxAnnotationModuleReportDialog::onSaveReportButtonClicked()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Save Report",
        QDir::currentPath(),
        "Reports (*.html)");

    // save the documents...
    if ( !filename.isNull())
    {
        m_filename = filename;
        emit filenameSelected();
    }

}*/

//---------------------------------------------------------------------------
QString qCjyxAnnotationModuleReportDialog::generateReport()
{
  QString html = "<html>\n";
  html.append("<head>\n");
  html.append("<meta name=\"Author\" content=\"Daniel Haehn, Kilian Pohl, Yong Zhang\">\n");
  html.append("<title>");
  html.append("3D Cjyx: ");
  html.append(ui.titleEdit->text());
  html.append("</title>\n");
  html.append("<style type=\"text/css\">\n");
  html.append("body {font-family: Helvetica, Arial;}\n");
  html.append(".heading {background-color:#a3a3a3;}\n");
  html.append("</style>\n");
  html.append("<body>\n");

  html.append("<h1>");
  html.append("3D Cjyx: ");
  html.append(ui.titleEdit->text());
  html.append("</h1><br>\n");

  html.append("<table border=0 width='100%' cellPadding=3 cellSpacing=0>\n");

  html.append("<tr><td class='heading'><b>Type</b></td><td class='heading'><b>Value</b></td><td class='heading'><b>Text<b></td></tr>\n");

  this->m_Html = QString("");

  vtkMRMLAnnotationHierarchyNode *toplevelNode = nullptr;
  char *toplevelNodeID = this->m_Logic->GetTopLevelHierarchyNodeID(nullptr);
  if (toplevelNodeID && this->m_Logic->GetMRMLScene() &&
      this->m_Logic->GetMRMLScene()->GetNodeByID(toplevelNodeID))
    {
//    toplevelNode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(this->m_Logic->GetMRMLScene()->GetNodeByID(toplevelNodeID));
    toplevelNode = this->m_Logic->GetActiveHierarchyNode();
    this->generateReportRecursive(0,toplevelNode);
    }
  html.append(this->m_Html);

  html.append("</table>\n");

  html.append("</body>\n");
  html.append("</html>");

  return html;
}

//---------------------------------------------------------------------------
bool qCjyxAnnotationModuleReportDialog::isAnnotationSelected(const char* mrmlId)
{
  vtkMRMLNode *node = nullptr;
  vtkCollectionSimpleIterator it;
  for (this->m_Annotations->InitTraversal(it);
    (node = vtkMRMLNode::SafeDownCast(this->m_Annotations->GetNextItemAsObject(it)));)
    {
    if (node != nullptr && !strcmp(mrmlId, node->GetID()))
      {
      // we found it
      return true;
      }
    }

  return false;
}

//---------------------------------------------------------------------------
void qCjyxAnnotationModuleReportDialog::generateReportRecursive(int level, vtkMRMLAnnotationHierarchyNode* currentHierarchy)
{
  vtkCollection* children = vtkCollection::New();
  currentHierarchy->GetDirectChildren(children);


  vtkMRMLNode *node = nullptr;
  vtkCollectionSimpleIterator it;
  for (this->m_Annotations->InitTraversal(it);
    (node = vtkMRMLNode::SafeDownCast(this->m_Annotations->GetNextItemAsObject(it)));)
    {
    // loop through all children

    vtkMRMLAnnotationNode* annotationNode = vtkMRMLAnnotationNode::SafeDownCast(node);
    if (annotationNode)
      {
      // this child is an annotationNode

      // now check if the child was selected
      if (this->isAnnotationSelected(annotationNode->GetID()))
        {
        // the node was selected in the annotation treeView, so add it to the report
        this->m_Html.append(QString(this->m_Logic->GetHTMLRepresentation(annotationNode,level)));
        }

      continue;

      } // annotationNode

    vtkMRMLAnnotationHierarchyNode* hierarchyNode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(node);
    if (hierarchyNode)
      {
      // this child is a user created hierarchyNode

      // now check if the child was selected
      if (this->isAnnotationSelected(hierarchyNode->GetID()))
        {
        // the node was selected in the annotation treeView, so add it to the report
        this->m_Html.append(QString(this->m_Logic->GetHTMLRepresentation(hierarchyNode,level)));
        }

      // anyway, we need to recursively start again at this hierarchy
      this->generateReportRecursive(level+1,hierarchyNode);

      } // hierarchyNode

    } // loop through children
}

//---------------------------------------------------------------------------
void qCjyxAnnotationModuleReportDialog::updateReport()
{
  this->ui.reportBrowser->setHtml(this->generateReport());
}

//---------------------------------------------------------------------------
void qCjyxAnnotationModuleReportDialog::onTextEdited()
{
  this->updateReport();
}

//---------------------------------------------------------------------------
void qCjyxAnnotationModuleReportDialog::onPrintButtonClicked()
{
  QPrinter printer;

  QPrintDialog *dialog = new QPrintDialog(&printer, this);
  dialog->setWindowTitle(tr("Print Annotation Report"));

  if( dialog->exec() == QDialog::Accepted )
  {

    this->ui.reportBrowser->print(&printer);
  }
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleReportDialog::onDialogRejected()
{
  // emit an event which gets caught by main GUI window
  emit dialogRejected();
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleReportDialog::onDialogAccepted()
{
  if (this->saveReport())
    {
    // emit an event which gets caught by main GUI window
    emit dialogAccepted();
    }
}

//-----------------------------------------------------------------------------
bool qCjyxAnnotationModuleReportDialog::saveReport()
{
  QString filename = QFileDialog::getSaveFileName(this, "Save Annotation Report", QString(), "3D Cjyx Annotation Report (*.html)");

  QString report = this->generateReport();

  if (filename.length() > 0)
    {

    // make sure the selected file ends with .html
    if ((!filename.endsWith(".html")) && (!filename.endsWith(".HTML")))
      {
      filename.append(".html");
      }

    // check, if we can write the file
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text))
      {
      std::cerr << "Error: Cannot save file " << qPrintable(filename) << ": "
          << qPrintable(file.errorString()) << std::endl;
      return false;
      }
    QTextStream out(&file);

    // create a directory for the graphics
    QString imgdir(filename);
    imgdir.remove(imgdir.size() - 5, 5);
    imgdir.append("_files");
    QDir currentdir = QDir::current();

    if (currentdir.exists())
      {
      if (!currentdir.mkdir(imgdir))
        {
        std::cerr << "Error: cannot make directory" << std::endl;
        }
      }

    // save a relative path to the imgdir
    QStringList list = imgdir.split("/");
    QString imgshortdir = list[list.size() - 1];

    // replace all Qt icon and image paths with real filepathes
    QString tempPath = qCjyxCoreApplication::application()->temporaryPath();

    report.replace(QString(":/Icons/"), imgshortdir.append("/"));
    report.replace(tempPath, imgshortdir.append("/"));

    // now save all graphics
    vtkMRMLNode *node = nullptr;
    vtkCollectionSimpleIterator it;
    for (this->m_Annotations->InitTraversal(it);
      (node = vtkMRMLNode::SafeDownCast(this->m_Annotations->GetNextItemAsObject(it)));)
      {
      vtkMRMLAnnotationNode* annotationNode = vtkMRMLAnnotationNode::SafeDownCast(node);
      vtkMRMLAnnotationHierarchyNode* hierarchyNode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(node);

      if (annotationNode)
        {

        if (annotationNode->IsA("vtkMRMLAnnotationSnapshotNode"))
          {
          QString tempPath = qCjyxCoreApplication::application()->temporaryPath();
          tempPath.append(annotationNode->GetID());
          tempPath.append(".png");

          QFile screenshotHolder(tempPath);
          QString outPath = imgdir;
          screenshotHolder.copy(outPath.append("/").append(QString(annotationNode->GetID())).append(QString(".png")));
          }

        // all annotation icons
        QString iconPath = QString(annotationNode->GetIcon());
        QImage iconHolder = QImage(iconPath);
        iconHolder.save(iconPath.replace(QString(":/Icons"), imgdir));
        }
      else if (hierarchyNode)
        {
        QString iconPath = QString(hierarchyNode->GetIcon());
        QImage iconHolder = QImage(iconPath);
        iconHolder.save(iconPath.replace(QString(":/Icons"), imgdir));
        }
      } // for loop through all selected nodes

    out << report;
    file.close();

    return true;
    } // file dialog accepted

  return false; // file dialog cancelled
}
