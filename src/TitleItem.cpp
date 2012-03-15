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

#include <QStringList>

#include "TitleItem.h"

CTitleItem::CTitleItem(const QString &qTitle, const QString &qAnchor, CTitleItem *parent)
{
    parentItem = parent;
    m_qTitle= qTitle;
    m_qAnchor=qAnchor;
}

CTitleItem::~CTitleItem()
{
    qDeleteAll(childItems);
}

void CTitleItem::appendChild(CTitleItem *item)
{
    childItems.append(item);
}

CTitleItem *CTitleItem::child(int row)
{
    return childItems.value(row);
}

int CTitleItem::childCount() const
{
    return childItems.count();
}

CTitleItem *CTitleItem::parent()
{
    return parentItem;
}

int CTitleItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<CTitleItem*>(this));

    return 0;
}
