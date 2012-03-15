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

#ifndef QRFCEDITOR_H
#define QRFCEDITOR_H

#include <qtextbrowser.h>
class QStringList;

/**
@author Romain Rollet
*/
class QRFCEditor : public QTextBrowser
{
Q_OBJECT
public:
    QRFCEditor(QWidget *parent = 0);

    ~QRFCEditor();
  
    void setSource ( const QUrl & name );
    void scrollToAnchor2 ( const QString & name );
    bool isBackwardAvailable ();
    bool isForwardAvailable ();
    
signals:
    void RFCReq(uint32_t); // Signal emitted when a new RFC is required    
public slots:
    void backward ();
    void forward ();
    
private:
    QList<int> m_qPositionPath;
    int m_iCurrentPositionIdx;
};

#endif
