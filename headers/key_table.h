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

DOMAIN: GUI
FILE: key_table.h
DESCRIPTION: Declarations for keyboard mapping.
---------------------------------------------------------------------------*/

#pragma once
#ifndef KEYTABLE_DECLA_H
#define KEYTABLE_DECLA_H

#include <conditions.h>
#include <dynamicarray.h>

extern BYTE key_table[256];

#ifndef STEEMKEYTEST
extern DWORD AltKeys_French[8];
extern DWORD AltKeys_German[7];
extern DWORD AltKeys_Spanish[8];
extern DWORD AltKeys_Italian[8];
extern DWORD AltKeys_Swedish[9];
extern DWORD AltKeys_Swiss[10];
                         
extern LANGID KeyboardLangID;

void GetTOSKeyTableAddresses(MEM_ADDRESS *lpUnshiftTable,
                             MEM_ADDRESS *lpShiftTable);

void GetAvailablePressChars(DynamicArray<DWORD> *lpChars);
#endif//#ifndef STEEMKEYTEST

#define LANG_CUSTOM 92 // number is arbitrary
extern EasyStr KeyboardMappingPath;

extern WORD *shift_key_table[4];

extern bool EnableShiftSwitching;
// SetSTKeys: direct mapping without modifiers
void SetSTKeys(char *Letters,int Val1,...);
void SetSTKeys(int Letter,...); // -1 to mark end of arguments

#ifdef WIN32
extern HKL keyboard_layout;
#endif

#ifdef UNIX
extern KeyCode Key_Pause,Key_Quit;
void SetSTKey(KeySym Sym,BYTE STCode,bool CanOverwrite=0);
#endif

void AddToShiftSwitchTable(int PCModifiers,int PCAscii,BYTE STModifiers,
                           BYTE STCode);

void DestroyKeyTable();
void InitKeyTable();

#ifdef UNIX
void UNIX_get_fake_VKs();
#endif

#endif//#ifndef KEYTABLE_DECLA_H
