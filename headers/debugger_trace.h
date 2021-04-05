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
FILE: debugger_trace.h
DESCRIPTION: Declarations for the Trace function of the Debugger.
struct Ttrace_display_entry
---------------------------------------------------------------------------*/
#pragma once
#ifndef TRACE_DECLA_H
#define TRACE_DECLA_H

#ifdef DEBUG_BUILD

#include <cpu.h>
#include <mem_browser.h>


#define TDE_BEFORE 1
#define TDE_AFTER 2
#define TDE_TEXT_ONLY 0x80

#pragma pack(push, 8)

struct Ttrace_display_entry {
  short when;
  bool regflag;
  MEM_ADDRESS ad;
  char name[100];
  int bytes;
  unsigned long val[2];
  unsigned long *ptr; // pointer to a byte, word, long
};

#pragma pack(pop)

#define MAX_TRACE_DISPLAY_ENTRIES 50
extern Ttrace_display_entry t_d_e[MAX_TRACE_DISPLAY_ENTRIES];
extern unsigned short trace_sr_before,trace_sr_after;
extern MEM_ADDRESS trace_pc;
extern int trace_entries;
extern mem_browser m_b_trace;
extern HWND trace_window_handle;
extern HWND trace_repeat_trace_button;
extern HWND trace_hLABEL[MAX_TRACE_DISPLAY_ENTRIES];
extern HWND trace_sr_before_display,trace_sr_after_display;
extern ScrollControlWin trace_scroller;
extern bool trace_show_window;

LRESULT CALLBACK trace_window_WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
void trace_window_init();
void trace_init();
void trace_add_entry(char*name1,char*name2,short when,bool regflag,
    int bytes,MEM_ADDRESS *ad);
void trace_add_movem_block(char*name,int,short when,int bytes,MEM_ADDRESS ad,int count);
void trace_get_after();
void trace_display();
void trace();
void trace_exception_display(m68k_exception*exc);
void trace_add_text(char*tt);


extern const char*bombs_name[12];

extern const char*exception_action_name[4];

#endif//#ifdef DEBUG_BUILD

#endif//#ifndef TRACE_DECLA_H

