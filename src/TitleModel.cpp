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
    
#include <QtGui>
#include "TitleItem.h"
#include "TitleModel.h"

CTitleModel::CTitleModel(const QString &qRFCName, QObject *parent)
    : QAbstractItemModel(parent)
{
  int iLevel;
  
  rootItem = new CTitleItem(qRFCName, 0);
  //setHeaderData(1, Qt::Horizontal, QString("Title"), Qt::DisplayRole);
          
  for (iLevel=0;iLevel<MAX_TITLE_LEVEL;iLevel++)
  {
    m_iCurrentValidTitleNum[iLevel]=0;
    m_pCurrentTitleItem[iLevel]=NULL;
  }
  m_iCurrentValidTitleLevel=0;  
}

CTitleModel::~CTitleModel()
{
    delete rootItem;
}

int CTitleModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<CTitleItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant CTitleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    CTitleItem *item = static_cast<CTitleItem*>(index.internalPointer());
    return item->GetTitle();
}

Qt::ItemFlags CTitleModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant CTitleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}


QModelIndex CTitleModel::index(int row, int column, const QModelIndex &parent)
            const
{
    CTitleItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<CTitleItem*>(parent.internalPointer());

    CTitleItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex CTitleModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CTitleItem *childItem = static_cast<CTitleItem*>(index.internalPointer());
    CTitleItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CTitleModel::rowCount(const QModelIndex &parent) const
{
    CTitleItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<CTitleItem*>(parent.internalPointer());

    return parentItem->childCount();
}

bool CTitleModel::ParseTitle(const QString &qLevel, const QString &qTitle)
{ // Return true if title is valid      
  int iLastDot=0,iNextDot;
  bool bOK;
  int iTitleLevel, iLevel;
  CTitleItem *pParentItem, *pChildItem;   
  uint8_t iTitleNum[MAX_TITLE_LEVEL];
  iTitleLevel=0;
  iLastDot=0;
  
  //qDebug() << qLevel+": "+qTitle;
  // Convert title level from ascii to binary (=>iTitleNum)
  while ( iTitleLevel<MAX_TITLE_LEVEL)
  {
    if ( (iNextDot=(qLevel.indexOf(QChar('.'), iLastDot ))) != -1)
    {
      iTitleNum[iTitleLevel]=qLevel.mid(iLastDot,iNextDot-iLastDot).toInt(&bOK);
      if (bOK)
        iTitleLevel++;  
      else
        break;                          
    }  
    else
    { // No more dot
      iTitleNum[iTitleLevel]=qLevel.mid(iLastDot,-1).toInt(&bOK);
      if (bOK)
        iTitleLevel++;  
      break;
    }
    iLastDot=iNextDot+1;
  }      
        
  // Check the title level
  bOK=true;
  if (iTitleLevel<=m_iCurrentValidTitleLevel+1)
  { // Check that all the numbers are identical except the last one, that must be incremented by one
    for (iLevel=0;iLevel<iTitleLevel-1;iLevel++)
    { 
      if (iTitleNum[iLevel] != m_iCurrentValidTitleNum[iLevel])
      {
        bOK=false;
        break;
      }
    }            
    if (iTitleNum[iLevel] != m_iCurrentValidTitleNum[iLevel]+1)
      bOK=false;
  }
  else
    bOK=false;
  
  // Update the structure
  if (bOK)             
  {            
    // Get the parent item
    if (iTitleLevel>=2)
      pParentItem=m_pCurrentTitleItem[iTitleLevel-2];
    else
      pParentItem=rootItem;
    
    // Add a new item  
    pChildItem=new CTitleItem(qLevel+" "+qTitle, "Title_"+qLevel, pParentItem );
    pParentItem->appendChild(pChildItem);
    
    // Update the struture
    m_pCurrentTitleItem[iTitleLevel-1] = pChildItem;
    m_pCurrentTitleItem[iTitleLevel]=NULL;
    for (iLevel=0;iLevel<iTitleLevel;iLevel++)
      m_iCurrentValidTitleNum[iLevel]=iTitleNum[iLevel];
    m_iCurrentValidTitleNum[iTitleLevel]=0;
    m_iCurrentValidTitleLevel=iTitleLevel;
  }    
  return bOK;    
}
