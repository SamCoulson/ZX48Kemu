ZX48Kemu
========

A *** Work in Progress *** 48K Spectrum emulator

This emulator uses an interpreted approach to emualte the Z80 CPU.  The screen and input are handled using the SDL Media Layer library.  

Requires a 48K spectrum ROM image to work

The build environment is:
Mingw32 + gmake + gvim
gcc (GCC) 4.4.7 20111023 (prerelease) [svn/rev.180339 - mingw-w64/oz]

Developed on:
Windows Vista - Home Premium, SP2

To build:

gmake

To run:

z80emu.exe

So far the screen shows the opening text for the boot ROM.  Rouhgly 50%-60% of the Z80 instructions have been implemented but much more work is required to check and test each instruction.

