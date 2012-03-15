/****************************************************************************
  
    qRFCView, A smart IETF RFC viewer based on the Qt4 library.
    Copyright (C) 2005 Mitsubishi Electric ITE-TCL, R. Rollet (rollet@tcl.ite.mee.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*********************************************************************************/
    
#include "cdialogfind.h"
#include <QDebug>
CDialogFind::CDialogFind(QWidget *parent, QStringList *pPrevResearch, uint32_t iOptionFlags)
 : QDialog(parent)
{
  m_ui.setupUi(this);
  connect(m_ui.findButton, SIGNAL(pressed()), this, SLOT(find() ) );
}


CDialogFind::~CDialogFind()
{
}

void CDialogFind::find()
{
  // Add the search text to the combo
  if (m_ui.textComboBox->findText( m_ui.textComboBox->currentText() )==-1)
    m_ui.textComboBox->addItem(m_ui.textComboBox->currentText());
  emit findnext();  
}

QString CDialogFind::GetTextToFind()
{
  return m_ui.textComboBox->currentText();
}


uint32_t CDialogFind::GetOptionFlags()
{
  uint32_t iFlags=0;
  
  if (m_ui.regexpCheckBox->checkState() == Qt::Checked)
    iFlags|=FIND_OPTIONSFLAG_REGEXP;
  if (m_ui.wholeCheckBox->checkState() == Qt::Checked)
    iFlags|=FIND_OPTIONSFLAG_WHOLE;
  if (m_ui.caseCheckBox->checkState() == Qt::Checked)
    iFlags|=FIND_OPTIONSFLAG_CASE;
  if (m_ui.cursorCheckBox->checkState() == Qt::Checked)
    iFlags|=FIND_OPTIONSFLAG_CURSOR;
  if (m_ui.backwardCheckBox->checkState() == Qt::Checked)
    iFlags|=FIND_OPTIONSFLAG_BACKWARD;
  return iFlags;  
}

void CDialogFind::SetOptionFlags(uint32_t iOptionFlags)
{
  m_ui.regexpCheckBox->setCheckState((iOptionFlags&FIND_OPTIONSFLAG_REGEXP)?Qt::Checked:Qt::Unchecked);
  m_ui.wholeCheckBox->setCheckState((iOptionFlags&FIND_OPTIONSFLAG_WHOLE)?Qt::Checked:Qt::Unchecked);
  m_ui.caseCheckBox->setCheckState((iOptionFlags&FIND_OPTIONSFLAG_CASE)?Qt::Checked:Qt::Unchecked);
  m_ui.cursorCheckBox->setCheckState((iOptionFlags&FIND_OPTIONSFLAG_CURSOR)?Qt::Checked:Qt::Unchecked);
  m_ui.backwardCheckBox->setCheckState((iOptionFlags&FIND_OPTIONSFLAG_BACKWARD)?Qt::Checked:Qt::Unchecked);
}

