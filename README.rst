qRFCView-OSX: a RFC viewer for OSX
==================================

qRFCView is a viewer for IETF RFC documents. Features:

- Automatic table of content, with direct opening of section
- Handling of RFC internal cross-references
- Automatic downloading of a referenced RFC from the IETF web site
  with a simple click
- Caching of RFC documents in a local directory
- Searching

This project is a fork of the original qRFCView (version 0.62) with some changes,
mainly cosmetic, so that it looks nice on OSX. Original project is located here:
http://qrfcview.berlios.de/


Original look on OSX
--------------------
.. image:: https://github.com/saghul/qrfcview-osx/raw/master/screenshots/original.png


New look on OSX
---------------
.. image:: https://github.com/saghul/qrfcview-osx/raw/master/screenshots/osx.png


Other changes
-------------
- Added application icon (for bundle)
- Added script for building a DMG easily
- Removed limit on RFC number
- Removed 'copy' button
- Removed unneeded files for OSX


Downloads
---------
Check here: https://github.com/saghul/qrfcview-osx/downloads


Building
--------
Install the Qt SDK (tested with version 4.8.3) from here: http://qt-project.org/downloads

Execute the build script:

::

    ./build.sh

The resulting app bundle and DMG will be in bin/

