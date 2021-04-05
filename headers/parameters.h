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

DOMAIN: Various
FILE: parameters.h
DESCRIPTION: Some parameters for emulation and the rest.
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSEPARAMETERS_H
#define SSEPARAMETERS_H

#if defined(__cplusplus)
#include "conditions.h"
#endif


//////////
// ACSI //
//////////

#define MAX_ACSI_DEVICES 8
#define ACSI_ID_LASER  7


/////////////
// BLITTER //
/////////////

// possibly effect of LINEW, which delays Blitter start until end of Write bus
// cycle - could be different if no CPU bus access at once?
#if defined(SSE_MEGASTE)
#define BLITTER_LATCH_LATENCY ((4*TICKS8)>>(Cpu16.ScuReg&1))
#else
#define BLITTER_LATCH_LATENCY (4*TICKS8) 
#endif
// we can see the delay on schematics but it's hard to follow - 4 clocks seems 
// to be most compatible
#define BLITTER_START_WAIT (4)
#define BLITTER_END_WAIT (4)


/////////
// CPU //
/////////

// main timer speed (ticks for 1 CPU cycle)
#if defined(SSE_TIMINGS32) // 64bit builds
#define TICKS8 4 // 32MHz
#else
#define TICKS8 1 //  8MHz
#endif

//#define CPU_MAX_HERTZ (2000000000) // 2ghz
#define CPU_MAX_HERTZ   (2048000000) // >2ghz

/*
The master clock crystal and derived CPU clock table is:
PAL (all variants)       32.084988   8.021247
NTSC (pre-STE)           32.0424     8.0106
Mega ST                  32.04245    8.0106125
NTSC (STE)               32.215905   8.053976
Peritel (STE) (as PAL)   32.084988   8.021247
Some STFs                32.02480    8.0071
*/

#if defined(SSE_TIMINGS32)
#define CPU_CLOCK_STF_NTSC  ((32042400*TICKS8)/4)
#define CPU_CLOCK_STF_PAL   ((32084988*TICKS8)/4)
#define CPU_CLOCK_MEGA_ST   ((32042450*TICKS8)/4)
#define CPU_CLOCK_STE_PAL   (CPU_CLOCK_STF_PAL)
#define CPU_CLOCK_STE_NTSC  ((32215905*TICKS8)/4)
#else
#define CPU_CLOCK_STF_NTSC   (8010600)
#define CPU_CLOCK_STF_PAL    (8021247) // precise
#define CPU_CLOCK_MEGA_ST    (8010613) // rounded
#define CPU_CLOCK_STE_PAL    (CPU_CLOCK_STF_PAL)
#define CPU_CLOCK_STE_NTSC   (8053976) // rounded
#endif

#define DBI_DELAY (1*TICKS8) // ~8mhz; notice we use < for comparison, not <=


///////////
// DEBUG //
///////////

#define TRACE_MAX_WRITES 200000 // to avoid too big file (debugger-only)
#define MAX_TRACE_CHARS 512//256

#if defined(SSE_DEBUGGER_FAKE_IO)
#define FAKE_IO_START 0xfffb00
#define FAKE_IO_LENGTH 64*2 // in bytes
#define FAKE_IO_END (FAKE_IO_START+FAKE_IO_LENGTH-2) // starting address of last one
#define STR_FAKE_IO_CONTROL "Control mask browser"
#endif

#if defined(SSE_DEBUGGER_FRAME_REPORT)
#if defined(SSE_UNIX)
#define FRAME_REPORT_FILENAME "./FrameReport.txt"
#else
#define FRAME_REPORT_FILENAME "FrameReport.txt"
#endif
#endif


///////////
// FILES //
///////////

