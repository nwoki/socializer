#!/bin/sh
export MAIN_DIR=$(pwd)

# Cleanup previous builds
rm -rf deps
rm -rf build

# Dependencies
mkdir deps
cd deps
export DEPS_DIR=$(pwd)

# Common CMake Modules
git clone git@git.ispirata.com:ispirata/common-cmake-modules.git
cd common-cmake-modules
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$MAIN_DIR/deps/install || return 1
make || return 1
make install || return 1
cd $DEPS_DIR

cd $MAIN_DIR

# Build qt5
mkdir buildQt5
cd buildQt5
cmake .. -DQt5_DIR=/opt/Qt5.1.0/5.1.0/gcc_64/lib/cmake/Qt5 -DCommonModules_DIR=$MAIN_DIR/deps/install/lib/cmake/CommonModules || return 1
make -j3 || return 1

# Build qt4
mkdir buildQt4
cd buildQt4
cmake .. -DCommonModules_DIR=$MAIN_DIR/deps/install/lib/cmake/CommonModules || return 1
make -j3 || return 1
