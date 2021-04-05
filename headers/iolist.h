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
FILE: iolist.h
DESCRIPTION: Declarations for the Debugger's IO list, including the
Control Mask Browser (fake IO used for Debugger control).
struct Tiolist_entry
---------------------------------------------------------------------------*/

#pragma once
#ifndef IOLIST_DECLA_H
#define IOLIST_DECLA_H

#ifdef DEBUG_BUILD

#include <conditions.h>
#include <easystr.h>
#ifdef MINGW_BUILD
#define NULL 0
#endif


#define IOLIST_PSEUDO_AD 0x53000000
#define IOLIST_PSEUDO_AD_PSG (IOLIST_PSEUDO_AD+0x1000)
#define IOLIST_PSEUDO_AD_FDC (IOLIST_PSEUDO_AD+0x2000)
#define IOLIST_PSEUDO_AD_IKBD (IOLIST_PSEUDO_AD+0x3000)
#define IOLIST_PSEUDO_AD_6301 (IOLIST_PSEUDO_AD+0x4000)
#define IS_IOLIST_PSEUDO_ADDRESS(x) ((x&0xff000000)==IOLIST_PSEUDO_AD)


void iolist_add_entry(MEM_ADDRESS ad,char*name,int bytes,char*bitmask=NULL,
  BYTE*ptr=NULL);

#pragma pack(push, 8)

struct Tiolist_entry {
  MEM_ADDRESS ad;
  EasyStr name;
  int bytes;
  EasyStr bitmask;
  BYTE*ptr;
};

#pragma pack(pop)

void iolist_init();
Tiolist_entry*search_iolist(MEM_ADDRESS);
int iolist_box_draw(HDC,int,int,int,int,Tiolist_entry*,BYTE*);
void iolist_box_click(int,Tiolist_entry*,BYTE*); //bit number clicked, toggle bit
int iolist_box_width(Tiolist_entry*);

#endif//#ifdef DEBUG_BUILD

#endif//IOLIST_DECLA_H
