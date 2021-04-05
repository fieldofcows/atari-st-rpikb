/*---------------------------------------------------------------------------
PROJECT: Steem SSE
Atari ST emulator
Copyright (C) 2021 by Anthony Hayward and Russel Hayward + SSE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see https://www.gnu.org/licenses/.

DOMAIN: GUI
FILE: resnum.h
DESCRIPTION: Declarations for resources.
---------------------------------------------------------------------------*/

#pragma once
#ifndef RESNUM_DECLA_H
#define RESNUM_DECLA_H

// resnum.h is included by rc\resource.rc, also by main.cpp
#include "../headers/SSE.h"


#define RC_ICO_APP 1
#define RC_ICO_APP256 2
#define RC_ICO_BACK 3
#define RC_ICO_OPS_ICONS 4
#define RC_ICO_CHIP 5
#define RC_ICO_DISKMAN 6
#define RC_ICO_SOUND 7
#define RC_ICO_DRIVEDROPDOWN 8
#define RC_ICO_FF 9
#define RC_ICO_FORWARD 10
#define RC_ICO_HARDDRIVES 11
#define RC_ICO_HOME 12
#define RC_ICO_INFO 13
#define RC_ICO_JOY 14
#define RC_ICO_EXTERNAL 15
#define RC_ICO_TOWINDOW 16
#define RC_ICO_OPTIONS 17
#define RC_ICO_RESET 18
#define RC_ICO_PLAY 19
#define RC_ICO_SETHOME 20
#define RC_ICO_SHORTCUT 21
#define RC_ICO_SNAPSHOTBUT 22
#define RC_ICO_DRIVEA 23
#define RC_ICO_DRIVEB 24
#define RC_ICO_STCLOSE 25
#define RC_ICO_DRIVE 26
#define RC_ICO_DRIVELINK 27
#define RC_ICO_DRIVEBROKEN 28
#define RC_ICO_DRIVEREADONLY 29
#define RC_ICO_DRIVEZIPPED_RO 30
#define RC_ICO_FOLDER 31
#define RC_ICO_FOLDERLINK 32
#define RC_ICO_FOLDERBROKEN 33
#define RC_ICO_HARDDRIVE16 34
#define RC_ICO_BOMB 35
#define RC_ICO_UPDATE 36
#define RC_ICO_PARENTDIR 37
#define RC_ICO_SNAPSHOTFILEICO 38
#define RC_ICO_PRGFILEICO 39
#define RC_ICO_SMALLDOWNARROW 40
#define RC_ICO_RECORD 41
#define RC_ICO_PATCHES 42
#define RC_ICO_PATCHESNEW 43
#define RC_ICO_OPS_GENERAL 44
#define RC_ICO_OPS_DISPLAY 45
#define RC_ICO_OPS_BRIGHTCON 46
#define RC_ICO_OPS_FULLSCREEN 48
#define RC_ICO_OPS_MIDI 49
#define RC_ICO_OPS_WINDOWED RC_ICO_TOWINDOW
#define RC_ICO_OPS_SOUND RC_ICO_SOUND
#define RC_ICO_OPS_UPDATE RC_ICO_UPDATE
#define RC_ICO_TEXT 50
#define RC_ICO_FUJILINK 51
#define RC_ICO_INFO_CLOCK 52
#define RC_ICO_INFO_FAQ 53
#define RC_ICO_PASTE 54
#define RC_ICO_OPS_MACHINE 55
#define RC_ICO_OPS_ASSOC 56
#define RC_ICO_DRIVEB_DISABLED 57
#define RC_ICO_OPS_MACROS 58
#define RC_ICO_OPS_PROFILES 59
#define RC_ICO_PLAY_BIG 60
#define RC_ICO_SHORTCUT_ON 61
#define RC_ICO_SHORTCUT_OFF 62
#define RC_ICO_ACCURATEFDC 63
#define RC_ICO_RESETGLOW 64
#define RC_ICO_FULLQUIT 47
#define RC_ICO_PCFOLDER 65
#define RC_ICO_DISK_HOWTO 66
#define RC_ICO_CART_HOWTO 67
#define RC_ICO_DRIVEZIPPED_RW 68
#define RC_ICO_HARDDRIVES_FR 69
#define RC_ICO_OPS_STARTUP 70
#define RC_ICO_OPS_OSD 71
#define RC_ICO_TAKESCREENSHOTBUT 72
#define RC_ICO_DISKMANTOOLS 73


#define RC_ICO_HARDDRIVES_ACSI 74
#define RC_ICO_OPS_SSE 75
#define RC_ICO_OPS_C1 76
#define RC_ICO_OPS_C2 77
#define RC_ICO_CFG 78
#define RC_ICO_OPS_HACKS 79
#define RC_ICO_OPS_C3 80
#define RC_ICO_OPS_LL 81
#define RC_ICO_OPS_KBDMOUSE 82
#define RC_ICO_CPUALTSPEED 83
#define RC_ICO_OPS_MACHINE2 84

#define RC_ICO_GREYDISK 85
#define RC_ICO_BLUEDISK 86
#define RC_ICO_STCUR 87
#define RC_NUM_ICONS 88



#define CART_ICON_NUM (RC_ICO_CHIP-1)
#define DISK_ICON_NUM (RC_ICO_DRIVE-1)
#define SNAP_ICON_NUM (RC_ICO_SNAPSHOTFILEICO-1)
#define PRG_ICON_NUM (RC_ICO_PRGFILEICO-1)

#define RCNUM(i) (LPTSTR) ((DWORD_PTR) ((WORD) (i)))

int RCGetSizeOfIcon(INT_PTR n);


#define IDR_STEEMNEW  101
#define IDR_HD6301    102
#define IDR_YM2149 103
#define IDR_HFEBOOT 104
#define IDR_MANUAL 105

#define DEBUG_ICONS_W 9
#define DEBUG_ICONS_H 9
#define DEBUG_NUM_ICONS 9


#endif//RESNUM_DECLA_H
