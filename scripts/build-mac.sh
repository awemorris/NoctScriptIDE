#!/bin/sh

rm -rf build
cmake -B build -G Ninja -S . -DCMAKE_PREFIX_PATH=$HOME/Qt/6.9.1/macos
cmake --build build --config Release --parallel
codesign --force --deep --sign "Developer ID Application" "build/NoctScript.app"
