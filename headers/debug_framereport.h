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

DOMAIN: Debug
FILE: debug_framereport.h
DESCRIPTION: Declarations for frame reports (debug tool).
struct SFrameEvent
class TFrameEvents
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSEFRAMEREPORT_H
#define SSEFRAMEREPORT_H

#if defined(SSE_DEBUGGER_FRAME_REPORT)

#include <run.h>

#include "debug.h"

/*  We use the new general fake io control masks, they are 16bit, but 
    only 8 bits should be used for the GUI.
    Higher bits also for the GUI.
*/

//mask 1
#define FRAME_REPORT_MASK1 (Debug.ControlMask[0])
#define FRAME_REPORT_MASK_SYNCMODE                 (1<<15)
#define FRAME_REPORT_MASK_SHIFTMODE                (1<<14)
#define FRAME_REPORT_MASK_PAL                      (1<<13)
#define FRAME_REPORT_MASK_VCOUNT                   (1<<12)
#define FRAME_REPORT_MASK_VC_LINES                 (1<<11)
#define FRAME_REPORT_MASK_HSCROLL                  (1<<10)
#define FRAME_REPORT_MASK_VIDEOBASE                (1<<9)

//mask 2
#define FRAME_REPORT_MASK2 (Debug.ControlMask[1])
#define FRAME_REPORT_MASK_INT                      (1<<15) // for hbi, vbi, mfp
#define FRAME_REPORT_MASK_BLITTER                  (1<<14)
#define FRAME_REPORT_MASK_SHIFTER_TRICKS           (1<<13)
#define FRAME_REPORT_MASK_SHIFTER_TRICKS_BYTES     (1<<12)
#define FRAME_REPORT_MASK_TIMER_B                  (1<<11)
#endif

//mask 3
#if defined(SSE_VID_STVL3)
#define FRAME_REPORT_MASK3 (Debug.ControlMask[15])
#define FRAME_REPORT_MASK_SYNC    (1<<15)
#define FRAME_REPORT_MASK_DE      (1<<14)
#define FRAME_REPORT_MASK_BLANK   (1<<13)
#define FRAME_REPORT_MASK_LD      (1<<12)

//mask 4
#define FRAME_REPORT_MASK4 (Debug.ControlMask[16])
#define FRAME_REPORT_MASK_PX (1<<15)
#define FRAME_REPORT_MASK_RL (1<<14)
#define FRAME_REPORT_MASK_SM (1<<13)
#define FRAME_REPORT_MASK_LIM (1<<12)


#pragma pack(push, 8)

struct SFrameEvent {
  short Scanline; 
  short Cycle;
  int Value;
  int Type;
  inline void Add(short scanline,short cycle, char type, int value); 
  inline void Add(short scanline,short cycle, char *type, int value); //overload
};


class TFrameEvents {
private:
  int m_nEvents; // how many video events occurred this vbl?
  int m_nReports;
public:
  enum EFrameEvents {MAX_EVENTS=313*128}; // how much is too high?
  int TriggerReport; // set 2 to ask a full report, then it's set FALSE again
  struct SFrameEvent m_FrameEvent[MAX_EVENTS]; // it's public
  TFrameEvents();
  void Add(short scanline, short cycle, char type, int value);
  void Add(short scanline, short cycle, char *type, int value);
  void Init();
  int Report();
  void ReportLine();
  int Vbl(); 
  int nVbl;
};

#pragma pack(pop)

extern TFrameEvents FrameEvents; // singleton

#if defined(SSE_VID_STVL3)
void FrameEvents_Add(int event_type,int event_value);
#endif

inline void SFrameEvent::Add(short scanline,short cycle, char type, int value) {
  Scanline=scanline;
  Cycle=cycle;
  Type=type;
  Value=value;
}

inline void SFrameEvent::Add(short scanline,short cycle, char *type, int value) { // overload
  Scanline=scanline;
  Cycle=cycle;
  Type=(type[0]<<8)+type[1];
  Value=value;
}


#endif//#if defined(SSE_DEBUGGER_FRAME_REPORT)

#endif//#ifndef SSEFRAMEREPORT_H
