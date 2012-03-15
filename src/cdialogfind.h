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

#ifndef CDIALOGFIND_H
#define CDIALOGFIND_H

#include <qdialog.h>
#include <ui_find_dlg.h>

#define FIND_OPTIONSFLAG_REGEXP   0x01
#define FIND_OPTIONSFLAG_CASE     0x02
#define FIND_OPTIONSFLAG_WHOLE    0x04
#define FIND_OPTIONSFLAG_CURSOR   0x08
#define FIND_OPTIONSFLAG_BACKWARD 0x10

/**
@author Romain Rollet
*/
class CDialogFind : public QDialog
{
Q_OBJECT
public:
    CDialogFind( QWidget *parent = 0, QStringList *pPrevResearch=NULL, uint32_t iOptionFlags=0);
    ~CDialogFind();
    QString GetTextToFind();
    uint32_t GetOptionFlags();
    void SetOptionFlags(uint32_t iOptionFlags);
    
signals:
    void findnext();
    
private slots:
    void find();
        
private:
  Ui_findDialog m_ui;
  
};

#endif
