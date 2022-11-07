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

#include "qCjyxIOOptions.h"
#include "qCjyxIOOptions_p.h"

//------------------------------------------------------------------------------
qCjyxIOOptionsPrivate::~qCjyxIOOptionsPrivate() = default;

//------------------------------------------------------------------------------
qCjyxIOOptions::qCjyxIOOptions()
  : d_ptr(new qCjyxIOOptionsPrivate)
{
}

//------------------------------------------------------------------------------
qCjyxIOOptions::qCjyxIOOptions(qCjyxIOOptionsPrivate* pimpl)
  : d_ptr(pimpl)
{
}

//------------------------------------------------------------------------------
qCjyxIOOptions::~qCjyxIOOptions() = default;

//------------------------------------------------------------------------------
bool qCjyxIOOptions::isValid()const
{
  Q_D(const qCjyxIOOptions);
  return d->Properties.size() > 0;
}

//------------------------------------------------------------------------------
const qCjyxIO::IOProperties& qCjyxIOOptions::properties()const
{
  Q_D(const qCjyxIOOptions);
  return d->Properties;
}

//------------------------------------------------------------------------------
void qCjyxIOOptions::updateValid()
{
  Q_D(qCjyxIOOptions);
  d->ArePropertiesValid = this->isValid();
}
