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

#ifndef CDIALOGSETDIRECTORY_H
#define CDIALOGSETDIRECTORY_H
#include <QStringList>
#include <ui_setdirectory_dlg.h>

/**
@author Romain Rollet
*/

class CDialogSetDirectory : public QDialog
{
Q_OBJECT
public:
    CDialogSetDirectory(QStringList *pDirectoryList, uint8_t iDefaultDir, QString &qRFCURL, QWidget *parent = 0);

    ~CDialogSetDirectory();
    int GetDirectoryList(QStringList *pDirectoryList);
    uint8_t GetDefaultDir() {return m_iDefaultDirRow;}
    QString GetRFCURL() {return m_ui.urlLineEdit->text();}
    
private slots:
    void addDirectoryToList();
    void remDirectoryFromList();  
    void setDefaultDirectory();
    
private:
  Ui_directoryDialog m_ui;
  uint8_t m_iDefaultDirRow;  
};

#endif
