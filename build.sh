#!/bin/bash

if [[ -f Makefile ]]; then
    make clean
    make distclean
fi

rm -f Makefile srv/Makefile
rm -rf bin/*
qmake
make
pushd bin
macdeployqt qRFCView.app -dmg
popd

echo "Done!"

