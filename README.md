# Krono's Battle Zone

Tank battle for 2 players, split-screen, hot-seat, or vs computer.

For DOSBOX / MS-DOS, made with C, DJGPP + Allegro 4.

Year 2001 - Released to the public March 2020

## Requirements

(**Old info**, year 2020 I recommend to use **DOSBOX emulator** instead of a real DOS machine)

Year 2020+ : GET https://www.dosbox.com/ and use the game there

If you're a retro purist, use real MS-DOS hardware:

The game requires a 386 with 4Mb of RAM, a hard drive with 2Mb free, DOS operating system and VGA video card with a resolution of 320x240
256 colors.

A 486DX2 with 8Mb of RAM or better is recommended, hard drive with 8Mb free and DOS 6.0 or higher operating system, and VESA 2.0 video card at 640x480 with 256 colors.

## Introduction

**Krono's Battle Zone** is an action game for one or two players, where the objective is to eliminate the opponent.

There is no time limit, or number of deaths, leaving this at the players' decision.

It is mainly intended for two players to play, but It can also be played against the computer.

You have various weapons and prizes to assist you in fighting your opponent.

## How to play

The game itself is controlled by the keyboard.

During the game, press **ESC** to end the game, and **PAUSE** to pause the game (**ENTER** to continue).

In **'debug'** mode, **F9, F10, F11 and F12** do different things.

## Keys

The player keys are:

### Player 1

Top screen, or full screen in case of playing vs CPU

Move tank: **LEFT, RIGHT, UP, DOWN** arrows

Turret: **INSERT** shoot, **DELETE** turn on / off lights (night only),
        **HOME, PG_UP** rotate turret

### Player 2

Lower screen

Move tank: **U, H, K, J**
Turret: **Q** shoot, **A** turn on / off lights (night only),
		**W, E** rotate turret

If you are watching a demo, ESC ends the demo.

## Command line parameters

-debug Activate debug mode

Effects:

-noluz = disable lights
-l0 = additive soft rendered light mode [DEFAULT]
-l1 = soft rendered light mode, NOT additive [WRONG]
-l2 = light mode not rendered, additive [QUICK!]

-nosplit = does not split the screen

Video modes:

-rVGA = VGA 320x200 mode [DEFAULT = FAST!]

All VGA X-MODES - Slow but some cute! ; ^)
They run on * almost * any VGA (I think all ...)

-r320240
-r320400
-r320480
-r320600
-r360200  
-r360240  
-r360360
-r360400
-r360480
-r360600

-r640480 Both modes of VESA 2.0 or higher.
-r800600 They require a lot of memory, they are slow, they look good, but
          they take away from the game because the area of ​​vision is very large.

I recommend leaving the VGA mode default for maximum performance even on a 486

640x480 to play two looks great

## TODO list

* Code needs to be beautified
* Code needs translation to English
* Many calls to Allegro are deprecated and should be replaced by the new calls
* Porting to Windows and Linux, removing the MS-DOS only routines (mainly console routines that can be replaced)
* The game has NO sound, never was finished. The WAVE files were recorded, but never finished it.
* The game needs to have a gamepad controller too
* The game needs to be able to configure KEYBOARD to custom keys
* Better IA for the computer opponent

## How to compile it

### DOSBOX / MS-DOS

You would need the following files for your toolchain:

Get them at : http://www.delorie.com/djgpp/

1. DOSBOX or a real MS-DOS machine (old hardware)

2. Allegro 4 game library for MS DOS
  * all422ar2.zip -- Allegro 4
  * all422br2.zip -- Allegro 4

3. DJGPP C compiler and tools
  * bnu234b.zip
  * csdpmi7b.zip
  * djdev205.zip
  * gcc920b.zip
  * gpp920b.zip
  * mak43b.zip

4. Misc tools
  * unzip32.exe -- Optional to unpack under MS-DOS
  * upx-3.96-dos.zip -- Optinal to pack the .exe
  * vc405sw.zip -- Volkov Commander - SUPER OPTIONAL TO MANAGE FILES UNDER MS-DOS

Install everything in your MS DOS box (or DOSBOX), test if the compiler works and is properly set-up per DJGPP instructions.

Then go to source/code and run **make.bat**

The compilation in a real 486DX2 lasts about 10 minutes, so be patient.  

### Windows

TODO needs to be ported yet

### Linux

TODO needs to be ported yet


## Credits

Copyright (c) 2001-2020, Krono from Argentina.

Thanks to all my family, friends, and UADE fellow students.

## Donations

If you find my code useful and want to donate, here is my address:

**TODO** - put btc address here

## Contributing

Just write on my GitHub with any contributions.

## License MIT

MIT License

Copyright (c) 2001-2020, Alvaro 'Krono' Gonzalez Ferrer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
