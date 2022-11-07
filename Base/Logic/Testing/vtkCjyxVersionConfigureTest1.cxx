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

#include "vtkCjyxVersionConfigure.h"

// vtkAddon includes
#include <vtkAddonTestingMacros.h>

//-----------------------------------------------------------------------------
int vtkCjyxVersionConfigureTest1(int /*argc*/, char * /*argv*/ [])
{
  // From vtkCjyxVersionConfigure
  CHECK_STRING_DIFFERENT(Cjyx_VERSION, "");
  CHECK_STRING_DIFFERENT(Cjyx_VERSION_FULL, "");
  CHECK_STRING_DIFFERENT(Cjyx_BUILDDATE, "");
  CHECK_STRING_DIFFERENT(Cjyx_WC_URL, "");
  CHECK_STRING_DIFFERENT(Cjyx_WC_REVISION, "");
  CHECK_STRING_DIFFERENT(Cjyx_REVISION, "");

  // From vtkCjyxVersionConfigureInternal
  CHECK_STRING(Cjyx_OS_LINUX_NAME, "linux");
  CHECK_STRING(Cjyx_OS_MAC_NAME, "macosx");
  CHECK_STRING(Cjyx_OS_WIN_NAME, "win");
  CHECK_STRING_DIFFERENT(Cjyx_ARCHITECTURE, "");
  CHECK_STRING_DIFFERENT(Cjyx_OS, "");

  return EXIT_SUCCESS;
}
