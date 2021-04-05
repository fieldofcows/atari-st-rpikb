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
FILE: interface_caps.h
DESCRIPTION: Declarations for CTR/IPF disk image support.
struct TCaps
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSECAPSIMG_H
#define SSECAPSIMG_H
#include "SSE.h"

/* Support for IPF file format using the fdc emulator included in 
   CAPSimg.dll (Caps library).

   All calls to the DLL functions are encapsulated here, but there are some
   direct access to CapsFdc variables.
   DMA transfers aren't handled by this emulator, contrary to Pasti, so we 
   use the existing Steem system, improved.
*/

#if defined(SSE_DISK_CAPS)

#ifdef WIN32
#include <caps/CommonTypes.h>
#include <caps/CapsAPI.h>
#include <caps/CapsFDC.h>
#include <caps/CapsLib.h>
#define CAPS_CALLCONV __cdecl
#endif

#ifdef UNIX
#define __cdecl
#include  <caps_linux/capsimg_source_linux_macosx/Core/CommonTypes.h>
#include <caps_linux/capsimg_source_linux_macosx/LibIPF/CapsAPI.h>
#include <caps_linux/capsimg_source_linux_macosx/LibIPF/CapsFDC.h>
#include <caps_linux/capsimg_source_linux_macosx/LibIPF/CapsLib.h>
#undef __cdecl
#define CAPS_CALLCONV
#endif


#pragma pack(push, 8)



// our interface with CAPSimg.dll, C++ style (RAII)
struct TCaps {  
  // FUNCTIONS
  TCaps();
  ~TCaps();
  int Init();
  void Reset();
  void Hbl(); // run the emulator for the HBL (512 cycles at 50hz)
  int InsertDisk(int drive, char *File,CapsImageInfo *img_info);
  void RemoveDisk(int drive);
  UDWORD ReadWD1772(BYTE Line);
  void WritePsgA(int data);
  void WriteWD1772(BYTE Line,BYTE data);

/*  Using static functions so that there's no 'this' for those
    callback functions.

    1) DRQ (data request)
    If we're reading from the disk, all data bytes combined by the controller
    come here, one by one.
    If we're writing to the disk, everytime the controller is ready to
    translate a byte, this function is called to put a byte in the data
    register.
    The caps library doesn't emulate writes yet, but Steem should be
    ready for it.
*/
  static void CAPS_CALLCONV CallbackDRQ(PCAPSFDC pc, UDWORD setting);


/*  2) IRQ (interrupt request)
    The assigned IRQ (7) is generally disabled on the ST, but the corresponding
    bit (5) in the  MFP GPIP register (IO address $FFFA01) is polled instead.
*/  
  static void CAPS_CALLCONV CallbackIRQ(PCAPSFDC pc, UDWORD lineout);


/*  3) Track
    Strangely it's our job to change track and update all variables,
    maybe because there are different ways (memory, track by track...)?
*/
  static void CAPS_CALLCONV CallbackTRK(PCAPSFDC pc, UDWORD driveact);

  // DATA
#if defined(SSE_DISK_CAPS_MEMORY)
  BYTE *pImage[2];
#endif
  BOOL Active; // if there's an IPF disk in some drive, we must run IPF cycles
  int Version; // 0: failed; else release revision eg 42
  // for drive A & B
  SDWORD ContainerID[2]; 
  SDWORD LockedSide[2];
  SDWORD LockedTrack[2]; 
  CapsDrive Drive[2]; // 2 double-sided floppy drives
  CapsFdc fdc; // 1 cheap controller
};

#pragma pack(pop)

#endif//#if defined(SSE_DISK_CAPS)

#endif//SSECAPSIMG_H

