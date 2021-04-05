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
FILE: shifter.h
DESCRIPTION: Declarations for the high-level Shifter emulation.
struct TShifter
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSESHIFTER_H
#define SSESHIFTER_H

#include "conditions.h"
#include "steemh.h"
#include "parameters.h"
#include "sound.h"


// in all shift modes, all 4 Shifter registers are loaded before picture starts:
#define SHIFTER_RASTER_PREFETCH_TIMING (16*TICKS8)
#define SHIFT_SDP(nbytes) shifter_draw_pointer+=(DWORD)(nbytes)

#define LORES   0
#define MEDRES  1
#define HIRES   2

#define OPTION_SHIFTER_WU (Shifter.WakeupShift)

// ID which part of emulation required video rendering
enum EShifterDispatchers { DISPATCHER_CPU,DISPATCHER_SET_SHIFT_MODE,
  DISPATCHER_WRITE_SDP,DISPATCHER_SET_PAL,DISPATCHER_DSTE, 
  DISPATCHER_LINEWIDTH,DISPATCHER_DEBUGGER,DISPATCHER_SET_SYNC};


extern "C" // used by assembly routines => no struct, don't change names
{
  extern WORD STpal[16]; // the full ST palette is here
  extern MEM_ADDRESS shifter_draw_pointer; // ST pointer to video RAM to be rendered
  extern BYTE shifter_hscroll; // STE-only horizontal scroll
}

#define HSCROLL shifter_hscroll

extern short shifter_tick8; // was shifter_pixel
extern short shifter_x,shifter_y;
extern BYTE shifter_sound_mode; // should be in struct but old memory snapshot issues

#pragma pack(push, 8)

struct TShifter {
  //FUNCTIONS
  TShifter();
#if !defined(SSE_DRAW_MARSHALL)
  inline void DrawBufferedScanlineToVideo();
#endif
  void DrawScanlineToEnd();
  void IncScanline();
  void Render(short cycles_since_hbl, int dispatcher);
  void Reset(bool Cold);
  void Restore();
  void RoundCycles(short &cycles_in);
  static void SetPal(int n, WORD NewPal); // static for STVL
  void sound_get_last_sample(WORD *pw1,WORD *pw2); // v402
  void sound_set_mode(BYTE new_mode);
  void sound_play();
  //DATA
  BYTE *ScanlineBuffer; // unused
  DWORD Scanline[230/4+2];
  int nVbl; // = FRAME for debugger
  BYTE Scanline2[112];
  BYTE HiresRaster; // for a hack
  BYTE HblStartingHscroll; // saving true hscroll in MED RES (no use)
  BYTE ShiftMode;
  BYTE Preload; // #words into Shifter's IR (gross approximation)
  char WakeupShift;
  char HblPixelShift; // for 4bit scrolling, other shifts
  WORD SoundFifo[4]; // ref. 4.0.2
  BYTE SoundFifoIdx;
};


#pragma pack(pop)


// just taking some unimportant code out of Render for clarity
#define   AUTO_BORDER_ADJUST  \
          if(!(border)) { \
            if(scanline_drawn_so_far<SideBorderSize) { \
              border1-=(SideBorderSize-scanline_drawn_so_far); \
              if(border1<0){ \
                picture+=border1; \
                if(screen_res==LORES) {  \
                  hscroll-=border1;  \
                  SHIFT_SDP((hscroll/16)*8); \
                  hscroll&=15; \
                }else if(screen_res==MEDRES) { \
                  hscroll-=border1*2;  \
                  SHIFT_SDP((hscroll/16)*4); \
                  hscroll&=15; \
                } \
                border1=0; \
                if(picture<0) picture=0; \
              } \
            } \
            int ta=(border1+picture+border2)-320; \
            if(ta>0) { \
              border2-=ta; \
              if(border2<0)  { \
                picture+=border2; \
                border2=0; \
                if (picture<0)  picture=0; \
              } \
            } \
            border1=border2=0; \
          }


////////////////////////////////////////////
// Digital Sound (GST Shifter of the STE) //
////////////////////////////////////////////


extern WORD SteSoundLastWord,SteSoundFreq;
// Max frequency/lowest refresh *2 for stereo
extern WORD *SteSoundChannelBuf;
extern DWORD SteSoundChannelBufLen, SteSoundChannelBufIdx;
extern int SteSoundOutputCountdown,SteSoundSamplesCountdown;

#endif//define SSESHIFTER_H
