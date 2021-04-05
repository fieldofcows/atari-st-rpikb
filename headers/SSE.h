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

DOMAIN: All
FILE: SSE.h
DESCRIPTION: Compilation directives (similar to conditions.h, this one is
specific to Steem SSE)
---------------------------------------------------------------------------*/

// for v4.1.0
#pragma once
#ifndef SSE_H
#define SSE_H

/*
ST Enhanced EMulator Sensei Software Edition (Steem SSE)
--------------------------------------------------------

SVN code repository is at:
https://sourceforge.net/projects/steemsse/

This is based on the source code for Steem R63 as released by Steem authors,
Ant & Russ Hayward, at
https://github.com/steem-engine/steem-engine

STEVEN_SEAGAL doesn't need to be defined anymore: the end of an era!

SSE_DEBUG, if needed, should be defined in the project/makefile.
It has an effect on both the Debugger and the Visual Studio _DEBUG builds.
DEBUG_BUILD for the Debugger (Windows-only)
SSE_DD for the DirectDraw build (2 or 7), Direct3D by default (Windows)
SSE_RELEASE for official releases (SSE2 processor requirement in Windows)
BCC_BUILD for the Borland C++ builds (development-only)
MINGW_BUILD for the MinGW32 builds (development-only)
VC_BUILD for all Visual C++ builds
SSE_STDCALL for the __stdcall calling convention (default in v4, VC builds only)

See Steem.cpp for other project defines

SSE.h is supposed to mainly be a collection of compiling switches (defines).
It should include nothing and can be included everywhere.
It is included in pch (precompiled header), any change in SSE.h triggers
compilation of about everything.
*/

#define SSE_BUILD
#define NO_RARLIB // don't use rarlib, use unrar
#define SSE_NO_INTERNAL_SPEAKER
#define SSE_NO_UPDATE // remove all update code
#define SSE_NO_WINSTON_IMPORT // nuke WinSTon import
#define SSE_NO_FALCONMODE //v402
//#define SSE_NO_INTRO
//#define SSE_VID_NO_FREEIMAGE
#define SSE_VID_NO_SCREENSAVER
#define SSE_LEAN_AND_MEAN

#if defined(SSE_RELEASE)
#define SSE_VERSION 410
#define SSE_VERSION_R 5
#else
#define SSE_BETA
#define SSE_VERSION 410
#define SSE_VERSION_R 5
#endif

#if defined(SSE_BETA)
//#define SSE_PRIVATE_BUILD // my "beta" option
#endif

#ifdef UNIX
#define SSE_UNIX // XSteem SSE
#define SSE_RTAUDIO_LARGER_BUFFER //simplistic attempt
#define SSE_UNIX_OPTIONS_SSE_ICON
#define SSE_UNIX_STATIC_VAR_INIT //odd
#endif

#ifdef _WIN64
#define SSE_X64
#define SSE_X64_DEBUG
#ifndef SSE_DRAW_C
#define SSE_DRAW_C
#endif
#if (_MSC_VER>=1900) // VS2015
#define SSE_X64_GUI // horrible problems
#endif
#define SSE_NO_UNZIPD32 // remove code for unzipd32.dll
#define SSE_TIMINGS32 // internally run at 32MHz instead of 8MHz
#define SSE_TIMINGS32A // spurious interrupt no rand()
#endif

#ifdef WIN32
#ifndef BCC_BUILD
#define SSE_WINDOWS_2000 // needed for 2 screens + CPU %
#endif
#if defined(VC_BUILD) && _MSC_VER>=1500 //VS2008+
#define SSE_VC_INTRINSICS
#endif
#define SSE_WINDOWS_XP // staying XP-compatible (32bit + 64bit, there are users)
#define SSE_WIN32_A // technical stuff
#endif


// Exception management...
//#define SSE_M68K_EXCEPTION_TRY_CATCH //works but too slow, especially if _DEBUG
#ifndef _DEBUG
#define SSE_MAIN_LOOP
#if defined(SSE_MAIN_LOOP)
#define SSE_MAIN_LOOP1
#if _MSC_VER >= 1500
#define SSE_MAIN_LOOP2 //VC only
#endif
#endif
#endif


