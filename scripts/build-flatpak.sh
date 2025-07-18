#!/bin/sh

flatpak-builder --repo=myrepo --force-clean build-dir ./resources/io.noctvm.noctcraft.yml
flatpak build-bundle myrepo NoctCraft.flatpak io.noctvm.noctcraft --runtime-repo=https://flathub.org/repo/flathub.flatpakrepo