#if defined(SSE_UNIX)
#define SSE_TRACE_FILE_NAME "./TRACE.txt"
#else
#define SSE_TRACE_FILE_NAME "TRACE.txt"
#endif
#define ACSI_HD_DIR "ACSI"
#define SSE_VID_RECORD_AVI_FILENAME "SteemVideo.avi"
#define DISK_HFE_BOOT_FILENAME "HFE_boot.bin"
#define DISK_IMAGE_DB "disk image list.txt"
#define HD6301_ROM_FILENAME "HD6301V1ST.img"
#define DRIVE_SOUND_DIRECTORY "DriveSound" // default
#define YM2149_FIXED_VOL_FILENAME "ym2149_fixed_vol.bin"
#define PASTI_DLL "pasti"
#define SSE_DISK_CAPS_PLUGIN_FILE "CAPSImg"
#define ARCHIVEACCESS_DLL "ArchiveAccess"
#define UNZIP_DLL "unzipd32" 
#define SSE_PLUGIN_DIR2 "plugins"
#ifdef SSE_X64
#define SSE_PLUGIN_DIR1 "plugins64"
#define UNRAR_DLL "unrar64"
#define MEMORY_SNAPSHOTS "snapshots64"
#if defined(SSE_DEBUG)
#define VIDEO_LOGIC_DLL "stvl64d"
#else
#define VIDEO_LOGIC_DLL "stvl64"
#endif
#else
#define SSE_PLUGIN_DIR1 "plugins32"
#define UNRAR_DLL "unrar" 
#define MEMORY_SNAPSHOTS "snapshots32"
#if defined(SSE_DEBUG)
#define VIDEO_LOGIC_DLL "stvl32d"
#else
#define VIDEO_LOGIC_DLL "stvl32"
#endif
#endif
#define STEEM_SSE_FAQ "FAQ (SSE)"
#define STEEM_HINTS "Hints"
#define STEEM_MANUAL_SSE "Steem SSE Manual" // file
#define FREE_IMAGE_DLL "FreeImage"
#if defined(SSE_STATS_RTF)
#define STEEM_STATS_FILENAME "stats.rtf"
#elif defined(SSE_UNIX)
#define STEEM_STATS_FILENAME "./stats.txt"
#else
#define STEEM_STATS_FILENAME "stats.txt"
#endif
#define SSE_SWEETFX_D3D_HACK "d3d9sweetfx" // original name d3d9.dll


//////////
// DISK //
//////////

#define SECTOR_SIZE 512
#define DISK_BYTES_PER_TRACK (6256)
/*  #bytes/track
    The value generally seen is 6250.
    The value for 11 sectors is 6256. It's possible if the clock is higher than
    8mhz, which is the case on the ST.
*/

#define FLOPPY_FF_VBL_COUNT 20
#define FLOPPY_MEDIACH_VBL 30
#define DISK_11SEC_INTERLEAVE 6
#define DRIVE_RPM 300
#define DRIVE_MAX_CYL 83
#define DRIVE_FAST_CYCLES_PER_BYTE (4*TICKS8)
#define DRIVE_FAST_IP_MULTIPLIER 8
#define FLOPPY_MAX_TRACK_NUM 85
#define FLOPPY_MAX_SECTOR_NUM 26
#define PASTI_FILE_EXTS_BUFFERSIZE 160


/////////
// GLU //
/////////

#define CYCLES_FROM_START_OF_HBL_IRQ_TO_WHEN_PEND_IS_CLEARED (28*TICKS8)


/////////
// GUI //
/////////

