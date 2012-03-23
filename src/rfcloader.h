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

#ifndef RFCLOADER_H
#define RFCLOADER_H

#include <qobject.h>
#include <QStringList>
#include <QMap>

/**
@author Romain Rollet
*/
class QHttp;
class QUrl;
class QHttpResponseHeader;
class QFile;
class QRFCLoader : public QObject
{
Q_OBJECT
private:
    struct RFCDesc_t
    {
      uint32_t iRFCNum;
      QFile *pFile;
    };
    
public:
    QRFCLoader(QObject *parent = 0);
    ~QRFCLoader();

    void SetDirectories(QStringList &qDirList, uint8_t iDefaultDir);
    void GetFile(uint32_t iRFCNum);
    QHttp *GetQHttp() {return m_qHttp;}
    QString GetDir() {return m_qDir;}
    void SetDownloadURL(QUrl &qURL);
    
signals:
    void start(const QString &sFilename); 
    void done(const QString &sFilename); 
       
private slots:
    void startDownload(int iRequestID);
    void fileDownload(int iRequestID, bool bError);    
    void receivedHeader(const QHttpResponseHeader &qResponseHdr);
    
private:
    QString m_qDir;
    QString m_qIETFSite, m_qIETFPath;
    int m_iCurrentRequestID;
    
    QMap<int, RFCDesc_t> m_RequestList;
    QHttp *m_qHttp;
};

#endif
