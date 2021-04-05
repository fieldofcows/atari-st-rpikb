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
FILE: mr_static.h
DESCRIPTION: Declaration of GUI elements used by the Debugger.
class mr_static
---------------------------------------------------------------------------*/

#pragma once
#ifndef MRSTATIC_DECLA_H
#define MRSTATIC_DECLA_H

#ifdef DEBUG_BUILD

#include "iolist.h"
#include "mem_browser.h"


LRESULT CALLBACK mr_static_WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
void mr_static_delete_children_of(HWND Win);

enum EMrStaticType{MST_MEMORY=0,MST_REGISTER,MST_HISTORIC_MEMORY,
MST_MEM_BROWSER_ADDRESS,MST_ADDRESS,MST_IOLIST,MST_HISTORIC_IOLIST,MST_DECIMAL};

class mr_static {
public:
  mr_static(char*label,char*name,int x,int y, HWND owner,HMENU id,
    MEM_ADDRESS *new_ad,int bytes,EMrStaticType type,
      bool editflag,mem_browser*mem_browser_update);
  void update();
  void edit(char*ttt);
  void setup_contextmenu();
  ~mr_static();
  bool active;
  HWND owner;
  HWND handle;
  HWND hLABEL;
  EMrStaticType type;
  char name[128];
  long*ptr;
  MEM_ADDRESS ad;
  int bytes;
  bool editflag;
  int Id; // v402
  mem_browser *mem_browser_update;
  Tiolist_entry *ile;
};

#define MAX_MR_STATICS 240//120 wasn't enough (MOVEM)
extern mr_static *m_s[MAX_MR_STATICS];

void mr_static_update_all();

#endif//#ifdef DEBUG_BUILD

#endif//MRSTATIC_DECLA_H

