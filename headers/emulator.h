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

DOMAIN: Emu
FILE: emulator.h
DESCRIPTION: Declarations for miscellaneous core emulator functions and
objects.
struct TAgenda, TRp5c15, TStats, TMegaSte, TCpu16, TDiskEmu
---------------------------------------------------------------------------*/

#pragma once
#ifndef EMULATOR_DECLA_H
#define EMULATOR_DECLA_H

#include <dynamicarray.h>
#include "conditions.h"
#include "blitter.h"
#include "steemh.h"
#include "interface_caps.h"
#include "disk_ghost.h"
#include "disk_stw.h"
#include "disk_scp.h"
#include "disk_hfe.h"
#include "acsi.h"
#include "palette.h"
#if defined(SSE_STATS_CPU)
#include <CpuUsage/CpuUsage.h>
#endif

#pragma pack(push, 8) //402R23

#define STRS(a) itoa((a),d2_t_buf,10)
#define HEXS(a) itoa((a),d2_t_buf,16)

#define BITS_ba9 0xe00
#define BITS_876 0x1c0
#define BITS_543 0x038

#define BITS_ba9_000 0x000
#define BITS_ba9_001 0x200
#define BITS_ba9_010 0x400
#define BITS_ba9_011 0x600
#define BITS_ba9_100 0x800
#define BITS_ba9_101 0xa00
#define BITS_ba9_110 0xc00
#define BITS_ba9_111 0xe00

#define BITS_876_000 0x000
#define BITS_876_001 0x040
#define BITS_876_010 0x080
#define BITS_876_011 0x0c0
#define BITS_876_100 0x100
#define BITS_876_101 0x140
#define BITS_876_110 0x180
#define BITS_876_111 0x1c0

#define BITS_543_000 0x00
#define BITS_543_001 0x08
#define BITS_543_010 0x10
#define BITS_543_011 0x18
#define BITS_543_100 0x20
#define BITS_543_101 0x28
#define BITS_543_110 0x30
#define BITS_543_111 0x38


#define B6_000000 0
#define B6_000001 1
#define B6_000010 2
#define B6_000011 3
#define B6_000100 4
#define B6_000101 5
#define B6_000110 6
#define B6_000111 7
#define B6_001000 8
#define B6_001001 9
#define B6_001010 10
#define B6_001011 11
#define B6_001100 12
#define B6_001101 13
#define B6_001110 14
#define B6_001111 15
#define B6_010000 16
#define B6_010001 17
#define B6_010010 18
#define B6_010011 19
#define B6_010100 20
#define B6_010101 21
#define B6_010110 22
#define B6_010111 23
#define B6_011000 24
#define B6_011001 25
#define B6_011010 26
#define B6_011011 27
#define B6_011100 28
#define B6_011101 29
#define B6_011110 30
#define B6_011111 31
#define B6_100000 32
#define B6_100001 33
#define B6_100010 34
#define B6_100011 35
#define B6_100100 36
#define B6_100101 37
#define B6_100110 38
#define B6_100111 39
#define B6_101000 40
#define B6_101001 41
#define B6_101010 42
#define B6_101011 43
#define B6_101100 44
#define B6_101101 45
#define B6_101110 46
#define B6_101111 47
#define B6_110000 48
#define B6_110001 49
#define B6_110010 50
#define B6_110011 51
#define B6_110100 52
#define B6_110101 53
#define B6_110110 54
#define B6_110111 55
#define B6_111000 56
#define B6_111001 57
#define B6_111010 58
#define B6_111011 59
#define B6_111100 60
#define B6_111101 61
#define B6_111110 62
#define B6_111111 63


#define MSB_B (BYTE)0x80
#define MSB_W (WORD)0x8000
#define MSB_L (DWORD)0x80000000


