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
FILE: debugger.h
DESCRIPTION: Declarations for the Debugger.
struct TDebugAddress
---------------------------------------------------------------------------*/

#pragma once
#ifndef BOILER_DECLA_H
#define BOILER_DECLA_H

#ifdef DEBUG_BUILD

#include "mem_browser.h"
#include "historylist.h"
#include "scrollingcontrolswin.h"

void DWin_init();
#if defined(SSE_DEBUGGER_TOGGLE)
extern BOOL DebuggerVisible;
void DebuggerToggle(BOOL visible);
#endif
LRESULT CALLBACK sr_display_WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);

extern DWORD_PTR dpc;
extern MEM_ADDRESS old_dpc;
extern HWND DWin,HiddenParent;
extern HMENU menu,breakpoint_menu,monitor_menu,breakpoint_irq_menu;
extern HMENU insp_menu;
extern HMENU mem_browser_menu,history_menu,logsection_menu;
extern HMENU menu1;
extern HMENU boiler_op_menu,shift_screen_menu;
extern HMENU logsection_menu2,sse_menu;
extern HWND sr_display,DWin_edit;
extern mr_static *lpms_other_sp;
extern HWND DWin_trace_button,DWin_trace_over_button,DWin_run_button;

extern Str LogViewProg;

void boiler_show_stack_display(int);
extern ScrollControlWin DWin_timings_scroller;
extern HWND DWin_right_display_combo;

extern WNDPROC Old_sr_display_WndProc;

extern mem_browser m_b_mem_disa,m_b_stack;

#define SIMULTRACE_CHOOSE ((HWND)(INT_PTR)(0xffffffff))
extern HWND simultrace;

void debug_load_file_to_address(HWND,MEM_ADDRESS);

Str debug_parse_disa_for_display(Str);
extern bool debug_monospace_disa,debug_uppercase_disa;

extern bool d2_trace;

#if defined(SSE_DEBUGGER_STATUS_BAR)
extern HWND hDbgStatusBar;
HWND DoCreateStatusBar(HWND hwndParent, HMENU idStatus, HINSTANCE
                   hinst, int cParts);
void DbgStatusBarMsg(char *Mess);
#endif

extern BYTE cbomb;
extern BYTE rowbombs[4];


/////////////////////////////// insp menu ////////////////////////////////////
extern int insp_menu_subject_type;
extern void* insp_menu_subject;
extern long insp_menu_long[3];
extern char insp_menu_long_name[3][100];
extern int insp_menu_long_bytes[3];
void insp_menu_setup();
extern int insp_menu_col,insp_menu_row;

/////////////////////////////// breakpoints ////////////////////////////////////
#pragma pack(push, 8)

struct TDebugAddress{
  MEM_ADDRESS ad;
  int mode; // 0=off, 1=global, 2=break, 3=log
  int bwr; // & 1=break, & 2=write, & 4=read
  WORD mask[2]; // write mask, read mask
  char name[64];
};

TDebugAddress *debug_find_address(MEM_ADDRESS);

#pragma pack(pop)

void debug_remove_address(MEM_ADDRESS);
void debug_update_bkmon();
void breakpoint_menu_setup();
void logfile_wipe();
void debug_set_bk(MEM_ADDRESS,bool);
void debug_set_mon(MEM_ADDRESS,bool,WORD);

extern DynamicArray<TDebugAddress> debug_ads;

/////////////////////////////// logfile  ////////////////////////////////////

//////////////////////////////// routines //////////////////////////////////
void update_register_display(bool);
void disa_to_file(FILE*f,MEM_ADDRESS dstart,int dlen,bool);

extern THistoryList HistList;

void debug_plugin_load(),debug_plugin_free();

#endif//#ifdef DEBUG_BUILD

#endif//BOILER_DECLA_H

