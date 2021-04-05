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
FILE: disk_ghost.h
DESCRIPTION: Declarations for Steem's disk ghost feature.
struct TGhostDisk
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSE_GHOSTDISK_H
#define SSE_GHOSTDISK_H

/*
    Version 1.0
    Version 1.0 of GhostDisk was written to manage STG files in v1.0 format.

Interface:

    The object reads or writes one sector in one go.
 
    The memory buffer is handled by the object, with direct access: 
    BYTE* SectorData.

    The object can manage max. one image at a time. 

    Open(char *path) 

    Opens the file specified in path.
    If the file didn't exist, it creates it.
    Returns false on failure (not a STG file).

    ReadSector(TWD1772IDField *IDField)

    If the image doesn't hold the sector, returns 0.
    If the image holds the sector, copies it in data buffer (SectorData),
    and returns #bytes read (should be 512).

    WriteSector(TWD1772IDField *IDField) 

    Adds a sector to the image or rewrite an existing sector.
    Structures to fill in are defined below.
    The data to write is in SectorData.

    Close()

    Closes current file. 
    This function is called by Open() and at object destruction, 
    just in case.   
    Allocated memory is freed.
*/

#if defined(SSE_DISK_GHOST)

#include "fdc.h"

#pragma pack(push, 8)

struct TGhostDisk {
  //DATA
  BYTE *SectorData; // memory allocation is handled by object
  FILE *fCurrentImage;
  WORD Version;  // version of the STG file (we write $100)
  WORD nRecords;
  WORD SectorBytes;
  TWD1772IDField CurrentIDField;
  BYTE Id;
  // interface
  bool Open(char *path);
  void Close();
  WORD ReadSector(TWD1772IDField *IDField);
  void WriteSector(TWD1772IDField *IDField); 
  bool CheckCommand(BYTE io_src_b);
  // other functions
  TGhostDisk() {
    Init();
  };
  ~TGhostDisk() {
    Close();
  }
  void Init();
  void Reset();
  bool FindIDField(TWD1772IDField *IDField);
};

#pragma pack(pop)

#endif//#if defined(SSE_DISK_GHOST)

#endif//#ifndef SSE_GHOSTDISK_H