#ifdef DEBUG_BUILD
#define STEM_MODE_CPU 0
#define STEM_MODE_D2 1
#define STEM_MODE_INSPECT 2
#else
#define DEBUG_CHECK_WRITE_B(ad) 
#define DEBUG_CHECK_WRITE_W(ad)
#define DEBUG_CHECK_WRITE_L(ad)
#define DEBUG_CHECK_READ_B(ad)
#define DEBUG_CHECK_READ_W(ad)
#define DEBUG_CHECK_READ_L(ad)
#define DEBUG_CHECK_WRITE_IO_B(ad,v)
#define DEBUG_CHECK_WRITE_IO_W(ad,v)
#define DEBUG_CHECK_WRITE_IO_L(ad,v)
#define DEBUG_CHECK_READ_IO_B(ad)
#define DEBUG_CHECK_READ_IO_W(ad)
#define DEBUG_CHECK_READ_IO_L(ad)
#endif



#if defined(DEBUG_BUILD) && defined(PEEK_RANGE_TEST)

void RangeError(DWORD &ad,DWORD hi_ad);

// Have to allow pointer to last byte to be returned for lpDPEEK (SET_PC)

BYTE& PEEK(DWORD ad);
WORD& DPEEK(DWORD ad);
DWORD& LPEEK(DWORD ad);
BYTE* lpPEEK(DWORD ad);
WORD* lpDPEEK(DWORD ad);
DWORD* lpLPEEK(DWORD ad);

BYTE& ROM_PEEK(DWORD ad);
WORD& ROM_DPEEK(DWORD ad);
DWORD& ROM_LPEEK(DWORD ad);
BYTE* lpROM_PEEK(DWORD ad);
WORD* lpROM_DPEEK(DWORD ad);
DWORD* lpROM_LPEEK(DWORD ad);

BYTE& CART_PEEK(DWORD ad);
WORD& CART_DPEEK(DWORD ad);
DWORD& CART_LPEEK(DWORD ad);
BYTE* lpCART_PEEK(DWORD ad);
WORD* lpCART_DPEEK(DWORD ad);
DWORD* lpCART_LPEEK(DWORD ad);

#define PAL_DPEEK(l)   *(WORD*)(palette_exec_mem+64+PAL_EXTRA_BYTES-2-(l))
#define lpPAL_DPEEK(l) (WORD*)(palette_exec_mem+64+PAL_EXTRA_BYTES-2-(l))

#else

#ifndef BIG_ENDIAN_PROCESSOR
//little endian version (x86,x64...)

#define PEEK(l)    *(BYTE*)(Mem_End_minus_1-(l))
#define DPEEK(l)   *(WORD*)(Mem_End_minus_2-(l))
#define LPEEK(l)   *(DWORD*)(Mem_End_minus_4-(l))
#define lpPEEK(l)  (BYTE*)(Mem_End_minus_1-(l))
#define lpDPEEK(l) (WORD*)(Mem_End_minus_2-(l))
#define lpLPEEK(l) (DWORD*)(Mem_End_minus_4-(l))

#define ROM_PEEK(l)    *(BYTE*)(Rom_End_minus_1-(l))
#define ROM_DPEEK(l)   *(WORD*)(Rom_End_minus_2-(l))
#define ROM_LPEEK(l)   *(DWORD*)(Rom_End_minus_4-(l))
#define lpROM_PEEK(l)  (BYTE*)(Rom_End_minus_1-(l))
#define lpROM_DPEEK(l) (WORD*)(Rom_End_minus_2-(l))
#define lpROM_LPEEK(l) (DWORD*)(Rom_End_minus_4-(l))

#define CART_PEEK(l)    *(BYTE*)(Cart_End_minus_1-(l))
#define CART_DPEEK(l)   *(WORD*)(Cart_End_minus_2-(l))
#define CART_LPEEK(l)   *(DWORD*)(Cart_End_minus_4-(l))
#define lpCART_PEEK(l)  (BYTE*)(Cart_End_minus_1-(l))
#define lpCART_DPEEK(l) (WORD*)(Cart_End_minus_2-(l))
#define lpCART_LPEEK(l) (DWORD*)(Cart_End_minus_4-(l))