// Feature switches, still a few, it's nothing compared with before!
#define SSE_ACSI // hard drive
#define SSE_ACSI_LASER // PBM files
#define SSE_CPU_MFP_RATIO_OPTION // user can fine tune CPU clock
#define SSE_DISK_CAPS // IPF, CTR disk images
//#define SSE_DISK_CAPS_MEMORY // file in memory
#define SSE_DISK_STW // MFM disk image format
#define SSE_DISK_HFE // HxC floppy emulator HFE (v.1) image support
#define SSE_DISK_SWAPPER // smart
#define SSE_DRIVE_NOSNAIL
#define SSE_DRIVE_SOUND // poor imitation of a SainT feature
//#define SSE_DRIVE_SOUND_2SEEKS // optional back & fwd SEEK samples
#define SSE_GUI_OPTIONS_MICROWIRE
#define SSE_HARDWARE_OVERSCAN
#define SSE_HD6301_LL // using 3rd party code
#define SSE_IKBD_FRENCH // hack for 4
#define SSE_IKBDI // command interpreter
#define SSE_IKBD_MAPPINGFILE // v402
#define SSE_IKBD_RTC // battery-powered 6301
#define SSE_INT_MFP_EVENT_IRQ // interrupts in event count mode - to check
#define SSE_MEGASTE // ultimate model for Steem SSE
#define SSE_MEGA_RTC // Ricoh chip //TODO linux
#define SSE_MMU_MONSTER_ALT_RAM // HW hack for ST
#define SSE_ONEINSTANCE
#define SSE_OSD_FPS_INFO
#define SSE_PRINTER // bold, italics... + graphics on RTF
#define SSE_SHIFTER_UNSTABLE
#define SSE_SOUND_16BIT_CENTRED
#define SSE_SOUND_CARTRIDGE // B.A.T etc.
#define SSE_SOUND_DS // DirectSound mods
#define SSE_SOUND_ENFORCE_RECOM_OPT // no more exotic options cluttering the page //Unix?
#define SSE_STATS // v401 file + window telling what the ST program is doing
#define SSE_TOS_KEYBOARD_CLICK // hack to suppress the click
#define SSE_UNRAR_SUPPORT
#define SSE_VID_BORDERS
#define SSE_VID_CHECK_VIDEO_RAM // update display before writes to video memory
#define SSE_VID_NEOPIC
#define SSE_VID_SIZE4
#define SSE_YM2149_LL // low-level emu (3rd party-inspired)
#define SSE_WD1772_CRCTABLE


#ifdef WIN32 //todo, some features could be added to XSteem
#define SSE_ARCHIVEACCESS_SUPPORT // 7z + ...
#ifdef SSE_DISK_STW
#define SSE_DISK_AUTOSTW
#endif
#define SSE_DISK_GHOST // save hiscores of STX etc.
#define SSE_DONGLE     // special adapters (including protection dongles)
#define SSE_DRIVE_FREEBOOT // + single-sided
#define SSE_EMU_THREAD // reduces hiccups
#define SSE_FILES_IN_RC
#define SSE_GUI_ALT_MENU // for keyboard control
#define SSE_GUI_BIGICONS // for high DPI screens
#define SSE_GUI_CONFIG_FILE // icon in tool bar
#define SSE_GUI_CONFIG_RELATIVE
#define SSE_GUI_DEFAULT_ST_CONFIG // option Default ST config
#define SSE_GUI_FONT
#define SSE_GUI_KBD // better keyboard control
#define SSE_GUI_MENUBAR // menu bar toggle
#define SSE_GUI_RICHEDIT
#define SSE_GUI_RICHEDIT2 // links mod
//#define SSE_GUI_SPURIOUS
#define SSE_GUI_STATUS_BAR
#define SSE_GUI_TOOLBAR // toolbar is optional
#define SSE_JOYSTICK_JUMP_BUTTON
#define SSE_JOYSTICK_NO_MM //circle around unsolved bug (ours or theirs?)
#define SSE_OPTION_FASTBLITTER // v402
#define SSE_OPTION_FASTLINEA // v410
#define SSE_OSD_DEBUGINFO // in rlz build
#define SSE_OSD_SHOW_TIME // measure time you waste
#define SSE_UNRAR_SUPPORT_WIN // using unrar.dll
#define SSE_SOUND_MICROWIRE_12DB_HACK
#define SSE_STATS_RTF
#define SSE_TOS_PRG_AUTORUN // Atari PRG + TOS file direct support
#define SSE_VID_SCANLINES_INTERPOLATED
#define SSE_VID_VSYNC_WINDOW
#endif//WIN32

