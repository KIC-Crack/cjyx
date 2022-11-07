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

  This file was originally developed by Kyle Sunderland, PerkLab, Queen's University
  and was supported through CANARIE's Research Software Program, and Cancer
  Care Ontario.

==============================================================================*/

#ifndef __qCjyxTextsReader_h
#define __qCjyxTextsReader_h

// Cjyx includes
#include "qCjyxFileReader.h"
class qCjyxTextsReaderPrivate;

//-----------------------------------------------------------------------------
class qCjyxTextsReader : public qCjyxFileReader
{
  Q_OBJECT
public:
  typedef qCjyxFileReader Superclass;
  qCjyxTextsReader(QObject* parent = nullptr);
  ~qCjyxTextsReader() override;

  QString description() const override;
  IOFileType fileType() const override;
  QStringList extensions() const override;

  bool load(const IOProperties& properties) override;

protected:
  QScopedPointer< qCjyxTextsReaderPrivate > d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxTextsReader);
  Q_DISABLE_COPY(qCjyxTextsReader);
};

#endif
