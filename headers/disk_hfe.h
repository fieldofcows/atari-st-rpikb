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
FILE: disk_hfe.h
DESCRIPTION: Declarations for HFE disk image support.
struct  TImageHFE
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSEHFE_H
#define SSEHFE_H

#include <hfe/libhxcfe.h> //3rdparty
#include <hfe/hfe_format.h> //3rdparty
#include "floppy_drive.h"

#pragma pack(push, 8)

struct  TImageHFE:public TImageMfm {
  // interface
  bool Create(char *path);
  bool Open(char *path);
  void Close();
  bool LoadTrack(BYTE side,BYTE track,bool reload=false);
  WORD GetMfmData(WORD position); 
  void SetMfmData(WORD position, WORD mfm_data);
  // other functions
  TImageHFE();
  ~TImageHFE();
  int ComputeIndex();
  void Init();
  WORD MirrorMFM(WORD mfm_word);
  // variables
  WORD *TrackData;
  BYTE *ImageData;
  picfileformatheader *file_header;
  pictrack *track_header;
  int image_size;
};

#pragma pack(pop)

#endif//#ifndef SSEHFE_H