#if defined(SSE_WINDOWS_2000)
#ifndef MINGW_BUILD
#define SSE_STATS_CPU // using cool 3rd party function
#endif
#define SSE_VID_2SCREENS
#endif

#define SSE_VID_32BIT_ONLY //TODO?

#if defined(SSE_VID_32BIT_ONLY)
#define SSE_DRAW_MARSHALL // video rendering refactoring
#define SSE_DRAW_MARSHALL2 // performance: marshall parameters only when necessary
#define SSE_DEBUGGER_NODRAW // great improvement!
#endif//#if defined(SSE_VID_32BIT_ONLY)

#if defined(SSE_DRAW_C) && defined(SSE_VID_32BIT_ONLY)
#define SSE_VID_32BIT_ONLY_410 // draw_dest_ad is DWORD*
#endif

#if defined(SSE_GUI_STATUS_BAR)
//#define SSE_GUI_STATUS_BAR_DRAW_FREQ // for different colour
//#define SSE_GUI_STATUS_BAR_DRAW_MAIN  // ?
#define SSE_GUI_STATUS_BAR_DRAW_ICONS // anyway
//#define SSE_GUI_STATUS_BAR_DRAW_CAPS  // too heavy
#define SSE_GUI_STATUS_BAR_MOUSE
#endif

#if defined(SSE_DISK_CAPS) // dependency!
#define SSE_WD1772_LL // low-level elements (3rd party-inspired)
#define SSE_DISK_SCP // Supercard Pro disk image format support
#endif

#if defined(SSE_DONGLE)
#define SSE_DONGLE_PORT // all dongles grouped in "virtual" port
#define SSE_DONGLE_BAT2
#define SSE_DONGLE_CRICKET
#define SSE_DONGLE_JEANNEDARC
#define SSE_DONGLE_PROSOUND // Wings of Death, Lethal Xcess  STF
#define SSE_DONGLE_LEADERBOARD
#define SSE_DONGLE_MULTIFACE
#define SSE_DONGLE_MUSIC_MASTER
#define SSE_DONGLE_URC
#endif

#ifdef WIN32
// The Windows builds can optionally use either DirectDraw (DD) or 
// Direct3D9 (D3D),not both
// Use SSE_DD as compile directive (config, makefile) for a DirectDraw build
// since v402R4:
// Use SSE_D3D for a Direct3D9 build
// If none of them is defined, a GDI-only build will be produced
#if defined(SSE_DD)
#define SSE_VID_DD
#ifdef VC_BUILD
#define SSE_VID_DD7 // if not defined, DirectDraw2
#endif
#endif
#if defined(SSE_D3D)
#define SSE_VID_D3D
#endif
#endif//WIN32

#if defined(SSE_VID_DD) // DirectDraw
#define SSE_VID_DD_3BUFFER_WIN // window Triple Buffering (DD-only)
#define SSE_VID_DD_MISC // compatibility issues
#ifndef MINGW_BUILD
#define SSE_VID_RECORD_AVI //avifile not so good 
#endif
#endif//dd

#if defined(SSE_VID_D3D) // Direct3D
#define SSE_VID_D3D_FAKE_FULLSCREEN
#define SSE_VID_D3D_SWEETFX // D3D hack
#if defined(SSE_DRAW_MARSHALL)
#define SSE_VID_D3D_VSYNC
#define SSE_VID_D3D_AUTOVSYNC
#endif
#endif//d3d


