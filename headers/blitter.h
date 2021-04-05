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
FILE: blitter.h
DESCRIPTION: Declarations for the high-level blitter emulation.
struct TBlitter
---------------------------------------------------------------------------*/

#pragma once
#ifndef BLITTER_DECLA_H
#define BLITTER_DECLA_H

#include <steemh.h>
#include <data_union.h>

#pragma pack(push, 8)

struct TBlitter { 
  enum EBlitter {PRIME,READ_SOURCE,READ_DEST,WRITE_DEST};
  // FUNCTIONS
  inline void check_blitter_start();
  // DATA
  DU32 SrcAdr,DestAdr;
  WORD YCount,dummy1; // was DWORD
  DWORD SrcBuffer;
  int XCounter,YCounter; //internal counters
  COUNTER_VAR TimeToSwapBus;
  COUNTER_VAR TimeAtBlit;
  COUNTER_VAR BlitCycles; // a bit hacky
  WORD HalfToneRAM[16];
  WORD EndMask[3];
  WORD XCount;
  WORD SrcDat,DestDat,NewDat;  //internal data registers
  WORD Mask;   //internal mask register
  short SrcXInc,SrcYInc,DestXInc,DestYInc;
  BYTE Hop,Op,Skew;
  BYTE BlittingPhase; // to follow 'state-machine' phase R-W...
  BYTE Smudge,Hog,NFSR,FXSR,Busy,Last,HasBus,NeedDestRead;
  BYTE dummy2; // was SelfRestarted
  BYTE Request; //0-3
  BYTE BusAccessCounter; // count accesses by blitter and by CPU in blit mode
  BYTE LineNumber; //4bit counter
  // v402
  int nWordsToBlit,nWordsBlitted; // debug
  BYTE rBusy; // register different from busy line
  bool LineStarted; // persistent flag for more reliable test
};

#pragma pack(pop)

#if !defined(SSE_410R3)
void Blitter_Draw();
#endif
void Blitter_CheckRequest();

inline void TBlitter::check_blitter_start() {
  if(Request)
    Blitter_CheckRequest();
}

#endif//BLITTER_DECLA_H
