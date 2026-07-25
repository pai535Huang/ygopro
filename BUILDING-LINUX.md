# YGOPro Linux Build Layout

This repository follows the upstream Premake build flow described in:

https://github.com/Fluorohydride/ygopro/wiki/Building-on-Ubuntu

The repository should track source code, Premake scripts, and submodule
references. Local dependency source trees, generated build files, generated
binaries, and runtime card assets are intentionally ignored by Git.

## Tracked Project Structure

The important tracked parts are:

```text
.
|-- gframe/        # GUI client source
|-- ocgcore/       # submodule: duel engine
|-- script/        # submodule: card scripts
|-- premake/       # Premake dependency project descriptions
|-- textures/      # GUI textures
|-- sound/         # sound metadata
|-- premake5.lua   # root Premake configuration
|-- .gitmodules    # submodule locations
`-- README.md
```

Use submodules when cloning:

```bash
git clone --recursive git@github.com:pai535Huang/ygopro.git
cd ygopro
git submodule update --init
```

## Local Build-Only Directories

These directories are required for a local build, but should not be committed:

```text
build/       # generated Makefiles and object files
bin/         # generated executables
irrlicht/    # modified Irrlicht source tree
lua/         # Lua 5.3 or 5.4 source tree
miniaudio/   # miniaudio split source files
event/
freetype/
jpeg/
lzma/
png/
sqlite3/
zlib/
```

The dependency project folders are copied from `premake/`:

```bash
cp -r premake/* .
```

## System Dependencies

Install equivalent packages for your Linux distribution. The upstream Ubuntu
guide uses:

```bash
sudo apt install libevent-dev libfreetype6-dev libgl1-mesa-dev libsqlite3-dev libopusfile-dev libvorbis-dev libjpeg-dev libpng-dev zlib1g-dev liblzma-dev
```

On Arch-based systems, the equivalent package-manager step is typically:

```bash
sudo pacman -S --needed base-devel git freetype2 mesa glu sqlite libevent opusfile libvorbis libjpeg-turbo libpng zlib xz
```

The local build used source builds for the dependencies that upstream marks as
manual/static requirements:

```text
irrlicht  -> https://github.com/mercury233/irrlicht
lua       -> https://www.lua.org/download.html
miniaudio -> version 0.11.25, extras/miniaudio_split
```

## Generate and Build

From the repository root:

```bash
chmod +x premake5
./premake5 gmake --no-build-lzma
make -C build -j 4 config=release
```

The executable is generated at:

```text
bin/release/YGOPro
```

Runtime assets such as `cards.cdb`, `pics/`, and `deck/` come from a YGOPro
distribution or a generated card database. They are runtime data, not source
files for this repository.
