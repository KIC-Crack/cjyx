// CjyxLogic includes
#include "vtkCjyxSceneViewsModuleLogic.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLSceneViewNode.h>
#include <vtkMRMLSceneViewStorageNode.h>

// VTK includes
#include <vtkImageData.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

// STD includes
#include <string>
#include <iostream>
#include <sstream>

//-----------------------------------------------------------------------------
vtkStandardNewMacro(vtkCjyxSceneViewsModuleLogic)

//-----------------------------------------------------------------------------
// vtkCjyxSceneViewsModuleLogic methods
//-----------------------------------------------------------------------------
vtkCjyxSceneViewsModuleLogic::vtkCjyxSceneViewsModuleLogic() = default;

//-----------------------------------------------------------------------------
vtkCjyxSceneViewsModuleLogic::~vtkCjyxSceneViewsModuleLogic() = default;

//-----------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkDebugMacro("SetMRMLSceneInternal - listening to scene events");

  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
//  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndCloseEvent);
  events->InsertNextValue(vtkMRMLScene::EndImportEvent);
  events->InsertNextValue(vtkMRMLScene::EndRestoreEvent);
//  events->InsertNextValue(vtkMRMLScene::SceneAboutToBeRestoredEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
  vtkDebugMacro("OnMRMLSceneNodeAddedEvent");
}

//-----------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::OnMRMLSceneEndImport()
{
  vtkDebugMacro("OnMRMLSceneEndImport");
}

//-----------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::OnMRMLSceneEndRestore()
{
  vtkDebugMacro("OnMRMLSceneEndRestore");
}

//-----------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::OnMRMLNodeModified(vtkMRMLNode* vtkNotUsed(node))
{
}

//-----------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::OnMRMLSceneEndClose()
{
}


//-----------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::RegisterNodes()
{

  if (!this->GetMRMLScene())
    {
    std::cerr << "RegisterNodes: no scene on which to register nodes" << std::endl;
    return;
    }

  vtkMRMLSceneViewNode* viewNode = vtkMRMLSceneViewNode::New();
  this->GetMRMLScene()->RegisterNodeClass(viewNode);
  // SceneSnapshot backward compatibility
#if MRML_APPLICATION_SUPPORT_VERSION < MRML_VERSION_CHECK(4, 0, 0)
  this->GetMRMLScene()->RegisterNodeClass(viewNode, "SceneSnapshot");
#endif
  viewNode->Delete();

  vtkMRMLSceneViewStorageNode *storageNode = vtkMRMLSceneViewStorageNode::New();
  this->GetMRMLScene()->RegisterNodeClass ( storageNode );
  storageNode->Delete();
}

//---------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::CreateSceneView(const char* name, const char* description, int screenshotType, vtkImageData* screenshot)
{
  if (!this->GetMRMLScene())
    {
    vtkErrorMacro("No scene set.");
    return;
    }

  if (!screenshot)
    {
    vtkErrorMacro("CreateSceneView: No screenshot was set.");
    return;
    }

  vtkStdString nameString = vtkStdString(name);

  vtkNew<vtkMRMLSceneViewNode> newSceneViewNode;
  newSceneViewNode->SetScene(this->GetMRMLScene());
  if (strcmp(nameString,""))
    {
    // a name was specified
    newSceneViewNode->SetName(nameString.c_str());
    }
  else
    {
    // if no name is specified, generate a new unique one
    newSceneViewNode->SetName(this->GetMRMLScene()->GetUniqueNameByString("SceneView"));
    }

  vtkStdString descriptionString = vtkStdString(description);

  newSceneViewNode->SetSceneViewDescription(descriptionString);
  newSceneViewNode->SetScreenShotType(screenshotType);

  // make a new vtk image data, as the set macro is taking the pointer
  vtkNew<vtkImageData> copyScreenShot;
  copyScreenShot->DeepCopy(screenshot);
  newSceneViewNode->SetScreenShot(copyScreenShot.GetPointer());
  newSceneViewNode->StoreScene();
  //newSceneViewNode->HideFromEditorsOff();
  // mark it modified since read so that the screen shot will get saved to disk

  this->GetMRMLScene()->AddNode(newSceneViewNode.GetPointer());
}

