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

DOMAIN: Various
FILE: acc.h
DESCRIPTION: Declarations for completely random accessory functions.
---------------------------------------------------------------------------*/

#pragma once
#ifndef ACC_DECLA_H
#define ACC_DECLA_H

#include "conditions.h"
#include <stdio.h>
#include <easystr.h>
#include <dynamicarray.h>
#include "debug.h"

// SWAP_BIG_ENDIAN: swap big endian data if we use a little endian processor (such as Intel, AMD)
// SWAP_LITTLE_ENDIAN: swap little endian data if we use a big endian processor (such as ...?)
#if defined(BIG_ENDIAN_PROCESSOR)
#define SWAP_BIG_ENDIAN_WORD(val)
#define SWAP_BIG_ENDIAN_DWORD(val)
//#define SWAP_LITTLE_ENDIAN_WORD(val) val=_byteswap_ushort(val) // intrinsics
//#define SWAP_LITTLE_ENDIAN_DWORD(val) val=_byteswap_ulong(val)
#define BYTESWAP16(n) (((n&0xFF00)>>8)|((n&0x00FF)<<8))
#define BYTESWAP32(n) ((BYTESWAP16((n&0xFFFF0000)>>16))|((BYTESWAP16(n&0x0000FFFF))<<16))
#define BYTESWAP64(n) ((BYTESWAP32((n&0xFFFFFFFF00000000)>>32))|((BYTESWAP32(n&0x00000000FFFFFFFF))<<32))
#define SWAP_LITTLE_ENDIAN_WORD(val) val=BYTESWAP16(val)
#define SWAP_LITTLE_ENDIAN_DWORD(val) val=BYTESWAP32(val)
#elif defined(SSE_VC_INTRINSICS) // Intel = little endian
#define SWAP_BIG_ENDIAN_WORD(val) val=_byteswap_ushort(val)
#define SWAP_BIG_ENDIAN_DWORD(val) val=_byteswap_ulong(val)
#define SWAP_LITTLE_ENDIAN_WORD(val)
#define SWAP_LITTLE_ENDIAN_DWORD(val)
#else // Intel = little endian
#if 1 // warning BCC, don't understand (acsi 117)
#define BYTESWAP16(n) ( (((n)&0xFF00)>>8) | (((n)&0x00FF)<<8) )
#else
#define BYTESWAP16(n) (((n&0xFF00)>>8)|((n&0x00FF)<<8))
#endif
#define BYTESWAP32(n) ((BYTESWAP16((n&0xFFFF0000)>>16))|((BYTESWAP16(n&0x0000FFFF))<<16))
#define BYTESWAP64(n) ((BYTESWAP32((n&0xFFFFFFFF00000000)>>32))|((BYTESWAP32(n&0x00000000FFFFFFFF))<<32))
#define SWAP_BIG_ENDIAN_WORD(val) val=BYTESWAP16(val)
#define SWAP_BIG_ENDIAN_DWORD(val) val=BYTESWAP32(val)
#define SWAP_LITTLE_ENDIAN_WORD(val)
#define SWAP_LITTLE_ENDIAN_DWORD(val)
#endif


EasyStr HEXSl(long n,int ln);
#ifdef _WIN64
EasyStr HEXSll(long long n,int ln);
#endif

#if defined(SSE_VC_INTRINSICS)
extern int count_bits_set_in_word1(unsigned short w);
#else
extern int count_bits_set_in_word(unsigned short w);
#endif

EasyStr ReadStringFromMemory(MEM_ADDRESS ad,int max_len);
MEM_ADDRESS WriteStringToMemory(MEM_ADDRESS ad,char*c);
void parse_search_string(Str OriginalText,DynamicArray<BYTE> &ByteList,
                         bool &WordOnly);
MEM_ADDRESS acc_find_bytes(DynamicArray<BYTE> &BytesToFind,bool WordOnly,
                           MEM_ADDRESS ad,int dir);
long colour_convert(int red,int green,int blue);


#ifdef WIN32
BOOL SetClipboardText(LPCTSTR pszText);
HMODULE SteemLoadLibrary(LPCSTR lpLibFileName);
int get_text_width(char*t);
#endif

#if !defined(SSE_NO_UPDATE)
EasyStr time_or_times(int n);
#endif

#endif//ACC_DECLA_H
