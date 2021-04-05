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

DOMAIN: OS
FILE: tos.h
DESCRIPTION: Declarations for TOS (The Operating System) utilities
struct TTos
---------------------------------------------------------------------------*/

#pragma once
#ifndef TOS_DECLA_H
#define TOS_DECLA_H

#include <dirsearch.h>
#include <easystr.h>

extern BYTE *STRom;
#ifndef BIG_ENDIAN_PROCESSOR
extern BYTE *Rom_End,*Rom_End_minus_1,*Rom_End_minus_2,*Rom_End_minus_4;
#endif
extern BYTE *cart,*cart_save;
#ifndef BIG_ENDIAN_PROCESSOR
extern BYTE *Cart_End_minus_1,*Cart_End_minus_2,*Cart_End_minus_4;
#endif
extern unsigned long tos_len;
extern MEM_ADDRESS rom_addr,rom_addr_end;
extern MEM_ADDRESS os_gemdos_vector,os_bios_vector,os_xbios_vector;
extern WORD tos_version;
extern bool tos_high;
extern int aes_calls_since_reset; // unused but saved in snapshot


#define ON_RTE_NONE 0
#define ON_RTE_STEMDOS 1
#define ON_RTE_LINEA_HACK 2
#define ON_RTE_EMHACK 3
#define ON_RTE_DONE_MALLOC_FOR_EM 4
#define ON_RTE_MOUSE 5
#define ON_RTE_STOP 400
#define ON_RTS_STOP 401

#define SV_PHYSTOP  0x42e
#define SV_DRVBITS  0x4c2

#define TRAP_GEMDOS  1
#define TRAP_GEM     2
#define TRAP_BIOS   13
#define TRAP_XBIOS  14


#pragma pack(push, 8)

struct TTos {
  enum ETos {                      //Table 4-3.  BDOS Error Codes
    NoError=0,                       // E_OK
    //InvalidFunction=-32,           // EINVFN
    FileNotFound=-33,              // EFILNF
    PathNotFound=-34,              // EPTHNF
    //NoHandlesLeft=-35,             // ENHNDL
    AccessDenied=-36,              // EACCDN
    //InvalidHhandle=-37,            // EIHNDL
    InsufficientMemory=-39,        // ENSMEM
    //InvalidMemoryBlockAddress=-40, // EIMBA
    InvalidDrive=-46,              // EDRIVE
    NoMoreFiles=-49,               // ENMFIL
    RangeError=-64,                // ERANGE (can't use this, defined in WinError.h !)
    InternalError=-65,             // EINTRN
    InvalidProgramLoadFormat=-66,  // EPLFMT
    //SetblockFailure=-67            // EGSBF (due to growth restrictions)  
  };
  void CheckKeyboardClick();
  EasyStr GetNextTos(DirSearch &ds); // to enumerate TOS files
  void GetTosProperties(EasyStr Path,WORD &Ver,BYTE &Country,WORD &Date,
                        BYTE &Recognised);
  void HackMemoryForExtendedMonitor();
  WORD MouseX,MouseY; // hack based on vq_mouse interception
  //MEM_ADDRESS MouseAd; // hack based on stock address
#if defined(SSE_GUI_CONFIG_RELATIVE)
  void UpdateTOSPath(EasyStr *path);
#endif
};

#pragma pack(pop)

MEM_ADDRESS GetSPBeforeTrap(bool* pInvalid=NULL);
void intercept_os();
bool load_cart(char *filename); // true: succeeded (402R10)
bool load_TOS(char *File);  // true: succeeded
MEM_ADDRESS get_TOS_address(char *File);
void GetTOSKeyTableAddresses(MEM_ADDRESS *lpUnshiftTable,
                             MEM_ADDRESS *lpShiftTable);

#endif//#ifndef TOS_DECLA_H