#define PAL_DPEEK(l)   *(WORD*)(palette_exec_mem+64+PAL_EXTRA_BYTES-2-(l))
#define lpPAL_DPEEK(l) (WORD*)(palette_exec_mem+64+PAL_EXTRA_BYTES-2-(l))

#else

#define PEEK(l)    *(BYTE*)(STMem+(l))
#define DPEEK(l)   *(WORD*)(STMem+(l))
#define LPEEK(l)   *(DWORD*)(STMem+(l))
#define lpPEEK(l)  (BYTE*)(STMem+(l))
#define lpDPEEK(l) (WORD*)(STMem+(l))
#define lpLPEEK(l) (DWORD*)(STMem+(l))

#define ROM_PEEK(l)    *(BYTE*)(STRom+(l))
#define ROM_DPEEK(l)   *(WORD*)(STRom+(l))
#define ROM_LPEEK(l)   *(DWORD*)(STRom+(l))
#define lpROM_PEEK(l)  (BYTE*)(STRom+(l))
#define lpROM_DPEEK(l) (WORD*)(STRom+(l))
#define lpROM_LPEEK(l) (DWORD*)(STRom+(l))

#define CART_PEEK(l)    *(BYTE*)(cart+(l))
#define CART_DPEEK(l)   *(WORD*)(cart+(l))
#define CART_LPEEK(l)   *(DWORD*)(cart+(l))
#define lpCART_PEEK(l)  (BYTE*)(cart+(l))
#define lpCART_DPEEK(l) (WORD*)(cart+(l))
#define lpCART_LPEEK(l) (DWORD*)(cart+(l))

#define PAL_DPEEK(l)   *(WORD*)(palette_exec_mem+(l))
#define lpPAL_DPEEK(l) (WORD*)(palette_exec_mem+(l))

#endif

#endif

extern BYTE SafePeek(MEM_ADDRESS ad);
extern WORD SafeDPeek(MEM_ADDRESS ad);
extern DWORD SafeLPeek(MEM_ADDRESS ad);
extern void SafePoke(MEM_ADDRESS ad,BYTE val);
extern void SafeDPoke(MEM_ADDRESS ad,WORD val);
extern void SafeLPoke(MEM_ADDRESS ad,DWORD val);

#ifdef WIN32
#if !defined(VC_BUILD) && !defined(MINGW_BUILD)
extern void _RTLENTRY __int__(int);
#define INTR(i) __int__(i)
#endif
#endif


/////////
// Bus //
/////////

// this is Steem SSE arbitrary internal coding for bus access
#define BUS_MASK_HIBYTE BIT_4
#define BUS_MASK_LOBYTE BIT_3
#define BUS_MASK_WORD (BUS_MASK_HIBYTE|BUS_MASK_LOBYTE)
#define BUS_MASK_FETCH BIT_2
#define BUS_MASK_WRITE BIT_1
#define BUS_MASK_ACCESS BIT_0
#define BUS_MASK bus_mask
// fetch:
//  BUS_MASK=BUS_MASK_ACCESS|BUS_MASK_FETCH|BUS_MASK_WORD;
// read byte:
// BUS_MASK=(iabus&1)? (BUS_MASK_ACCESS|BUS_MASK_LOBYTE) : (BUS_MASK_ACCESS|BUS_MASK_HIBYTE);
// read word
//  BUS_MASK=BUS_MASK_ACCESS|BUS_MASK_WORD;
// write byte
//  BUS_MASK=(iabus&1)? (BUS_MASK_ACCESS|BUS_MASK_WRITE|BUS_MASK_LOBYTE) : (BUS_MASK_ACCESS|BUS_MASK_WRITE|BUS_MASK_HIBYTE);
// write word
//  BUS_MASK=BUS_MASK_ACCESS|BUS_MASK_WRITE|BUS_MASK_WORD;

