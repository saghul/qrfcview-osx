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



    #ifndef MDICHILD_H
    #define MDICHILD_H

    #include <QSplitter>

#define PARSER_NORMALTEXT 0
#define PARSER_BIBTEXT 1
#define PARSER_HEADER 2
#define PARSER_TRAILER 3

    class QTreeView;
    class QRFCEditor;
    class CTitleModel;
    class QModelIndex;
    class QFile;
    class QUrl;
    class QPrinter;
        
    class MdiChild : public QSplitter
    {
        Q_OBJECT

    public:
        MdiChild(QWidget *pParent=0);
        ~MdiChild();
        bool loadFile(const QString &fileName);
        QString userFriendlyCurrentFile();
        QString currentFile() { return curFile; }
        void copy();
        bool hasSelection();
        void setCurrentFont(const QFont &);
        bool FindText(QString &qTextToFind, uint32_t iOptionFlags);
        void Print(QPrinter *pPrinter, bool all, int fromPage, int toPage);
        uint32_t GetNbPages() {return m_iNbPages;}
                
    protected:
        void closeEvent(QCloseEvent *event);

    private:
        void setCurrentFile(const QString &fileName);
        QString strippedName(const QString &fullFileName);
        bool ParseRFCText(const QFile &qFile, QString &qOutput);

    public slots:
        void goToTitle(const QModelIndex &);
        void goToAnchor(const QUrl &qURL);
    
    public:
      QRFCEditor *m_pTextEdit;     
    private:
      QTreeView *m_pTreeView;              
      CTitleModel *m_pTitleModel; 
      QString m_qFileName;
      QString curFile;
      bool isUntitled;
      uint32_t m_iNbPages;
      
    };

    #endif
