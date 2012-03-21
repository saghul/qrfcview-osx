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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringList>
#include <QMainWindow>
#include <QDir>

class QAction;
class QMenu;
class QWorkspace;
class MdiChild;
class QSignalMapper;
class QRFCLoader;
class QProgressBar;
class CDialogFind;
class QTabWidget;
class QFileDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void SetCurrentDir(QDir qCurrentDir) {m_qLastOpenDir=qCurrentDir;}
    
protected:
    void closeEvent(QCloseEvent *event);   
    
public slots:
    void RFCLoad(uint32_t);
    void RFCReady(const QString &qFileName);
    
private slots:
    void getrfc();
    void findOpen();
    void findnext();
    void findprev();
    void about();
    void setFont();
    void setDirectories();
    void updateMenus();
    void updateWindowMenu();
    void open_dialog_finished(int result);
    void close_tab(int index);
    MdiChild *createMdiChild(const QString &qTitle);
    
    void RFCStart(const QString &qFilename);
    void updateRFCProgress(int bytesRead, int totalBytes);
    void forward();
    void backward();
    void print();
    
    
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    MdiChild *activeMdiChild();
    MdiChild *findMdiChild(const QString &fileName);

    QTabWidget *m_qTabWidget;
    QFileDialog *open_dialog;
    QSignalMapper *windowMapper;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *openAct, *printAct, *closeAct, *loadAct;
    QAction *exitAct;
    QAction *copyAct;    
    QAction *setFontAct, *setDirectoriesAct;
    /*
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;*/
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *backwardAct, *forwardAct;
    QAction *findAct, *findnextAct, *findprevAct;
    QProgressBar *m_pProgressBar; // Progress bar located in the status bar
    QFont m_qFont; // Font used for text display
    QStringList m_qDirectoryList;
    uint8_t m_iDefaultDirectory;
    QString m_qRFCURL;
    QRFCLoader *m_pRFCLoader;
    CDialogFind *m_pDialogFind;
    QDir m_qLastOpenDir;
};

#endif