extern WORD &dbus;
extern BYTE &dbush;
extern BYTE &dbusl;

#if defined(SSE_VID_STVL1)
extern DU16 &udbus;
extern MEM_ADDRESS& abus;
extern BYTE &bus_mask;
#else
extern DU16 udbus;
extern MEM_ADDRESS abus;
extern BYTE bus_mask;
#endif

#define BUS_SAVE\
  MEM_ADDRESS iabus_b4=iabus;\
  MEM_ADDRESS abus_b4=abus;\
  WORD dbus_b4=dbus;\
  BYTE ds_mask_b4=BUS_MASK;

#define BUS_RESTORE\
  iabus=iabus_b4;\
  abus=abus_b4;\
  dbus=dbus_b4;\
  BUS_MASK=ds_mask_b4;


/////////////
// Agendas //
/////////////

// Steem can still be confused if frequency changes right after a disk command
#define HBL_PER_SECOND (hbl_per_second[VideoFreqIdx])
#define HBLS_PER_SECOND_AVE 15700 // Average between 50 and 60hz
#define HBLS_PER_SECOND_MONO (501.0*71.42857)
extern const WORD hbl_per_second[4]; 

typedef void AGENDAPROC(int);
typedef AGENDAPROC* LPAGENDAPROC;

struct TAgenda{
  LPAGENDAPROC perform;
  unsigned long time;
  int param;
};

extern TAgenda agenda[MAX_AGENDA_LENGTH];
extern WORD agenda_length;
extern unsigned long agenda_next_time;
extern unsigned long hbl_count;

void agenda_add(LPAGENDAPROC action,int pause,int param);
void agenda_delete(LPAGENDAPROC job);
int agenda_get_queue_pos(LPAGENDAPROC job);
int milliseconds_to_hbl(int);

// This list is used to reinit the agendas after loading a snapshot
// add any new agendas to the end of the list, replace old agendas
// with NULL.
//extern LPAGENDAPROC agenda_list[]; // BCC don't like that
extern LPAGENDAPROC agenda_list[15+4+1+1+1+1
#if defined(SSE_ACSI_LASER)
  +1
#endif
];

WIN_ONLY( extern CRITICAL_SECTION agenda_cs; )


/////////////
// Timings //
/////////////

void init_timings();
extern COUNTER_VAR sys_timer; // was cpu_timer
extern int sys_cycles; // was cpu_cycles
extern DWORD nSysCyclesPerSecond,new_n_cpu_cycles_per_second;
extern DWORD n_millions_cycles_per_sec;
extern "C" double cpu_cycles_multiplier; // used by 6301
extern const int EIGHT_MILLION; //8000000*TICKS8


///////////
// Video //
///////////

extern short TimerBLinecycle;
extern BYTE VideoFreqIdx;
extern int video_first_draw_line;
extern int video_last_draw_line;
extern BYTE screen_res;
extern short scan_y;
extern BYTE video_mixed_output;
extern BYTE ScreenResAtStartOfVbl;
extern BYTE VideoFreqAtStartOfVbl;
extern COUNTER_VAR sys_time_of_last_vbl,shifter_cycle_base;
extern COUNTER_VAR TimeOfHSyncOff;
extern MEM_ADDRESS VCountAtHSync;
// for Shifter trick analysis
extern COUNTER_VAR GlueFreqChangeTime[NSHIFTERCHANGES];
extern BYTE shifter_freq_change[NSHIFTERCHANGES];
extern BYTE GlueFreqChangeIdx;
extern COUNTER_VAR ShifterModeChangeTime[NSHIFTERCHANGES];
extern BYTE shifter_mode_change[NSHIFTERCHANGES];
extern BYTE ShifterModeChangeIdx;
extern bool freq_change_this_scanline;
#if defined(SSE_OPTION_FASTLINEA)
extern bool lineA;
#endif
#if defined(SSE_VID_D3D_VSYNC)
extern WORD MasterSync;
#else
#define MasterSync Glue.VideoFreq
#endif


