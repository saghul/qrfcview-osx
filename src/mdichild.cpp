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


#include <QtGui>
#include <QPainter>
#include <QFontMetrics>

#include "mdichild.h"
#include "TitleModel.h"
#include "TitleItem.h"
#include "qrfceditor.h"
#include "cdialogfind.h"

MdiChild::MdiChild(QWidget *pParent):QSplitter(pParent)
{
    m_pTreeView= new QTreeView;
    m_pTextEdit= new QRFCEditor;
    m_pTitleModel=NULL;
    m_iNbPages=0;
    addWidget(m_pTreeView);
    addWidget(m_pTextEdit);
    setStretchFactor(0, 0);
    setStretchFactor(1, 1);

    setAttribute(Qt::WA_DeleteOnClose);
}

MdiChild::~MdiChild()
{
  if (m_pTitleModel)
    delete m_pTitleModel;
  delete m_pTreeView;
  delete m_pTextEdit;
}

bool MdiChild::loadFile(const QString &fileName)
{
    QFile file(fileName);
    QString qText;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MDI"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
        return false;
    }
    m_pTitleModel = new CTitleModel("RFC Bidule");
    ParseRFCText(file, qText);
    file.close();

    m_pTreeView->setModel(m_pTitleModel);
    m_pTreeView->show();

    m_pTextEdit->setHtml(qText);
    m_pTextEdit->setReadOnly(true);
    //QApplication::restoreOverrideCursor();

    connect(m_pTreeView, SIGNAL(activated(QModelIndex)), this, SLOT(goToTitle(QModelIndex)));
    disconnect(m_pTextEdit, SIGNAL(anchorClicked(QUrl)), 0 , 0 );
    connect(m_pTextEdit, SIGNAL(anchorClicked(QUrl) ), this, SLOT(goToAnchor(QUrl)));
    setCurrentFile(fileName);

    return true;
}


QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    event->accept();
    /*
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }*/
}


void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    m_pTextEdit->document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
    m_qFileName=fileName;
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

bool MdiChild::hasSelection()
{
  return m_pTextEdit->textCursor().hasSelection();
}

void MdiChild::goToTitle(const QModelIndex &qModelIndex)
{
  CTitleItem *pChildItem = static_cast<CTitleItem*>(qModelIndex.internalPointer());

  m_pTextEdit->scrollToAnchor2(pChildItem->GetAnchor());
}

void MdiChild::goToAnchor(const QUrl &qURL)
{
  qDebug() << qURL.toString();
}

void MdiChild::setCurrentFont(const QFont &qFont)
{
  QTextCursor cursor(m_pTextEdit->textCursor());

  m_pTextEdit->selectAll();
  m_pTextEdit->setCurrentFont(qFont);
  cursor.clearSelection();
  m_pTextEdit->setTextCursor(cursor);
  m_pTextEdit->document()->setDefaultFont(qFont);
}

