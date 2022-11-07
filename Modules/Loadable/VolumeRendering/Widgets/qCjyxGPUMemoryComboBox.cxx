/*==============================================================================

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
  and was supported through CANARIE's Research Software Program, and Cancer
  Care Ontario.

==============================================================================*/

// qCjyxVolumeRendering includes
#include "qCjyxGPUMemoryComboBox.h"

// VTK includes
#include <vtkNew.h>
#include <vtkGPUInfo.h>
#include <vtkGPUInfoList.h>

// Qt includes
#include <QDebug>
#include <QLineEdit>

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_VolumeRendering
class qCjyxGPUMemoryComboBoxPrivate
{
  Q_DECLARE_PUBLIC(qCjyxGPUMemoryComboBox);
protected:
  qCjyxGPUMemoryComboBox* const q_ptr;

public:
  qCjyxGPUMemoryComboBoxPrivate(qCjyxGPUMemoryComboBox& object);
  virtual ~qCjyxGPUMemoryComboBoxPrivate();

  void init();

  double memoryFromString(const QString& memory)const;
  QString memoryToString(double memory)const;

  QRegExp MemoryRegExp;
  QString DefaultText;
};

//-----------------------------------------------------------------------------
// qCjyxGPUMemoryComboBoxPrivate methods

//-----------------------------------------------------------------------------
qCjyxGPUMemoryComboBoxPrivate::qCjyxGPUMemoryComboBoxPrivate(
  qCjyxGPUMemoryComboBox& object)
  : q_ptr(&object)
  , DefaultText("0 MB (Default)")
{
  this->MemoryRegExp = QRegExp("^(\\d+(?:\\.\\d*)?)\\s?(MB|GB|\\%)$");
}

//-----------------------------------------------------------------------------
qCjyxGPUMemoryComboBoxPrivate::~qCjyxGPUMemoryComboBoxPrivate() = default;

//-----------------------------------------------------------------------------
void qCjyxGPUMemoryComboBoxPrivate::init()
{
  Q_Q(qCjyxGPUMemoryComboBox);

  q->setEditable(true);
  q->lineEdit()->setValidator( new QRegExpValidator(this->MemoryRegExp, q));
  q->addItem(DefaultText);
  //q->addItem(qCjyxGPUMemoryComboBox::tr("25 %")); //TODO: Uncomment when totalGPUMemoryInMB works
  //q->addItem(qCjyxGPUMemoryComboBox::tr("50 %"));
  //q->addItem(qCjyxGPUMemoryComboBox::tr("75 %"));
  //q->addItem(qCjyxGPUMemoryComboBox::tr("90 %"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("128 MB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("256 MB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("512 MB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("1024 MB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("1.5 GB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("2 GB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("3 GB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("4 GB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("6 GB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("8 GB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("12 GB"));
  q->addItem(qCjyxGPUMemoryComboBox::tr("16 GB"));
  q->insertSeparator(1);

  // Detect the amount of memory in the graphic card and set it as default
  int gpuMemoryInMB = q->totalGPUMemoryInMB();
  if (gpuMemoryInMB > 0)
    {
    q->setCurrentGPUMemory(gpuMemoryInMB);
    }
}

// --------------------------------------------------------------------------
double qCjyxGPUMemoryComboBoxPrivate::memoryFromString(const QString& memory)const
{
  if (memory == this->DefaultText)
    {
    return 0.0;
    }

  int pos = this->MemoryRegExp.indexIn(memory);
  if (pos < 0)
    {
    return 0.0;
    }

  QString memoryValue = this->MemoryRegExp.cap(1);
  double value = memoryValue.toDouble();
  QString memoryUnit = this->MemoryRegExp.cap(2);

  if (memoryUnit == "%")
    {
    return value / 100.0;
    }
  else if (memoryUnit == "GB")
    {
    return value * 1024.0;
    }
  return value;
}

// --------------------------------------------------------------------------
QString qCjyxGPUMemoryComboBoxPrivate::memoryToString(double memory)const
{
  if (memory == 0.0)
    {
    return this->DefaultText;
    }
  if (memory < 1.0)
    {
    return QString::number(static_cast<int>(memory * 100)) + " %";
    }
  if (memory > 1024.0)
    {
    return QString::number(static_cast<float>(memory) / 1024) + " GB";
    }
  return QString::number(static_cast<int>(memory)) + " MB";
}


//-----------------------------------------------------------------------------
// qCjyxGPUMemoryComboBox methods

// --------------------------------------------------------------------------
qCjyxGPUMemoryComboBox::qCjyxGPUMemoryComboBox(QWidget* parentWidget)
  : Superclass(parentWidget)
  , d_ptr(new qCjyxGPUMemoryComboBoxPrivate(*this))
{
  Q_D(qCjyxGPUMemoryComboBox);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxGPUMemoryComboBox::~qCjyxGPUMemoryComboBox() = default;

//-----------------------------------------------------------------------------
int qCjyxGPUMemoryComboBox::totalGPUMemoryInMB()const
{
  // Detect the amount of memory in the graphic card
  vtkNew<vtkGPUInfoList> gpuInfoList;
  gpuInfoList->Probe();

  if (gpuInfoList->GetNumberOfGPUs() > 0)
    {
    int gpuMemoryInBytes = gpuInfoList->GetGPUInfo(0)->GetDedicatedVideoMemory();
    int gpuMemoryInKB = gpuMemoryInBytes / 1024;
    int gpuMemoryInMB = gpuMemoryInKB / 1024;
    return gpuMemoryInMB;
    }

  return 0;
}

// --------------------------------------------------------------------------
double qCjyxGPUMemoryComboBox::currentGPUMemory()const
{
  Q_D(const qCjyxGPUMemoryComboBox);

  QString memoryString = this->currentText();
  return d->memoryFromString(memoryString);
}

// --------------------------------------------------------------------------
int qCjyxGPUMemoryComboBox::currentGPUMemoryInMB()const
{
  Q_D(const qCjyxGPUMemoryComboBox);

  QString memoryString = this->currentText();
  if (memoryString == d->DefaultText)
    {
    return 0;
    }
  double memory = d->memoryFromString(memoryString);
  if (memory < 1.0)
    {
    int gpuMemoryInMB = this->totalGPUMemoryInMB();
    if (gpuMemoryInMB == 0)
      {
      return 0;
      }
    return static_cast<int>(memory * gpuMemoryInMB);
    }
  return static_cast<int>(memory);
}

// --------------------------------------------------------------------------
QString qCjyxGPUMemoryComboBox::currentGPUMemoryAsString()const
{
  return this->currentText();
}

// --------------------------------------------------------------------------
void qCjyxGPUMemoryComboBox::setCurrentGPUMemory(double memory)
{
  Q_D(qCjyxGPUMemoryComboBox);

  QString memoryString = d->memoryToString(memory);
  this->setCurrentGPUMemoryFromString(memoryString);
}

// --------------------------------------------------------------------------
void qCjyxGPUMemoryComboBox::setCurrentGPUMemoryFromString(const QString& memoryString)
{
  int index = this->findText(memoryString);
  if (index == -1)
    {
    int customIndex = 0;
    this->setItemText(customIndex, memoryString);
    index = customIndex;
    }
  this->setCurrentIndex(index);
}