#define README_FONT_NAME "Courier New"
#define README_FONT_HEIGHT 16
#define EXT_TXT ".txt"
#define EXT_RTF ".rtf"
#define CONFIG_FILE_EXT "ini" // ini, cfg?
#define SHORTCUT_VBLS_BETWEEN_CHECKS 3 // ?
#define WINDOW_TITLE_MAX_CHARS 100
#if defined(SSE_BUILD)
#define SSE_VERSION_TXT_LEN 8// "3.7.0"
#define WINDOW_TITLE stem_window_title
#define APP_NAME "Steem SSE"
#define STEEM_WEB "https:/""/sourceforge.net/p/steemsse/"
#define STEEM_WEB_BUG_REPORTS "https:/""/sourceforge.net/p/steemsse/forum/bugs/"
#define DIDATABASE_WEB "http://steem.atari.st/database.htm" // still online
#define STEEM_WEB_LEGACY "http:/""/steem.atari.st/" // for links in Disk Image Database
#else
extern const char *stem_version_text;
#define STEEM_EMAIL "steem@gmx.net"
#define STEEM_WEB "http:/""/steem.atari.st/"
#define DIDATABASE_WEB STEEM_WEB "database.htm"
#endif
#define MSACONV_WEB "http:/""/msaconverter.free.fr/" //390 still valid
#define MAX_DIALOGS 20
#define COLOUR_CONTROL_BITMAP_H 150
#define GUI_FONT_SIZE 11
#define GUI_SMALLFONT_SIZE 12
#define GUI_BIGFONT_SIZE 16


//////////
// IKBD //
//////////

#define HD6301_CYCLE_DIVISOR (8*TICKS8) // the 6301 runs at 1MHz (verified by Stefan jL)
#define HD6301_CLOCK (1000000) //used in 6301/ireg.c for mouse speed
//#define HD6301_ROM_CHECKSUM 0296915D  // CRC32 - BTW this rom sends $F1 on reset
#define MAX_PC_JOYS 8
#define MAX_KEYBOARD_BUFFER_SIZE 128
#define IKBD_HBLS_FROM_COMMAND_WRITE_TO_PROCESS 5
#define IKBD_SCANLINES_FROM_ABS_MOUSE_POLL_TO_SEND ((MONO_MONITOR) ? 50 : 30)
#define IKBD_SCANLINES_FROM_JOY_POLL_TO_SEND ((MONO_MONITOR) ? 32 : 20)   // 32:20
#define IKBD_DEFAULT_MOUSE_MOVE_MAX 15
#define IKBD_RESET_MESSAGE 0xf1
#define MACRO_DEFAULT_ADD_MOUSE 1
#define MACRO_DEFAULT_ALLOW_VBLS 1
#define MACRO_DEFAULT_MAX_MOUSE 15
#define MACRO_RECORD_BUF_INC_SECS 20
#define DIJOY_MAX_DATAOBJECT 43
#define ACIA_CYCLES_NEEDED_TO_START_TX 512 // not OPTION_C1


////////
// IO //
////////

#define PORT_LOOPBUFSIZE 8192
#define MAX_SYSEX_BUFS 10


/////////
// MFP //
/////////

// for timers - the MC68901 itself runs at 4mhz on the ST
#define MFP_XTAL1 2457600 // default for STF
#if defined(SSE_410R3)
#define MFP_XTAL2 2457700 // default for STE
#else
#define MFP_XTAL2 2457721 // default for STE
#endif

// MFP emu is not low-level enough for direct handling of timer B, so we still use
// a delay hack
#define MFP_TIMER_B_DELAY (22*TICKS8)  // TIMERB01.TOS
#define MFP_TIMER_B_DELAY_STVL (18+2) // 2 less because we start from DE, not HDE, still stinky!
#define MFP_READ_TIMER_LATENCY (3*TICKS8) // TIMERR.TOS
#define MFP_TIMER_PRECISION 10000000 // 1000 in previous versions


//////////
// MISC //
//////////

#define MAX_AGENDA_LENGTH 32
#define STE_CLOCK8 8010613


/////////
// MMU //
/////////

// delay between GLUE 'DE' decision and first LOAD signal emitted by the MMU
// without waitstates
#define MMU_PREFETCH_LATENCY (8*TICKS8)
#define MEM_EXTRA_BYTES 320


/////////
// OSD //
/////////

#define OSD_MESSAGE_LENGTH 40 // in bytes excluding /0
#define OSD_MESSAGE_TIME 1 // in seconds
#define HD_TIMER 100 // Yellow hard disk led (imperfect timing)
#ifdef OSD_TEST_SCROLLERS
#define CHANCE_OF_SCROLLER 1 // 100%
#else
#define CHANCE_OF_SCROLLER 8 // 1/8
#endif
#define SECONDS_BETWEEN_SCROLLERS (60*5)


