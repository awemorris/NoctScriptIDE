@echo off

PATH=%PATH%;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja;C:\Qt\6.9.1\mingw_64\bin

del /S /Q build
mkdir build
cd build
cmake -G Ninja -DCMAKE_MAKE_PROGRAM=C:\Qt\Tools\Ninja\ninja.exe -DCMAKE_CXX_COMPILER=C:\Qt\Tools\mingw1310_64\\bin\g++.exe -DCMAKE_PREFIX_PATH=C:\Qt\6.9.1\mingw_64\lib\cmake ..
ninja
cd ..