#ifdef UNIX
#define SSE_7Z_SUPPORT_UNIX
#ifndef SSE_LINUX_DYN // environment setting (could reverse that)
#define SSE_DISK_CAPS_STATIC
#endif
#define SSE_UNIX_PULSEAUDIO
#define SSE_UNRAR_SUPPORT_UNIX
#define SSE_VID_32BIT_ONLY
#define SSE_VID_DISABLE_AUTOBORDER
#define SSE_VID_NO_FREEIMAGE
#if defined(SSE_ACSI) //410R2
#define SSE_ACSI_OPTION //while there's no manager (coming in v411!)
#endif
#endif//UNIX


#ifdef WIN32
#if !defined(SSE_LLVLE)
#define SSE_LLVLE // used to be defined in environment
#endif
#endif

#if defined(SSE_LLVLE)
#define SSE_VID_STVL // use low-level video logic plugin
#if defined(SSE_VID_STVL)
#define SSE_VID_STVL1 // misc
#define SSE_VID_STVL2 // keep STVL regs up-to-date
#define SSE_VID_STVL_SREQ // callback fetch ste sound
#ifdef SSE_DEBUG
#define SSE_VID_STVL3 // use debug facilities
#endif
#endif
#endif//llvle


///////////
// DEBUG //
///////////

// SSE_DEBUG is defined or not by the environment
// debug switches are grouped here

#define SSE_DEBUG_TRACE // all builds now
#ifdef WIN32
#define SSE_DEBUG_TRACE_LOCK
#endif
#ifdef _DEBUG // VC
#define SSE_DEBUG_TRACE_IDE
#endif
#if defined(SSE_UNIX)
#define SSE_UNIX_TRACE
#endif

#if defined(SSE_DEBUG) // Debugger + debug build

#if defined(DEBUG_BUILD)
#define SSE_DEBUGGER
#endif

#if defined(SSE_DEBUGGER)
#define SSE_DEBUGGER_FAKE_IO //to control some debug options
#define SSE_DEBUGGER_MONITOR_RANGE // will stop for every address between 2 stops
#define SSE_DEBUGGER_MONITOR_VALUE // specify value (RW) that triggers stop
#define SSE_DEBUGGER_STATUS_BAR
#define SSE_DEBUGGER_TOGGLE // click on bomb to toggle
#define SSE_DEBUGGER_TRACE_EVENTS
//#define SSE_IKBD_6301_DISASSEMBLE_ROM // once is enough
#endif//Debugger

#if defined(SSE_DEBUGGER_FAKE_IO)
#define SSE_DEBUGGER_MUTE_SOUNDCHANNELS
#define SSE_DEBUGGER_FRAME_REPORT
#define SSE_DEBUGGER_TOPOFF
#endif//fake io

#endif//SSE_DEBUG


//////////////
// BUGFIXES //
//////////////

#define SSE_410R1
#define SSE_410R2
#define SSE_410R3
#define SSE_410R4
#define SSE_410R5


///////////////////////
// DEV BUILD RELEASE //
///////////////////////


////////////////////
// DEV BUILD BETA //
////////////////////

#if defined(SSE_BETA)
#define TEST01//quick switch
#define TEST02//track bug
//#define TEST03
//#define TEST04
//#define TEST05
//#define TEST06 
//#define TEST07
//#define TEST08
//#define TEST09

//#define OSD_TEST_SCROLLERS
//#define SSE_HIRES_COLOUR // as POC
//#define SSE_INT_MFP_TIMER_B_PULSE
//#define SSE_VID_TRACE_SIZE
#define SSE_FORCE_TRACE_VIDEO_RENDERING // force TRACE.txt and log that section
//#define TEST_STEEM_INTRO
//#define SSE_OSD_EXTRACT_GRAPHICS // one-time switch
#endif

#if defined(SSE_PRIVATE_BUILD)
#define SSE_GUI_OPTION_FOR_TESTS
#endif

#endif//#ifndef SSE_H 
