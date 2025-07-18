FROM ubuntu:24.04

RUN apt-get update
RUN apt-get install -y --no-install-recommends build-essential cmake qt6-base-dev flatpak flatpak-builder ca-certificates elfutils
RUN flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
RUN flatpak install -y flathub org.kde.Sdk/x86_64/6.8 org.kde.Sdk/aarch64/6.8 org.kde.Platform/x86_64/6.8 org.kde.Platform/aarch64/6.8
