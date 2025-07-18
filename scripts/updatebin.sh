#!/bin/sh

set -eu

# Check that git-diff is empty.
if git diff --quiet; then
    echo "Repository is clean.";
else
    echo "Repository is dirty.";
    exit 1;
fi    

# Update NoctVM submodule.
cd NoctVM
git clean -fd
cd ..
git submodule update --init --remote
git add NoctVM
git commit -m "Updated NoctVM submodule" || true

# Reconstruct the build directory.
cd NoctVM

#
# Windows x86
#

rm -rf build
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-i686-toolchain.cmake
cmake --build .
cp noctvm.exe ../../bin/noctvm.exe
cd ..
rm -rf build

#
# Windows arm64
#

rm -rf build
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-arm64-toolchain.cmake
cmake --build .
cp noctvm.exe ../../bin/noctvm-arm64.exe
cd ..
rm -rf build

#
# macOS
#

rm -rf build
mkdir build
cd build

# Build.
cmake .. -G Ninja
cmake --build .

# Sign & Notarize.
codesign --timestamp --options runtime --entitlements ../resources/macos.entitlements --deep --force --sign "Developer ID Application" NoctVM.app
ditto -c -k --sequesterRsrc --keepParent NoctVM.app NoctVM.zip
source ../../.credential
xcrun notarytool submit NoctVM.zip --apple-id "$APPLE_ID" --team-id "$TEAM_ID" --password "$APP_PASSWORD" --wait
xcrun stapler staple NoctVM.app

# Pack.
mkdir tmp
cp -Rv NoctVM.app tmp/NoctVM.app
hdiutil create -fs HFS+ -format UDBZ -srcfolder tmp -volname NoctVM NoctVM.dmg
codesign --sign 'Developer ID Application' NoctVM.dmg

cd ..
cp build/NoctVM.dmg ../bin/NoctVM.dmg
rm -rf build

#
# Linux x86_64
#

docker build --platform=linux/amd64 -t noctscript-x86_64 ./scripts
mkdir build
docker run --rm -it -v "$PWD":/src -w /src/build ubuntu22-x86_64 cmake .. -G Ninja
docker run --rm -it -v "$PWD":/src -w /src/build ubuntu22-x86_64 cmake --build .
cp build/noctvm ../bin/noctvm-linux-x86_64
rm -rf build

#
# Linux arm64
#

docker build --platform=linux/amd64 -t noctscript-arm64 ./scripts
mkdir build
docker run --rm -it -v "$PWD":/src -w /src/build ubuntu22-arm64 cmake .. -G Ninja
docker run --rm -it -v "$PWD":/src -w /src/build ubuntu22-arm64 cmake --build .
cp build/noctvm ../bin/noctvm-linux-arm64
rm -rf build

#
# iOS
#

cd projects/ios-src
make src
tar czf ios-src.tar.gz ios-src
cd ../..
cp projects/ios-src/ios-src.tar.gz ../bin/

#
# Android
#

cd projects/android-src
make src
tar czf android-src.tar.gz android-src
cd ../..
cp projects/android-src/android-src.tar.gz ../bin/

#
# Wasm
#

cd projects/wasm
make
tar czf wasm.tar.gz index.html index.js index.wasm
cd ../..
cp projects/wasm/wasm.tar.gz ../bin/

#
# Unity
#

cd projects/unity-src
make src
tar czf unity-src.tar.gz unity-src
cd ../..
cp projects/unity-src/unity-src.tar.gz ../bin/

#
# Commit to git repo
#

git clean -fd
GIT_HASH_NOCTVM="$(git rev-parse HEAD)"
cd ..

echo "$GIT_HASH_NOCTVM" > bin/hash

git add bin/hash
git add bin/noctvm.exe
git add bin/noctvm-arm64.exe
git add bin/NoctVM.dmg
git add bin/macos-src.tar.gz
git add bin/noctvm-linux-x86_64
git add bin/noctvm-linux-aarch64
git add bin/ios-src.tar.gz
git add bin/android-src.tar.gz
git add bin/wasm.tar.gz
git add bin/unity-src.tar.gz

git commit -m "Updated NoctVM binaries"

echo 'Done.'