bool MdiChild::ParseRFCText(const QFile &qFile, QString &qOutput)
{
  QTextStream qInStream((QFile*)&qFile);
  QString qLine;
  //QRegExp qRegExpTitle("\\s*([\\d]+[\\d\\.]*)[\\s]+(\\w[-/+:;\\w\\s]*)");
  QRegExp qRegExpTitle("^([\\d]+[\\d\\.]*)[\\s]+(.*)$");
  QRegExp qRegExpBiblio("^\\s*\\[([\\d\\w]+)\\]\\s+");
  QRegExp qRegExpReference("\\[([\\d\\w]+)\\]");
  QRegExp qRegExpRFC("RFC\\s+([\\d]+)");
  QRegExp qRegExpFooter("\\[Page\\s\\d+\\]");
  QRegExp qRegExpSection("[Ss]ection\\s(\\d+[\\d\\.]*\\d+)");

  uint8_t iState=PARSER_NORMALTEXT, iNextState=PARSER_NORMALTEXT;
  int iNextPos, iPrevPos=0;
  QString qLineOutput;

  m_iNbPages=1;
  qOutput="<HTML><BODY><PRE>";
  while( !qInStream.atEnd() )
  {
    qLine=qInStream.readLine();

    // Replace character
    qLine.replace(QChar('&'),QString("&amp;") );
    qLine.replace(QChar('<'),QString("&lt;") );
    qLine.replace(QChar('>'),QString("&gt;") );
    qLine.replace(QChar('\"'),QString("&quot;") );

    iNextState=iState;
    if (iState==PARSER_NORMALTEXT)
    {
      if ( (qRegExpTitle.indexIn(qLine, 0)) != -1)
      { // Is it a new title?
        // Check title num is correct
        if (m_pTitleModel->ParseTitle(qRegExpTitle.cap(1), qRegExpTitle.cap(2) ))
        { // Insert the HTML anchor
          qLine="</PRE><H3><a name=\"Title_" + qRegExpTitle.cap(1) + "\"></a>"+ qLine +"</H3><PRE>";
          //bLineNotAdded=false;
        }
      }
      else if ( (qRegExpBiblio.indexIn(qLine, 0)) != -1)
      { // Is it a bib ref (shall be at the end of the document
        qLine="<font color=\"Red\"><a name=\"BibRef_" + qRegExpBiblio.cap(1) + "\"></a>"+ qLine;
        iNextState=PARSER_BIBTEXT;
        //bLineNotAdded=false;
      }
      else if ( (qRegExpFooter.indexIn(qLine, 0)) != -1)
      { // Test if it is the footer line
        qLine="<font color=\"Grey\">"+qLine+"</font>";
      }
      else if (qLine.indexOf(QChar('\f'))!=-1)
      { // Test if it is the header line
        iNextState=PARSER_HEADER;
        qLine="<HR><font color=\"Grey\">";
        m_iNbPages++;
      }

    }

    else if (iState==PARSER_BIBTEXT)
    {
      if (qLine.trimmed().isEmpty())
      {
        qLine="</font>";
        iNextState=PARSER_NORMALTEXT;
      }
    }

    else if (iState==PARSER_HEADER)
    {
      if (!qLine.isEmpty())
      {
        qLine=qLine+"</font>";
        iNextState=PARSER_NORMALTEXT;
      }
    }

    iPrevPos=0;
    if ( iNextState!=PARSER_BIBTEXT && (iNextPos=qRegExpReference.indexIn(qLine, iPrevPos)) != -1 )
    { // Format biblio references: "[%d]"
      qLineOutput.clear();
      do
      {
        // Insert a link to the reference
        //qDebug()<<qRegExpReference.cap(1);
        qLineOutput+=qLine.mid(iPrevPos,iNextPos-iPrevPos)+"<a href=\"#BibRef_"+ qRegExpReference.cap(1) + "\">"+qRegExpReference.cap(0)+"</a>";
        iPrevPos=iNextPos+qRegExpReference.matchedLength();
      }while (  (iNextPos=qRegExpReference.indexIn(qLine, iPrevPos)) != -1 );

      qLineOutput+=qLine.mid(iPrevPos,-1);
      qLine=qLineOutput;
    }

    iPrevPos=0;
    if ( iState!=PARSER_HEADER && (iNextPos=qRegExpRFC.indexIn(qLine, iPrevPos)) != -1 )
    { // Format RFC references: "RFC %d"
      qLineOutput.clear();
      do
      {
        // Insert a link to the reference
        //qDebug()<<qRegExpReference.cap(1);
        qLineOutput+=qLine.mid(iPrevPos,iNextPos-iPrevPos)+"<a href=\"rfc"+ qRegExpRFC.cap(1) + ".txt\">"+qRegExpRFC.cap(0)+"</a>";
        iPrevPos=iNextPos+qRegExpRFC.matchedLength();
      }while (  (iNextPos=qRegExpRFC.indexIn(qLine, iPrevPos)) != -1 );
      qLineOutput+=qLine.mid(iPrevPos,-1);
      qLine=qLineOutput;
    }


    iPrevPos=0;
    if ( iState!=PARSER_HEADER && (iNextPos=qRegExpSection.indexIn(qLine, iPrevPos)) != -1 )
    {
      qLineOutput.clear();
      do
      {
        // Insert a link to the reference
        //qDebug()<<qRegExpReference.cap(1);
        qLineOutput+=qLine.mid(iPrevPos,iNextPos-iPrevPos)+"<a href=\"#Title_"+ qRegExpSection.cap(1) + "\">"+qRegExpSection.cap(0)+"</a>";
        iPrevPos=iNextPos+qRegExpSection.matchedLength();
      }while (  (iNextPos=qRegExpSection.indexIn(qLine, iPrevPos)) != -1 );
      qLineOutput+=qLine.mid(iPrevPos,-1);
      qLine=qLineOutput;
    }

    iState=iNextState;


    qOutput+=qLine+"\n";
  }

  qOutput+="</PRE></BODY></HTML>";
  return true;
}

