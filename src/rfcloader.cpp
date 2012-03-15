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

#include <QHttp>
#include <QUrl>
#include <QFile>
#include "rfcloader.h"
#include <QMessageBox>
#include <QtDebug>
#include <QDir>

QRFCLoader::QRFCLoader(QObject *parent)
 : QObject(parent)
{
  m_qHttp=new QHttp(this);
  connect(m_qHttp, SIGNAL( requestStarted(int) ), this, SLOT( startDownload(int) ) );
  connect(m_qHttp, SIGNAL( requestFinished(int, bool) ), this, SLOT( fileDownload(int, bool) ) );
  connect(m_qHttp, SIGNAL( responseHeaderReceived(QHttpResponseHeader) ), this, SLOT( receivedHeader(QHttpResponseHeader) ) );
  //m_qDirList.append(".");
  m_iDefaultDir=0;
  m_qIETFSite="www.ietf.org";
  m_iCurrentRequestID=-1;
}


QRFCLoader::~QRFCLoader()
{
  delete m_qHttp;
}

void QRFCLoader::SetDirectories(QStringList &qDirList, uint8_t iDefaultDir) 
{
  int i;
  QDir qDirectory;
  
  m_qDirList.clear();
  for (i=0;i<qDirList.count();i++)
  {
    if (qDirList[i].isEmpty() )
      continue;
    qDirectory=QDir(qDirList[i]);
    if (qDirectory.exists())
      m_qDirList.append(qDirList[i]);
    if (i==iDefaultDir)
      m_iDefaultDir=m_qDirList.count() - 1;
    qDebug() << qDirList[i];  
  }
  
  if ( m_qDirList.count()==0 )
  {
    m_qDirList=QStringList(QString("."));
    iDefaultDir=0;
  }

  //m_iDefaultDir=iDefaultDir;
}   

void QRFCLoader::SetDownloadURL(QUrl &qURL)
{  
  m_qIETFSite=qURL.host();
  m_qIETFPath=qURL.path(); 
}

void QRFCLoader::GetFile(uint32_t iRFCNum)
{
  uint8_t i;
  QString qFilename;
  int iRequestID;
  
  for (i=0;i<m_qDirList.count();i++)
  {
    qFilename=m_qDirList.at(i)+ "/rfc" + QString::number(iRFCNum) +".txt";
    if ( QFile::exists( qFilename ) )
    {
      emit done(qFilename);
      return;
    }    
  }
  
  // RFC is not yet loaded
  // Open a file in the default dir.      
  RFCDesc_t sRFCDesc;
  qFilename=m_qDirList.at(m_iDefaultDir) + "/rfc" + QString::number(iRFCNum) +".txt";
  qDebug() << qFilename;
  sRFCDesc.pFile = new QFile(qFilename);
  sRFCDesc.iRFCNum=iRFCNum;
  
  if (!sRFCDesc.pFile->open(QIODevice::WriteOnly)) {
     delete sRFCDesc.pFile;  
     QMessageBox::information(NULL, tr("RFCView"),
                                     tr("Unable to write RFC %1")
                                     .arg(iRFCNum));
     
     return;
  }    
  QString qUrl=m_qIETFPath+QString("/rfc%1.txt").arg(iRFCNum, 4, 10, QChar('0'));  
  
  
  qDebug() << qUrl;
  m_qHttp->setHost(m_qIETFSite);
  iRequestID=m_qHttp->get( qUrl, sRFCDesc.pFile);
  m_RequestList.insert(iRequestID, sRFCDesc);
  m_iCurrentRequestID=-1;
}

void QRFCLoader::startDownload(int iRequestID)
{
  //qDebug() << "startDownload="+QString::number(iRequestID);
  if ( m_RequestList.contains(iRequestID))
    m_iCurrentRequestID=iRequestID;    
  else
    m_iCurrentRequestID=-1;
}

void QRFCLoader::receivedHeader(const QHttpResponseHeader &qResponseHdr) 
{
  RFCDesc_t sRFCDesc;
  QString qFilename;
  //qDebug() << "HTTP status=" + QString::number(qResponseHdr.statusCode())+","+QString::number(m_iCurrentRequestID);
  if ( m_iCurrentRequestID!=-1 && qResponseHdr.statusCode() != 200 )
  {
    sRFCDesc=m_RequestList.value(m_iCurrentRequestID);
    m_iCurrentRequestID=-1;         
    QMessageBox::information(NULL, tr("RFCView"),
                                    tr("Unable to load RFC %1: ")
                                    .arg(sRFCDesc.iRFCNum)+  qResponseHdr.reasonPhrase() ) ;
    m_RequestList.remove(m_iCurrentRequestID);     
                      
  }                                       
  else
  {
    sRFCDesc=m_RequestList.value(m_iCurrentRequestID);
    qFilename=m_qDirList.at(m_iDefaultDir) + "/rfc" + QString::number(sRFCDesc.iRFCNum) +".txt";
    emit start( qFilename );  
  }
}

void QRFCLoader::fileDownload(int iRequestID, bool bError)
{
  QString qFilename;
  RFCDesc_t sRFCDesc;
  //qDebug() << "fileDownload="+QString::number(iRequestID)+","+QString::number(m_iCurrentRequestID);
  if ( m_RequestList.contains(iRequestID) )
  {
    sRFCDesc=m_RequestList.value(iRequestID);
    if (m_iCurrentRequestID==iRequestID)
    {      
      qFilename=m_qDirList.at(m_iDefaultDir) + "/rfc" + QString::number(sRFCDesc.iRFCNum) +".txt";
      sRFCDesc.pFile->close();
      delete sRFCDesc.pFile;
      
      if (!bError)  
      {
        qDebug() << "File downloaded: " << qFilename;
        emit done( qFilename );
      }
      else
        QMessageBox::information(NULL, tr("RFCView"),
                                      tr("Unable to load RFC %1: ")
                                      .arg(sRFCDesc.iRFCNum)+  m_qHttp->errorString() ) ;
  
      m_RequestList.remove(iRequestID);
    }
    else
    {
       sRFCDesc.pFile->remove();
    }
  }
}
