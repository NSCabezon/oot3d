# The Legend of Zelda: Ocarina of Time 3D

This is a WIP decompilation of The Legend of Zelda: Ocarina of Time 3D.
The purpose of the project is to recreate a source code base for the game from scratch, using information found inside the game along with static and/or dynamic analysis.
The only build currently supported is the USA version.

The reference build targets the following binaries:


* **oot3d.axf**
* **code.bin** `md5: 20c7ec6288ba0af4feb6db646c6cc6d5`

Re-building of `romfs.bin` and `oot3d.3ds` will be targeted in the future.

**Note:** This repository does not include any of the assets necessary to build the ROM.
A prior copy of the game is required to extract the needed assets.

Website: <https://zelda.deco.mp/>

## Fork progress

Verified on **2026-10-19** against the USA reference build:

| Metric | Current progress |
| --- | --- |
| `.text` implemented in C/C++ | **17.388%** — 690,544 bytes |
| Functions implemented in C/C++ | **3,453 / 12,160 (28.40%)** |

The complete local rebuild has one known mismatch, `FUN_00249af8` (four words),
and zero differing bytes outside functions. Function counts use the current split
boundaries; percentages describe decompilation progress, not a complete ROM port.

This fork includes matching actor implementations and macOS RVCT build support.
The original project is [zeldaret/oot3d](https://github.com/zeldaret/oot3d).
ROMs, extracted data, generated assembly and build outputs are kept locally.
`tools/local_assets.json` contains only section names and span metadata;
`tools/regenerate_local_assets.py` recreates split assembly and small data tails
from the user's own extracted game during the build.

## Installation
These instructions and the tools in this repository all assume a Windows development environment. Cross-platform support may be added in the future.

To set up the repository, place a USA version of `The Legend of Zelda: Ocarina of Time 3D (USA)` in the `disassembly` directory of the repository and name it `baserom.3ds`. Any revision (v0 or v1) and either trimmed or untrimmed roms should be able to produce a correct repository.

Download and extract the contents of RVCT4.0 [Build 821] for Windows. Once extracted, the following environment variables must be set:

* `ARMCC_PATH`: The root directory of the extracted contents, such that `$ARMCC_PATH/win_32-pentium` is the folder containing the compiler binaries
* `RVCT40INC`: The include directory for ARM's Windows headers, most likely equal to `$ARMCC_PATH/include/windows`
* `RVCT40LIB`: The library director for ARM's libaries, most likely equal to `$ARMCC_PATH/lib`

You will also need to have installed cmake, make, and python for Windows.

To begin, run `cmake -B build -S . -G "Unix Makefiles"`. This should configure the build using the ARM compiler toolchain. If this succeeds, perform the first build using `cmake --build build -t check`. If all goes well, you should see:
```
build/code.bin: OK
```

Note: to speed up the build, you can pass `-jN`, where N is the number of threads to use in the build.

## Building

A variety of useful cmake targets are available. Run `cmake --build build -t [target]` with the following values of `[target]`:

* To build `oot3d.axf`, set `[target]` = `oot3d`
* To build `code.bin`, set `[target]` = `code`
* To check matching, set `[target]` = `check`


## macOS build notes

The local macOS build uses **RVCT 4.0 Build 839**, `wibo-macos` under Rosetta 2,
CMake and Python 3. Supply your own RVCT installation and game copy. The first
disassembly pass also needs Docker or a compatible engine running Linux/amd64
containers. The command shim is built from source with Homebrew `llvm` and `lld`:

```sh
git submodule update --init --recursive
sh tools/macos/cmdshim/build.sh
export ARMCC_PATH=/path/to/rvct
export RVCT40INC=/path/to/rvct/include
export RVCT40LIB=/path/to/rvct/lib
export WIBO=/path/to/wibo-macos
cmake -S . -B build
cmake --build build --target code --parallel 1
```

`ARMCC_PATH/win_32-pentium` must contain the Windows RVCT tools. Place the game
at `disassembly/baserom.3ds` as described above. Generated local assets are not
versioned. The upstream `check` target still checks for a fully identical binary;
the known mismatch described above means this fork does not yet pass that hash.
