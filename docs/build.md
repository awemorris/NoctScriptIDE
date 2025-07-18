Build
=====

## Cloning Source

```
git clone https://github.com/awemorris/NoctCraft.git
cd NoctCraft
git submodule update --init --recursive
```

## Install Qt6

```
sudo apt-get install qt6-base-dev libxkbcommon-dev
```

## Install NoctVM Dependencies

```
sudo apt-get install libx11-dev libxpm-dev libasound2-dev mesa-common-dev
```

## Build

```
cmake -B build -S . -G Ninja
cmake --build build --config Release
```

## Platform-Specific Buil Scripts

### macOS

Install Qt 6.9.1 by the `Qt Installer` at `$HOME/Qt`.

Type the following in the terminal:
```
./script/build-mac.sh
```

### Windows

Install Qt 6.9.1 by the `Qt Installer` at `C:\\Qt`.

Type the following in the command prompt:
```
script/build-win.bat
```
