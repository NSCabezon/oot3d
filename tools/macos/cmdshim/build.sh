#!/bin/sh
# Rebuild cmd.exe (32-bit PE) with Homebrew LLVM. Needs: brew install llvm lld
set -e
cd "$(dirname "$0")"
L=${LLVM_BIN:-/opt/homebrew/opt/llvm/bin}
"$L/llvm-dlltool" -m i386 -k -d k32.def -l kernel32.lib
"$L/clang" --target=i686-pc-windows-msvc -O1 -ffreestanding -fno-stack-protector -c cmd.c -o cmd.obj
"$L/clang" --target=i686-pc-windows-msvc -fuse-ld=lld -nostdlib \
    -Wl,/entry:mainCRTStartup,/subsystem:console,kernel32.lib cmd.obj -o cmd.exe
rm -f cmd.obj kernel32.lib