bool MdiChild::FindText(QString &qTextToFind, uint32_t iOptionFlags)
{
  QRegExp qRegExp;
  QTextDocument::FindFlags qFindFlags=0;

  if (iOptionFlags&FIND_OPTIONSFLAG_REGEXP)
    qRegExp=QRegExp(qTextToFind);

  if (iOptionFlags&FIND_OPTIONSFLAG_BACKWARD)
    qFindFlags|= QTextDocument::FindBackward;
  if (iOptionFlags&FIND_OPTIONSFLAG_CASE)
    qFindFlags|= QTextDocument::FindCaseSensitively;
  if (iOptionFlags&FIND_OPTIONSFLAG_WHOLE)
    qFindFlags|= QTextDocument::FindWholeWords;

  if (!(iOptionFlags&FIND_OPTIONSFLAG_CURSOR))
  { // Set the cursor at the beginning
    QTextCursor cursor(m_pTextEdit->textCursor());
    cursor.movePosition(QTextCursor::Start);
    m_pTextEdit->setTextCursor(cursor);
  }

  return m_pTextEdit->find(qTextToFind, qFindFlags);
}


void MdiChild::Print(QPrinter *qPrinter, bool bAll, int fromPage, int toPage)
{
  QFile qFile(m_qFileName);
  if (!qFile.open(QFile::ReadOnly | QFile::Text))
  {
      QMessageBox::warning(this, tr("MDI"),
                          tr("Cannot read file %1:\n%2.")
                          .arg(m_qFileName)
                          .arg(qFile.errorString()));
      return;
  }
  QTextStream qInStream((QFile*)&qFile);
  QString qLine;
  int yPos        = 0;                    // y position for each line
  //QRegExp qRegExpTitle("\\s*([\\d]+[\\d\\.]*)[\\s]+(\\w[-/+:;\\w\\s]*)");
  uint8_t iState=PARSER_NORMALTEXT;

  QRegExp qRegExpTitle("^([\\d]+[\\d\\.]*)[\\s]+(.*)$");
  QRegExp qRegExpBiblio("^\\s*\\[([\\d\\w]+)\\]\\s+");
  QRegExp qRegExpReference("\\[([\\d\\w]+)\\]");
  QRegExp qRegExpRFC("RFC\\s([\\d]+)");
  QRegExp qRegExpFooter("\\[Page\\s\\d+\\]");

  QPainter p(qPrinter);
  const int dpiy = p.device()->logicalDpiY();
  const int iMargin = (int) ((2/2.54)*dpiy); // 2 cm margins
  //QFont qFont  =  m_pTextEdit->document()->defaultFont();
  QFont qFont  =  m_pTextEdit->currentFont();
  p.setFont( qFont );
  QFontMetrics fm = p.fontMetrics();

  qreal iRatio= ( (qreal)p.device()->height() - 2*iMargin )/((qreal)60*fm.lineSpacing());
  //p.scale(iRatio,iRatio);
  qDebug() << "Ratio=" + QString::number(qFont.pointSize()*iRatio);
  qFont.setPointSize((int)(qFont.pointSize()*iRatio));
  //qFont.setPointSize(10);
  QFont qTitleFont=qFont;
  qTitleFont.setWeight(QFont::Bold);
  QFont qHeaderFont=qFont;
  qHeaderFont.setItalic(true);
  p.setFont( qFont );

  fm = p.fontMetrics();
  int iCurrentPage=1;
  if (bAll)
  {
    fromPage=0;
    toPage=1000000;
  }

  while( !qInStream.atEnd() )
  {
    qLine=qInStream.readLine();

    if ( iMargin + yPos > p.device()->height() - iMargin )
    {
      if (iCurrentPage>=fromPage && iCurrentPage<toPage)
        qPrinter->newPage();
      iCurrentPage++;
      yPos = 0;
    }

    // Change font
    if (iState==PARSER_NORMALTEXT)
    {
      if (qLine.indexOf(QChar('\f'))!=-1)
      { // Test if it is the header line
        p.setFont( qHeaderFont );
        iState=PARSER_HEADER;
        yPos = 0;
        qLine="\n";
        if (iCurrentPage>=fromPage && iCurrentPage<toPage)
        {
          qPrinter->newPage();

        }
        iCurrentPage++;
      }
      else if ( (qRegExpTitle.indexIn(qLine, 0)) != -1)
      {
        p.setFont( qTitleFont );
      }
      else if ( (qRegExpFooter.indexIn(qLine, 0)) != -1)
      { // Test if it is the footer line
        p.setFont( qHeaderFont );
      }
    }
    else if (iState==PARSER_HEADER)
    {
      if (!qLine.isEmpty())
      {
        iState=PARSER_NORMALTEXT;
      }
    }

    if (iCurrentPage>=fromPage && iCurrentPage<=toPage)
    {
      p.drawText( iMargin, iMargin + yPos,
                    p.device()->width(), fm.lineSpacing(),
                    Qt::TextExpandTabs | Qt::TextDontClip,
                    qLine );
      yPos = yPos + fm.lineSpacing();
    }
    if (iState==PARSER_NORMALTEXT)
      p.setFont( qFont );
  }
  qFile.close();
}