#define CYCLES_FROM_HBL_TO_LEFT_BORDER_OPEN ((84)*TICKS8) //SS 84 = 56+28

#define CALC_CYCLES_FROM_HBL_TO_TIMER_B(freq) \
  switch (freq){ \
    case MONO_HZ: TimerBLinecycle=192*TICKS8;break; \
    case 60: TimerBLinecycle\
      =(CYCLES_FROM_HBL_TO_LEFT_BORDER_OPEN+320*TICKS8-4*TICKS8);break; \
    default: TimerBLinecycle\
      =(CYCLES_FROM_HBL_TO_LEFT_BORDER_OPEN+320*TICKS8); \
}

extern const BYTE TopScanlines[4];
extern const WORD CyclesPerScanline8MHz[4];
extern int CyclesPerScanline[4];

/*  The overscan mask is an imitation of Hatari's BorderMask.
    Each trick has a dedicated bit, to set it we '|' it, to check it
    we '&' it. Each value is the previous one x2.
    Note max 32 bit = $80 000 000
*/
enum EOverscanMask {
 TRICK_LINE_PLUS_26=0x01, 
 TRICK_LINE_PLUS_2=0x02, 
 TRICK_HIRES_OVERSCAN=2, // used for software and hw overscan in monochrome
 TRICK_LINE_MINUS_106=0x04,
 TRICK_LINE_MINUS_2=0x08,
 TRICK_LINE_PLUS_44=0x10,
 TRICK_4BIT_SCROLL=0x20,
 TRICK_OVERSCAN_MED_RES=0x40,
 TRICK_BLACK_LINE=0x80,
 TRICK_TOP_OVERSCAN=0x100,
 TRICK_BOTTOM_OVERSCAN=0x200,
 TRICK_BOTTOM_OVERSCAN_60HZ=0x400,
 TRICK_LINE_PLUS_20=0x800,
 TRICK_0BYTE_LINE=0x1000,
 TRICK_STABILISER=0x2000,
 TRICK_80BYTE_LINE=0x4000, // don't assume a "no trick" colour line = 160byte
 TRICK_UNSTABLE=0x08000,
 TRICK_LINE_PLUS_24=0x10000,
 TRICK_LINE_PLUS_4=0x20000,
 TRICK_LINE_PLUS_6=0x40000,
 TRICK_NEO=0x80000//tests
};



//////////////////////
// Extended monitor //
//////////////////////

#ifndef NO_CRAZY_MONITOR
void extended_monitor_hack();
extern long save_r[16];
extern MEM_ADDRESS line_a_base;
extern MEM_ADDRESS vdi_intout,vdi_ptsout;
extern MEM_ADDRESS em_rte_return_address;
extern DWORD em_width,em_height;
extern BYTE em_planes;
extern BYTE extended_monitor;
#define EXTMON_RESOLUTIONS (7+2+2)
extern UINT extmon_res[EXTMON_RESOLUTIONS][3];
void call_a000();
#endif


////////////////
// Emu detect //
////////////////

void emudetect_reset();
extern bool emudetect_called,emudetect_write_logs_to_printer,
  emudetect_overscans_fixed;


////////////
// Falcon //
////////////

#if !defined(SSE_NO_FALCONMODE) // we don't want that in Steem SSE
void emudetect_init();
void ASMCALL emudetect_falcon_draw_scanline(int,int,int,int);
void emudetect_falcon_palette_convert(int);
#define EMUD_FALC_MODE_OFF 0
#define EMUD_FALC_MODE_8BIT 1
#define EMUD_FALC_MODE_16BIT 2
extern BYTE emudetect_falcon_mode;
extern BYTE emudetect_falcon_mode_size;
extern bool emudetect_falcon_extra_height;
extern DynamicArray<DWORD> emudetect_falcon_stpal;
extern DynamicArray<DWORD> emudetect_falcon_pcpal;
#endif


