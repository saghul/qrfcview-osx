#!/bin/bash

if [[ -f Makefile ]]; then
    make clean
    make distclean
fi

rm -rf bin/*
qmake -spec macx-g++
make
pushd bin
macdeployqt qRFCView.app -dmg
popd

echo "Done!"