///////////
// SOUND //
///////////

#define SOUND_DESIRED_LQ_FREQ (50066/2)
#define YM_LOW_PASS_FREQ (10500) //in Hz, default
#define YM_LOW_PASS_MAX (22000)
#define MW_LATENCY_CYCLES ((128+16)*TICKS8) // quite the hack, TODO
#define MW_LOW_SHELF_FREQ 80 // officially 50 Hz
#define MW_HIGH_SHELF_FREQ (MIN(10000,(int)sound_freq/2)) // officially  15 kHz
#if defined(SSE_SOUND_16BIT_CENTRED) // see note in TYM2149::LoadFixedVolTable() 
#define PSG_CHANNEL_AMPLITUDE 40
#else
#define PSG_CHANNEL_AMPLITUDE 60
#endif
#define VOLTAGE_ZERO_LEVEL 0
#define PSG_BUFFER_FRAMES 3
#ifndef ONEGAME
#define PSG_WRITE_EXTRA 300
#else
#define PSG_WRITE_EXTRA OGExtraSamplesPerVBL
#endif
#define DEFAULT_SOUND_BUFFER_LENGTH (32768*SCREENS_PER_SOUND_VBL)


/////////
// TOS //
/////////

#if defined(SSE_TOS_PRG_AUTORUN)
#define AUTORUN_HD DRIVE_Z // Z: is used for PRG support (can't be a valid GEMDOS drive)
#endif
#define MAX_STEMDOS_FSNEXT_STRUCTS 100
#define MAX_STEMDOS_PEXEC_LIST 76 //Change loadsave_emu.cpp if change this! 
#define MAX_STEMDOS_FILES 255 // was 46, found no reference for it
#define STEMDOS_MAX_PATH 128
/*Dgetpath() As there is no way to specify the buffer size to this function you
should allow at least 128 bytes of buffer space. This will allow for up to 8
folders deep.*/
#define MAX_GEMDOS_HARDDRIVES 10


///////////
// VIDEO //
///////////

#define ORIGINAL_BORDER_SIDE 32
#define VERY_LARGE_BORDER_SIDE 50 // 52+48+320=420
#define LARGE_BORDER_SIDE 46 // 48+44+320=412
#define ORIGINAL_BORDER_BOTTOM 40
#define LARGE_BORDER_BOTTOM 45
#define VERY_LARGE_BORDER_BOTTOM 45
#define ORIGINAL_BORDER_TOP 30
#define BIG_BORDER_TOP 38 // 50hz
#define BIGGEST_BORDER 3 //420
#define ST_ASPECT_RATIO_DISTORTION 1.10f // multiplier for Y axis
#define ST_ASPECT_RATIO_DISTORTION_60HZ 1.25f // with no border
//#define SSE_VID_RECORD_AVI_CODEC "MPG4" //DD-only, poor result
#define SHIFTER_DEFAULT_WAKEUP (0) // 0: Spectrum512 compatible; -1: Mega STE
#define SHIFTER_MAX_WU_SHIFT (3) // - or +
#define AUTO_FRAMESKIP 8
#if defined(SSE_VID_32BIT_ONLY_410)
#define DRAW_TEMP_LINE_BUF_LEN (2*1024)
#elif defined(SSE_VID_SIZE4)
#define DRAW_TEMP_LINE_BUF_LEN (4*2*1024)
#else
#define DRAW_TEMP_LINE_BUF_LEN (4*1024) // 32bit pixels for 1 scanline, overkill
#endif
#define NSHIFTERCHANGES 32
#define MONO_HZ 72 //71  // it's between 71 and 72, 72 is more common on video cards
#define PAL_EXTRA_BYTES 16 // what for?
#define PAL_SIZE 16
#define NFRAME_TIME_AVG 18 // 18 for hires, originally 12

#endif//#ifndef SSEPARAMETERS_H
