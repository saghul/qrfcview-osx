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

#ifndef TITLEITEM_H
#define TITLEITEM_H

#include <QList>
#include <QVariant>

class CTitleItem
{
public:
    CTitleItem(const QString &,const QString &, CTitleItem *parent=0);
    ~CTitleItem();

    void appendChild(CTitleItem *child);

    CTitleItem *child(int row);
    int childCount() const;
    int columnCount() const {return 1;}
    QString GetTitle() {return m_qTitle;}
    
    int row() const;
    CTitleItem *parent();
    QString GetAnchor() {return m_qAnchor;}
    
private:
    QList<CTitleItem*> childItems;
    QString m_qTitle;
    CTitleItem *parentItem;
    QString m_qAnchor;
};

#endif
