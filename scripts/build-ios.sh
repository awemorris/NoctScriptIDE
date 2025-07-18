#!/bin/sh

rm -rf build
mkdir build
cd build
cmake -G Xcode \
  -DCMAKE_SYSTEM_NAME=iOS \
  -DCMAKE_OSX_SYSROOT=iphoneos \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=14.0 \
  -DCMAKE_PREFIX_PATH=$HOME/Qt/6.9.1/ios/lib/cmake \
  -DCMAKE_TOOLCHAIN_FILE=$HOME/Qt/6.9.1/ios/lib/cmake/Qt6/qt.toolchain.cmake \
  ..
cd ..