//////////
// Disc //
//////////

#if defined(SSE_DISK_CAPS)
extern TCaps Caps;
#endif

#if defined(SSE_DISK_GHOST)
extern TGhostDisk GhostDisk[2];
#endif

extern TImageSTW ImageSTW[3];
#if defined(SSE_DISK_STW)
#define IMAGE_STW (FloppyDrive[DRIVE].ImageType.Extension==EXT_STW)
#else
#define IMAGE_STW 0
#endif

extern TImageSCP ImageSCP[3];
#if defined(SSE_DISK_SCP)
#define IMAGE_SCP (FloppyDrive[DRIVE].ImageType.Extension==EXT_SCP)
#else
#define IMAGE_SCP 0
#endif
#define IMAGE_LL_MFM IMAGE_SCP // for clarification

extern TImageHFE ImageHFE[3];
#if defined(SSE_DISK_HFE)
#define IMAGE_HFE (FloppyDrive[DRIVE].ImageType.Extension==EXT_HFE)
#else
#define IMAGE_HFE 0
#endif

#if defined(SSE_ACSI)
extern TAcsiHdc AcsiHdc[MAX_ACSI_DEVICES];
#endif

#define DRIVE                      (Psg.SelectedDrive)
#define CURRENT_SIDE               (Psg.SelectedSide)
#define CURRENT_TRACK              (FloppyDrive[DRIVE].track)

extern bool fdc_irq,hdc_irq;
void update_disk_irq();


/*#ifdef SSE_DEBUG
#define BUFFER_LENGTH (10+4+2+3) //4bytes for ext, 2bytes for command
#elif defined(SSE_OSD_DEBUGINFO)
#define BUFFER_LENGTH (10+4+3+3) //4bytes for ext, 3bytes for command
#else
#define BUFFER_LENGTH (10+2) //"D:S-TR-SEC" drive-side-track-sector
#endif*/

struct TDiskEmu {
  void TraceRegs();
  void TraceStatus(BYTE MyStr);
  void Update(int op=0); // 1=irq 2=change disk
  char sTrackinfo[16]; // D:S-TT-SS
  bool WritingToDisk();
  BYTE track,maxtrack;
  BYTE LastManager; // updated at command
  bool AcsiBsy; // meta
  bool VBLSoundcheck; // for check command at VBL for Pasti/Caps
  BYTE InterTrack; // sound: distance from target track in a SEEK
  bool direction; // for step/seek sound
  bool StatusBar;
};

extern TDiskEmu DiskEmu;


///////////
// Misc. //
///////////

extern int ioaccess;
extern int interrupt_depth;
extern int on_rte;
extern int on_rte_interrupt_depth;
extern MEM_ADDRESS old_pc;
extern MEM_ADDRESS pc_high_byte;


///////////
// Stats //
///////////

#if defined(SSE_STATS)

struct TStatsStatic { // not reset
  TStatsStatic() { 
    ZeroMemory(this,sizeof(TStatsStatic));
  }
  int TotalRuntime;
  int nReset;
  int nBlitError; // ;)
  int nSteemCrash; // ;)
  int nScrollers; // poor player ;)
  int nPatchedBytes; // reset on reset
  BYTE maxSide[2],maxTrack[2],maxSector[2]; // reset on eject
};