//---------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::
         ModifySceneView(vtkStdString id,
                         const char* name,
                         const char* description,
                         int vtkNotUsed(screenshotType),
                         vtkImageData* screenshot)
{
  if (!this->GetMRMLScene())
    {
    vtkErrorMacro("No scene set.");
    return;
    }

  if (!screenshot)
    {
    vtkErrorMacro("ModifySceneView: No screenshot was set.");
    return;
    }

  vtkMRMLSceneViewNode* viewNode = vtkMRMLSceneViewNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(id.c_str()));

  if (!viewNode)
    {
    vtkErrorMacro("GetSceneViewName: Could not get sceneView node!");
    return;
    }

  vtkStdString nameString = vtkStdString(name);
  if (strcmp(nameString,""))
    {
    // a name was specified
    viewNode->SetName(nameString.c_str());
    }
  else
    {
    // if no name is specified, generate a new unique one
    viewNode->SetName(this->GetMRMLScene()->GetUniqueNameByString("SceneView"));
    }

  vtkStdString descriptionString = vtkStdString(description);
  viewNode->SetSceneViewDescription(descriptionString);
  // only the text is allowed to be modified, not the screen shot type nor the
  // screen shot image, so don't resave them
  // see also qMRMLScreenShotDialog::grabScreenShot()

  // TODO: Listen to the node directly, probably in OnMRMLSceneNodeAddedEvent
  this->OnMRMLNodeModified(viewNode);
}

//---------------------------------------------------------------------------
vtkStdString vtkCjyxSceneViewsModuleLogic::GetSceneViewName(const char* id)
{
  if (!this->GetMRMLScene())
    {
    vtkErrorMacro("No scene set.");
    return nullptr;
    }

  vtkMRMLSceneViewNode* viewNode = vtkMRMLSceneViewNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(id));

  if (!viewNode)
    {
    vtkErrorMacro("GetSceneViewName: Could not get sceneView node!");
    return nullptr;
    }

  return vtkStdString(viewNode->GetName());
}

//---------------------------------------------------------------------------
vtkStdString vtkCjyxSceneViewsModuleLogic::GetSceneViewDescription(const char* id)
{
  if (!this->GetMRMLScene())
    {
    vtkErrorMacro("No scene set.");
    return nullptr;
    }

  vtkMRMLSceneViewNode* viewNode = vtkMRMLSceneViewNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(id));

  if (!viewNode)
    {
    vtkErrorMacro("GetSceneViewDescription: Could not get sceneView node!");
    return nullptr;
    }

  return viewNode->GetSceneViewDescription();
}

//---------------------------------------------------------------------------
int vtkCjyxSceneViewsModuleLogic::GetSceneViewScreenshotType(const char* id)
{
  if (!this->GetMRMLScene())
    {
    vtkErrorMacro("No scene set.");
    return -1;
    }

  vtkMRMLSceneViewNode* viewNode = vtkMRMLSceneViewNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(id));

  if (!viewNode)
    {
    vtkErrorMacro("GetSceneViewScreenshotType: Could not get sceneView node!");
    return -1;
    }

  return viewNode->GetScreenShotType();
}

//---------------------------------------------------------------------------
vtkImageData* vtkCjyxSceneViewsModuleLogic::GetSceneViewScreenshot(const char* id)
{
  if (!this->GetMRMLScene())
    {
    vtkErrorMacro("No scene set.");
    return nullptr;
    }

  vtkMRMLSceneViewNode* viewNode = vtkMRMLSceneViewNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(id));

  if (!viewNode)
    {
    vtkErrorMacro("GetSceneViewScreenshot: Could not get sceneView node!");
    return nullptr;
    }

  return viewNode->GetScreenShot();
}

//---------------------------------------------------------------------------
bool vtkCjyxSceneViewsModuleLogic::RestoreSceneView(const char* id, bool removeNodes)
{
  if (!this->GetMRMLScene())
    {
    vtkErrorMacro("No scene set.");
    return true;
    }

  vtkMRMLSceneViewNode* viewNode = vtkMRMLSceneViewNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(id));

  if (!viewNode)
    {
    vtkErrorMacro("RestoreSceneView: Could not get sceneView node!");
    return true;
    }

  return viewNode->RestoreScene(removeNodes);
}

//---------------------------------------------------------------------------
const char* vtkCjyxSceneViewsModuleLogic::MoveSceneViewUp(const char* vtkNotUsed(id))
{
  // reset stringHolder
  this->m_StringHolder = "";

  vtkErrorMacro("MoveSceneViewUp: operation not supported!");
  return this->m_StringHolder.c_str();
}

//---------------------------------------------------------------------------
const char* vtkCjyxSceneViewsModuleLogic::MoveSceneViewDown(const char* vtkNotUsed(id))
{
  // reset stringHolder
  this->m_StringHolder = "";

  vtkErrorMacro("MoveSceneViewDown: operation not supported!");
  return this->m_StringHolder.c_str();
}

//---------------------------------------------------------------------------
void vtkCjyxSceneViewsModuleLogic::RemoveSceneViewNode(vtkMRMLSceneViewNode *sceneViewNode)
{
  if (!sceneViewNode)
    {
    vtkErrorMacro("RemoveSceneViewNode: No node to remove");
    return;
    }

  if (!this->GetMRMLScene())
    {
    vtkErrorMacro("RemoveSceneViewNode: No MRML Scene found from which to remove the node");
    return;
    }

  this->GetMRMLScene()->RemoveNode(sceneViewNode);
}
