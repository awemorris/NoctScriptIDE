#!/bin/sh

set -eu

# Check that git-diff is empty.
if git diff --quiet; then
    echo "Repository is clean.";
    git push github main;
else
    echo "Repository is dirty.";
    exit 1;
fi    

rm -rf dist build build-dir
mkdir dist

#
# Windows
#

rm -rf build
wine scripts/build-win.bat
cd build
mv app NoctScript
cd NoctScript
zip -r ../../dist/NoctScript-Windows-x64.zip *
cd ..
cd ..

#
# macOS
#

rm -rf build
./scripts/build-mac.sh
cd build
codesign --timestamp --options runtime --deep --force --sign "Developer ID Application" NoctScript.app
ditto -c -k --sequesterRsrc --keepParent NoctScript.app NoctScript-macOS-universal.zip
source ../.credential
xcrun notarytool submit NoctScript-macOS-universal.zip --apple-id "$APPLE_ID" --team-id "$TEAM_ID" --password "$APP_PASSWORD" --wait
xcrun stapler staple NoctScript.app
cp NoctScript-macOS-universal.zip ../dist/
cd ..

#
# Linux
#

# docker build -t noctscript-build ./scripts

# docker run --rm -it --privileged -v "$PWD":/src -w /src noctscript-build bash -c '
#     flatpak-builder --arch=aarch64 --repo=myrepo --force-clean build-dir ./resources/com.noctscript.NoctScript.yml
#     flatpak build-bundle myrepo NoctScript.flatpak com.noctscript.NoctScript --runtime-repo=https://flathub.org/repo/flathub.flatpakrepo
# '
# mv NoctScript.flatpak dist/NoctScript-Linux-aarch64.flatpak
# rm -rf build-dir

# docker run --rm -it --privileged -v "$PWD":/src -w /src noctscript-build bash -c '
#     flatpak-builder --arch=x86_64 --repo=myrepo --force-clean build-dir ./resources/com.noctscript.NoctScript.yml
#     flatpak build-bundle myrepo NoctScript.flatpak com.noctscript.NoctScript --runtime-repo=https://flathub.org/repo/flathub.flatpakrepo
# '
# mv NoctScript.flatpak dist/NoctScript-Linux-x86_64.flatpak
# rm -rf build-dir

#
# NoctVM
#

# cd bin
# zip -r ../dist/Noct2D-stable.zip \
#     hash \
#     noct2d-x86.exe \
#     noct2d-arm64.exe \
#     NoctVM.dmg \
#     noctvm-linux-aarch64 \
#     noctvm-linux-x86_64 \
#     macos-src.tar.gz \
#     ios-src.tar.gz \
#     android-src.tar.gz \
#     wasm.tar.gz \
#     noctcli-x86.exe \
#     noctcli-arm64.exe \
#     noctcli-macos \
#     noctcli-linux-aarch64 \
#     noctcli-linux-x86_64
# cd ..

#
# GitHub Release
#

VERSION="$(date +%Y%m%d)-dev+$(git rev-parse --short HEAD)"
gh release create $VERSION \
    dist/NoctScript-Windows-x64.zip \
    dist/NoctScript-macOS-universal.zip \
    --title "NoctScript $VERSION" \
    --notes "This is a development release of NoctScript!"
