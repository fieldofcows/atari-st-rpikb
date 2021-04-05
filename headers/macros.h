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
FILE: macros.h
DESCRIPTION: Declarations for Steem's macro system, to record, replay, 
load and save user input.
struct TMacroVblInfo, TMacroFileOptions
---------------------------------------------------------------------------*/

#pragma once
#ifndef MACROS_DECLA_H
#define MACROS_DECLA_H

#include <easystr.h>
#include <dynamicarray.h>


void macro_advance(int StartCode=0);
void macro_end(int EndCode);

void macro_record_joy();
void macro_record_mouse(int x_change,int y_change);
void macro_record_key(BYTE STCode);

bool macro_play_start();
void macro_play_joy();
void macro_play_mouse(int &x_change,int &y_change);
void macro_play_keys();

extern int macro_record ,macro_play ,macro_play_until;
extern bool macro_play_has_mouse ,macro_play_has_keys,macro_play_has_joys;
extern int macro_start_after_ikbd_read_count;
extern int macro_play_max_mouse_speed;
extern DWORD macro_jagpad[2];

#define MACRO_LEFT_INIT_MOVE (shifter_x*2)
#define MACRO_UP_INIT_MOVE (shifter_y*2)
#define MACRO_STARTRECORD 1
#define MACRO_STARTPLAY 2

#define MACRO_ENDRECORD 1
#define MACRO_ENDPLAY 2


#pragma pack(push, 8)

struct TMacroVblInfo {
  int xdiff,ydiff;
  BYTE stick[8],keycode[32];
  DWORD keys;
  DWORD jagpad[2];
};

struct TMacroFileOptions {
  int add_mouse_together,allow_same_vbls,max_mouse_speed;
};

#pragma pack(pop)

#define MACRO_FILE_GET 0
#define MACRO_FILE_SET 1

bool macro_mvi_blank(TMacroVblInfo *mvi);
void macro_file_options(int GetSet,char *Path,TMacroFileOptions *lpMFO,
                        FILE *f=NULL);

extern Str macro_play_file,macro_record_file;
extern DynamicArray<TMacroVblInfo> macro_record_store,macro_play_store;
extern TMacroVblInfo *mrsc,*mpsc;


#endif//#ifndef MACROS_DECLA_H
