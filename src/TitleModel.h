    /****************************************************************************
    **
    ** qRFCView, A smart IETF RFC viewer based on the Qt4 library.
    ** Copyright (C) 2005 Mitsubishi Electric ITE-TCL, R. Rollet (rollet@tcl.ite.mee.com)
    ** Copyright (C) 2005-2005 Trolltech AS. All rights reserved.
    **
    ** This file is part of the documentation of the Qt Toolkit.
    **
    ** This file may be used under the terms of the GNU General Public
    ** License version 2.0 as published by the Free Software Foundation
    ** and appearing in the file LICENSE.GPL included in the packaging of
    ** this file.  Please review the following information to ensure GNU
    ** General Public Licensing requirements will be met:
    ** http://www.trolltech.com/products/qt/opensource.html
    **
    ** If you are unsure which license is appropriate for your use, please
    ** review the following information:
    ** http://www.trolltech.com/products/qt/licensing.html or contact the
    ** sales department at sales@trolltech.com.
    **
    ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
    ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
    **
    ****************************************************************************/
    
#ifndef TITLEMODEL_H
#define TITLEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#define MAX_TITLE_LEVEL 16

class CTitleItem;

class CTitleModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    CTitleModel(const QString &data, QObject *parent = 0);
    ~CTitleModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool ParseTitle(const QString &qLevel, const QString &qTitle);

private:
    uint8_t m_iCurrentValidTitleNum[MAX_TITLE_LEVEL];      
    uint8_t m_iCurrentValidTitleLevel;
    CTitleItem *m_pCurrentTitleItem[MAX_TITLE_LEVEL];     

    CTitleItem *rootItem;
};

#endif
