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

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#include "qCjyxCoreModule.h"


class ACoreModule: public qCjyxCoreModule
{
public:
  QString title()const override
  {
    return "A title \n\t#$%^&*";
  }
  qCjyxAbstractModuleRepresentation* createWidgetRepresentation() override
  {
    return nullptr;
  }
  vtkMRMLAbstractLogic* createLogic() override
  {
    return nullptr;
  }
};


int qCjyxCoreModuleTest1(int, char * [] )
{
  qCjyxCoreModule* module = new ACoreModule;
  delete module;

  return EXIT_SUCCESS;
}
