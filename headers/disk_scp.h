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
FILE: disk_scp.h
DESCRIPTION: Declarations for SCP disk image support.
struct TSCP_file_header, TSCP_TDH_TABLESTART, TSCP_track_header, TImageSCP
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSESCP_H
#define SSESCP_H

#include "floppy_drive.h"

/*
; TRACK DATA = 16 BIT VALUE, TIME IN NANOSECONDS/25ns FOR ONE BIT CELL TIME
;
; i.e. 0x00DA = 218, 218*25 = 5450ns = 5.450us
*/

#define SCP_DATA_WINDOW_TOLERANCY 20 

#pragma pack(push, 8)

struct TSCP_file_header {
  char IFF_ID[3]; //"SCP" (ASCII CHARS)
  BYTE IFF_VER; //version (nibbles major/minor)
  BYTE IFF_DISKTYPE; //disk type (0=CBM, 1=AMIGA, 2=APPLE II, 3=ATARI ST, 4=ATARI 800, 5=MAC 800, 6=360K/720K, 7=1.44MB)
  BYTE IFF_NUMREVS; //number of revolutions (2=default)
  BYTE IFF_START; //start track (0-165)
  BYTE IFF_END; //end track (0-165)
  BYTE IFF_FLAGS; //FLAGS bits (0=INDEX, 1=TPI, 2=RPM, 3=TYPE - see defines below)
  BYTE IFF_ENCODING; //BIT CELL ENCODING (0=16 BITS, >0=NUMBER OF BITS USED)
  BYTE IFF_HEADS; //0=both heads are in image, 1=side 0 only, 2=side 1 only
  //BYTE IFF_RSRVED; //reserved space
  BYTE IFF_RESOLUTION; // 0=25ns, 1=50, 2=75, 3=100, 4=125, etc.
  DWORD IFF_CHECKSUM; //32 bit checksum of data added together starting at 0x0010 through EOF
  DWORD IFF_THDOFFSET[166]; // track data header offset
};


struct TSCP_TDH_TABLESTART {
  DWORD TDH_DURATION; //duration of track, from index pulse to index pulse (1st revolution)
  DWORD TDH_LENGTH; //length of track (1st revolution)
  DWORD TDH_OFFSET;
};


struct TSCP_track_header {
  char TDH_ID[3]; //"TRK" (ASCII CHARS)
  BYTE TDH_TRACKNUM; //track number
  TSCP_TDH_TABLESTART TDH_TABLESTART[5]; //table of entries (3 longwords per revolution stored)
  DWORD track_data_checksum; //? see hxc project
};


struct  TImageSCP:public TImageMfm {
  // interface
  bool Open(char *path);
  void Close();
  bool LoadTrack(BYTE side,BYTE track,bool reload=false);
  WORD GetMfmData(WORD position); 
  void SetMfmData(WORD position, WORD mfm_data);
  int GetNextTransition(WORD& us_to_next_flux);
  // other functions
  TImageSCP();
  ~TImageSCP();
  void ComputePosition();
  int UnitsToNextFlux(DWORD position);
  WORD UsToNextFlux(int units_to_next_flux);
  void IncPosition();
  void Init();
  // variables
  DWORD *TimeFromIndexPulse; // from IP
  DWORD nBits;
  WORD nBytes; //not really pertinent (TODO?)
  TSCP_file_header file_header;
  TSCP_track_header track_header;
  BYTE rev;
};

#pragma pack(pop)

#endif//SSESCP_H
