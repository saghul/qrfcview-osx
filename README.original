==== qRFCView ====

== What is qRFCView? ==

 qRFCView is a viewer for IETF RFC.
 
 Advantages:
  - automatic table of content, with direct opening of section;
  - handling of RFC internal cross-references;
  - automatic downloading of a referenced RFC from the IETF web site
    on a simple click;
  - caching of RFC in a local directory;
  - tab-browsing of RFC;
  - searching.
  
 It is available for Linux and Windows operating system (and
 potentially MacOS X).

== How to install it? ==

 qRFCView requires the Trolltech Qt4 library that is available under MAC OS, 
 Windows and Linux platforms. Trolltech's dual license policy provides versions 
 of Qt under the terms of the GNU General Public License (GPL). It can be
 downloaded from http://www.trolltech.com/products/qt/downloads.
 
 * Win32 building (WinNT/Win2k/WinXP)
 Makefile generation: qmake -win32 rfcview.pro
 qRFCView binary build: make
 Optionally, you can build a Windows installer
 
 * Unix building
 qmake -unix rfcview.pro
 qRFCView binary build: make
  
 * Win32 binary
 The qRFCView binary can be directly installed by using the provided Win32 
 installer file. The following DLLs will be added to the windows system
 directory:
	- mingwm10.dll
    - QtCore4.dll
    - QtDesignerComponents4.dll
    - QtGui4.dll
    - QtNetwork4.dll

== How to use it? ==

Just start it. Use "File>Load RFC..." to load a RFC by its number.

Double-click on a table of content item to open the corresponding
section. Internal links and referenced RFC are active hyperlinks,
underlined in blue.

One can set the default directories where RFC are saved using
"Edit>Set Directories...".

== License ==

qRFCView is free software, licensed under GNU GPL (see LICENSE file).

== Contact ==
qRFCView has been developped at Mitsubishi Electric, ITE-TCL, France.
qRFCView developer is Romain ROLLET <rfcview@gmail.com>.


== Known bugs ==

#1 Under Windows, qRFCView sometimes doesn't close when exiting Windows while
   qRFCView is still open.
