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

DOMAIN: Disk image
FILE: disk_stw.h
DESCRIPTION: Declarations for STW disk image support.
struct TImageSTW
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSE_STW_H
#define SSE_STW_H

#include "floppy_drive.h"

#pragma pack(push, 8)

struct TImageSTW:public TImageMfm {
  // interface
  bool Create(char *path,BYTE density=1); //DD=1 HD=2
  bool Open(char *path);
  void Close();
  bool LoadTrack(BYTE side,BYTE track,bool reload=false);
  WORD GetMfmData(WORD position); 
  void SetMfmData(WORD position,WORD mfm_data);
  // other functions
  TImageSTW();
  ~TImageSTW();
  void Init();
  bool GetSectorData(BYTE side,BYTE track,BYTE sector,BYTE *pdata); // for utilities
  // variables
  WORD *TrackData;
  BYTE *ImageData;
  WORD Version;
  BYTE nSides,nTracks; // those are constants in STW v100
  WORD nTrackBytes;
};

#pragma pack(pop)

#if defined(SSE_DISK_AUTOSTW)
// conversion functions
bool STtoSTW(BYTE id,char *dst_path); // ST/MSA/DIM -> STW
bool STWtoST(BYTE id); // STW -> ST/MSA/DIM
#endif

#endif//#ifndef SSE_STW_H
