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
FILE: mem_browser.h
DESCRIPTION: Core element of the Debugger.
class mem_browser
---------------------------------------------------------------------------*/

#pragma once
#ifndef MEMBROWSER_DECLA_H
#define MEMBROWSER_DECLA_H

#ifdef DEBUG_BUILD

#include <easystr.h>
#include <easystringlist.h>
#include <parameters.h>
#ifdef MINGW_BUILD
#define NULL 0
#endif


enum ETypeDispType{DT_INSTRUCTION=0,DT_MEMORY,DT_REGISTERS};
enum ETypeMode{MB_MODE_STANDARD=0,MB_MODE_PC,MB_MODE_STACK,MB_MODE_FIXED,
MB_MODE_IOLIST};

void mem_browser_update_all();
LRESULT CALLBACK mem_browser_WndProc(HWND Win,UINT Mess,
  WPARAM wPar,LPARAM lPar);
extern WNDPROC Old_mem_browser_WndProc;
LRESULT CALLBACK mem_browser_window_WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);


class mr_static;

#pragma pack(push, 8)

class mem_browser {
  static LRESULT CALLBACK FindEditWndProc(HWND Win,UINT Mess,
    WPARAM wPar,LPARAM lPar);
  static WNDPROC OldEditWndProc;
  void listbox_add_line(HWND,int,char**,int);
  char* get_mem_mon_string(void*);
  Str get_hex_map(MEM_ADDRESS);
public:
//  bool active;
  HWND owner; // handle of associated window
  HWND handle; // handle of list box
  ETypeDispType disp_type;
  MEM_ADDRESS ad;
  ETypeMode mode;
  int wpl,lb_height,columns,text_column,disa_column,mon_column,break_column,hex_column;
  bool editflag,init_text;
  mr_static *editbox;
  static HBITMAP icons_bmp;
  static HDC icons_dc;
  EasyStringList hex_map;

  mem_browser() {
    owner=NULL;handle=NULL;disp_type=DT_MEMORY;ad=0;mode=MB_MODE_STANDARD;
    wpl=1;lb_height=3;editflag=true;editbox=NULL;columns=0;init_text=0;
  }
  mem_browser(MEM_ADDRESS new_ad,ETypeDispType dt) {
    init_text=0;columns=0;new_window(new_ad,dt);
  }
  void new_window(MEM_ADDRESS,ETypeDispType);
  void update();
  void init();
  void draw(DRAWITEMSTRUCT*);
  int calculate_wpl();
  void get_breakpoint_labels(MEM_ADDRESS ad,int bpl,char*t[3]);
  MEM_ADDRESS get_address_from_row(int row);
  void vscroll(int of);
  void setup_contextmenu(int row,int col);
  void update_icon();

  ~mem_browser();

  static DWORD ex_style;
};

#pragma pack(pop)

#define MAX_MEMORY_BROWSERS 20

extern mem_browser *m_b[MAX_MEMORY_BROWSERS];

extern char reg_browser_entry_name[20][8];
extern unsigned long *reg_browser_entry_pointer[20];
#define NUM_REGISTERS_IN_REGISTER_BROWSER 18

#endif//#ifdef DEBUG_BUILD

#endif//MEMBROWSER_DECLA_H