struct TStats { 
  // used to record and display information about what the program is doing
  enum EStats {
    IKBD_22=BIT_0,EMU_DETECT=BIT_1,WRITE_DISK=BIT_2,READ_MFP_TIMER=BIT_3,
    READ_MFP_TIMERB=BIT_4}; 
  // FUNCTIONS
  TStats() {
    Restore();
  }
  void Restore() {
    ZeroMemory(this,sizeof(TStats)); // simple
  }
  void Report();
  // DATA
#if defined(SSE_STATS_CPU)
  CpuUsage myCpuUsage; // Windows 2000 dependency
#endif
  COUNTER_VAR nFrame,nTimerb,nSlowdown,tEmuT;
  COUNTER_VAR nException[12],nStop;
  COUNTER_VAR nHbi,nVbi,nMfpIrq[16],nTrap[16],nSpurious;
  COUNTER_VAR nBlit,nBlitT,nBlith,nExtendedPal,nExtendedPalT,nHscroll,nVscroll;
  COUNTER_VAR nScreensplit,nVbase;
  COUNTER_VAR nReadvc;
  COUNTER_VAR nPsgSound,nMicrowire,nMicrowireT;
  COUNTER_VAR nKeyIn,nJoy0,nJoy1,nClick0,nClick1,nMousex,nMousey;
  COUNTER_VAR nHdsector,nPrg;
  COUNTER_VAR nGemdos,nBios,nXbios,nVdi,nAes;
  COUNTER_VAR nGemdosi,nBiosi,nXbiosi,nVdii;
  COUNTER_VAR nPorti[3],nPorto[3],nMfpTimeout[4];
  COUNTER_VAR nSectorR[2],nSectorW[2];
  COUNTER_VAR nFastBlit;
  DWORD nPal,nTimerbtick,nBlit1,nHbi1,nReadvc1,nScreensplit1; // per frame
  DWORD nLinePlus16; 
  DWORD mskSpecial,mskDigitalSound,mskOverscan,mskOverscan1;
  DWORD fTimer[4];
  DWORD tLock,tEmu,tEmu0,tEmuN,tUnlock,tBlit; // 1st just last frame
  int run_time;
  WORD nFps,tCpuUsage;
  WORD boot_checksum[2][2];
};

extern TStats Stats;
extern TStatsStatic StatsStatic;
extern EasyStr sStatsFile;

#endif


//////////
// Mega //
//////////

struct TRp5c15 { // RTC of the Mega ST (+STE?)
  BYTE reg[2][16]; // 2 banks, really 4bit each (BCD nibbles)
  WORD Read(MEM_ADDRESS addr);
  void Write(MEM_ADDRESS addr, BYTE io_src_b);
};

extern TRp5c15 MegaRtc;


#if defined(SSE_MEGASTE)

struct TCpu16 {
  TCpu16();
  ~TCpu16();
  void Ready(BOOL enabling);
  BOOL IsFast(MEM_ADDRESS ad);
  void Add(MEM_ADDRESS ad);
  void Reset();
  // DATA
  MEM_ADDRESS *pAdlist;
  bool *pIsCached; // big table
  int iPos;
/*
-------+-----+-----------------------------------------------------+----------
##############Mega STe Cache/Processor Control                     ###########
-------+-----+-----------------------------------------------------+----------
$FF8E21|byte |Mega STe Cache/Processor Control           BIT 15-1 0|R/W (MSTe)
       |     |Cache enable lines (set all to 1 to enable) -----'  ||
       |     |CPU Speed (0 - 8mhz, 1 - 16mhz) --------------------'|
-------+-----+-----------------------------------------------------+----------
*/
  BYTE ScuReg;
};


