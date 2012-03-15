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

#include <QFileDialog>
#include "cdialogsetdirectory.h"

CDialogSetDirectory::CDialogSetDirectory( QStringList *pDirectoryList, uint8_t iDefaultDirRow, QString &qRFCURL, QWidget *parent)
 : QDialog(parent)
{
  int i; 
  QListWidgetItem *newItem;
  
  m_ui.setupUi(this);
  connect(m_ui.addButton, SIGNAL(pressed()), this, SLOT(addDirectoryToList()) );
  connect(m_ui.remButton, SIGNAL(pressed()), this, SLOT(remDirectoryFromList()) );
  connect(m_ui.applyButton, SIGNAL(pressed()), this, SLOT(accept() ) );
  connect(m_ui.cancelButton, SIGNAL(pressed()), this, SLOT(reject() ) );
  connect(m_ui.defaultButton, SIGNAL(pressed()), this, SLOT(setDefaultDirectory() ) );
  m_ui.urlLineEdit->setText(qRFCURL);
  
  if (pDirectoryList!=NULL)
  {
    for (i = 0; i < pDirectoryList->size(); i++) 
    {
       newItem = new QListWidgetItem( pDirectoryList->at(i) );  
       if (i==iDefaultDirRow)
       {
         QFont qFont=newItem->font();
         qFont.setBold(true);
         newItem->setFont( qFont );        
       }
       m_ui.directoryList->addItem(newItem);
    }
  }
  m_iDefaultDirRow=iDefaultDirRow;
}


CDialogSetDirectory::~CDialogSetDirectory()
{
}

int CDialogSetDirectory::GetDirectoryList(QStringList *pDirectoryList)
{
  int i;
  
  pDirectoryList->clear();
  for (i=0;i<m_ui.directoryList->count();i++)
  {      
    pDirectoryList->append( m_ui.directoryList->item(i)->text() );
  }
  return m_iDefaultDirRow;
}

void CDialogSetDirectory::addDirectoryToList()
{
  QString qDirectoryName = QFileDialog::getExistingDirectory( this, "Choose a directory");
  
  if (!qDirectoryName.isEmpty())
  {
    QListWidgetItem *newItem = new QListWidgetItem(qDirectoryName);
    m_ui.directoryList->addItem(newItem);    
  }  
}

void CDialogSetDirectory::remDirectoryFromList()
{
  QListWidgetItem *pCurrentItem=m_ui.directoryList->currentItem();
  if (pCurrentItem!=NULL)
  {
    // Update the default index
    if (m_iDefaultDirRow>m_ui.directoryList->currentRow())
      m_iDefaultDirRow--;
    m_ui.directoryList->takeItem(m_ui.directoryList->currentRow() );
    delete pCurrentItem;
  }
}

void CDialogSetDirectory::setDefaultDirectory()
{
  QListWidgetItem *pCurrentItem=m_ui.directoryList->currentItem();
  QListWidgetItem *pPrevDefaultItem=m_ui.directoryList->item(m_iDefaultDirRow);
  
  if (pCurrentItem!=NULL)
  {    
    QFont qFont=pCurrentItem->font();
    pPrevDefaultItem->setFont( qFont );    
    qFont.setBold(true);
    pCurrentItem->setFont( qFont );
    m_iDefaultDirRow=m_ui.directoryList->currentRow();
  }
}

