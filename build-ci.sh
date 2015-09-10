#!/bin/sh
export MAIN_DIR=$(pwd)

# Cleanup previous builds
rm -rf build*

# Build qt5
mkdir buildQt5
cd buildQt5
cmake .. -DQT5_BUILD=TRUE -DQt5_DIR=/opt/Qt5.1.0/5.1.0/gcc_64/lib/cmake/Qt5 || return 1
make -j3 || return 1

# back to main dir
cd $MAIN_DIR

# Build qt4
mkdir buildQt4
cd buildQt4
cmake ..  || return 1
make -j3 || return 1