struct TMegaSte {
//  TCpu16 Cpu16;
  // For now we regroup other Mega STE specific hardware registers in one struct
  BYTE FdHd; // $ffff860e.w (0 = 8MHz = DD/ 3 = 16MHz = HD)
/*
-------+-----+-----------------------------------------------------+----------
##############Zilog 8530 SCC (MSTe/TT/F030)                        ###########
-------+-----+-----------------------------------------------------+----------
$FF8C81|byte |Channel A - Control Register                         |R/W  (SCC)
$FF8C83|byte |Channel A - Data Register                            |R/W  (SCC)
$FF8C85|byte |Channel B - Control Register                         |R/W  (SCC)
$FF8C87|byte |Channel B - Data Register                            |R/W  (SCC)
*/
  BYTE Scc[4]; //TODO
/*
-------+-----+-----------------------------------------------------+----------
##############VME Bus System Control Unit (MSTe/TT)                ###########
-------+-----+-----------------------------------------------------+----------
$FF8E01|byte |VME sys_mask                      BIT 7 6 5 4 . 2 1 .|R/W  (VME)
$FF8E03|byte |VME sys_stat                      BIT 7 6 5 4 . 2 1 .|R    (VME)
       |     |_SYSFAIL in VMEBUS -------------------' | | |   | |  |program
       |     |MFP ------------------------------------' | |   | |  |autovec
       |     |SCC --------------------------------------' |   | |  |autovec
       |     |VSYNC --------------------------------------'   | |  |program
       |     |HSYNC ------------------------------------------' |  |program
       |     |System software INT ------------------------------'  |program
       |     +-----------------------------------------------------+
       |     |Reading sys_mask resets pending int-bits in sys_stat,|
       |     |so read sys_stat first.                              |
-------+-----+-----------------------------------------------------+----------
$FF8E05|byte |VME sys_int                                     BIT 0|R/W  (VME)
       |     |Setting bit 0 to 1 forces an INT of level 1. INT must|Vector $64
       |     |be enabled in sys_mask to use it.                    |
-------+-----+-----------------------------------------------------+----------
$FF8E0D|byte |VME vme_mask                      BIT 7 6 5 4 3 2 1 .|R/W  (VME)
$FF8E0F|byte |VME vme_stat                      BIT 7 6 5 4 3 2 1 .|R    (VME)
       |     |_IRQ7 from VMEBUS --------------------' | | | | | |  |program
       |     |_IRQ6 from VMEBUS/MFP ------------------' | | | | |  |program
       |     |_IRQ5 from VMEBUS/SCC --------------------' | | | |  |program
       |     |_IRQ4 from VMEBUS --------------------------' | | |  |program
       |     |_IRQ3 from VMEBUS/soft -----------------------' | |  |prog/autov
       |     |_IRQ2 from VMEBUS ------------------------------' |  |program
       |     |_IRQ1 from VMEBUS --------------------------------'  |program
       |     +-----------------------------------------------------+
       |     |MFP-int and SCC-int are hardwired to the VME-BUS-ints|
       |     |(or'ed). Reading vme_mask resets pending int-bits in |
       |     |vme_stat, so read vme_stat first.                    |
*/
  BYTE VmeSysMask; //TODO
  BYTE VmeSysStat;
  BYTE VmeSysInt;
  BYTE VmeMask;
  BYTE VmeStat;

/*
-------+-----+-----------------------------------------------------+----------
##############Floating Point Coprocessor (CIR Interface in MSTe)   ###########
-------+-----+-----------------------------------------------------+----------
$FFFA40|word |FP_Stat    Response-Register                         |??? (MSTe)
$FFFA42|word |FP_Ctl     Control-Register                          |??? (MSTe)
$FFFA44|word |FP_Save    Save-Register                             |??? (MSTe)
$FFFA46|word |FP_Restor  Restore-Register                          |??? (MSTe)
$FFFA48|word |                                                     |??? (MSTe)
$FFFA4A|word |FP_Cmd     Command-Register                          |??? (MSTe)
$FFFA4E|word |FP_Ccr     Condition-Code-Register                   |??? (MSTe)
$FFFA50|long |FP_Op      Operand-Register                          |??? (MSTe)
$FFFA54|word |FP_Selct   Register Select                           |??? (MSTe)
$FFFA58|long |FP_Iadr    Instruction Address                       |??? (MSTe)
$FFFA5C|long |           Operand Address                           |??? (MSTe)
*/
   //TODO
};

#endif//#if defined(SSE_MEGASTE)

#pragma pack(pop)

#endif// EMULATOR_DECLA_H
