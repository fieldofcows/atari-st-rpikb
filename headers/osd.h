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

DOMAIN: Rendering
FILE: osd.h
DESCRIPTION: Declarations for OSD (On Screen Display).
struct TOsdControl
---------------------------------------------------------------------------*/

#pragma once
#ifndef OSD_DECLA_H
#define OSD_DECLA_H

#include <easystr.h>
#include <easystringlist.h>
#include <conditions.h>
#include <parameters.h>
#include <draw.h>


#define OSD_LOGO_W 124
#define OSD_LOGO_H 11
#define OSD_SHOW_ALWAYS 0xff

extern DWORD *osd_plasma_pal;
extern BYTE *osd_plasma;
extern long *osd_font;
extern EasyStr osd_scroller;
extern EasyStringList osd_scroller_array;
extern DWORD osd_scroller_finish_time,FDCCantWriteDisplayTimer;
extern long col_blue,col_white;
void osd_draw_begin();
void osd_init_run(bool allow_scroller);
void osd_draw();
void osd_hide();
void osd_draw_end();
void osd_routines_init();
void osd_init_draw_static();
EasyStr get_osd_scroller_text(int n);

#ifdef WIN32
void osd_draw_reset_info(HDC dc);
LRESULT CALLBACK ResetInfoWndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
extern HWND ResetInfoWin;
#else
void osd_draw_reset_info(int,int,int,int);
#endif

#if defined(SSE_VID_32BIT_ONLY_410)
void ASMCALL osd_draw_char_dont(long*,DWORD*,long,long,int,long,long);
typedef void ASMCALL OSDDRAWCHARPROC(long*,DWORD*,long,long,int,long,long);
#else
void ASMCALL osd_draw_char_dont(long*,BYTE*,long,long,int,long,long);
typedef void ASMCALL OSDDRAWCHARPROC(long*,BYTE*,long,long,int,long,long);
#endif
typedef OSDDRAWCHARPROC* LPOSDDRAWCHARPROC;
#if defined(SSE_VID_32BIT_ONLY)
extern LPOSDDRAWCHARPROC jump_osd_draw_char;
#else
extern LPOSDDRAWCHARPROC jump_osd_draw_char[4];
#endif


extern "C" {
#if !defined(SSE_VID_32BIT_ONLY)
void ASMCALL osd_draw_char_clipped_8(long*,BYTE*,long,long,int,long,long,RECT*);
void ASMCALL osd_draw_char_clipped_16(long*,BYTE*,long,long,int,long,long,RECT*);
void ASMCALL osd_draw_char_clipped_24(long*,BYTE*,long,long,int,long,long,RECT*);
#endif
#if defined(SSE_VID_32BIT_ONLY_410)
void ASMCALL osd_draw_char_clipped_32(long* src,DWORD* dst,long x,long y,int l,
                                      long c,long s,RECT* cr);
#else
void ASMCALL osd_draw_char_clipped_32(long* src,BYTE* dst,long x,long y,int l,
                                      long c,long s,RECT* cr);
#endif
#if !defined(SSE_VID_32BIT_ONLY)
void ASMCALL osd_draw_char_8(long*,BYTE*,long,long,int,long,long);
void ASMCALL osd_draw_char_16(long*,BYTE*,long,long,int,long,long);
void ASMCALL osd_draw_char_24(long*,BYTE*,long,long,int,long,long);
#endif

void ASMCALL osd_draw_char_32(long*source_ad,draw_type*dst,long x,long y,
                              int l,long colour,long h);

#if !defined(SSE_VID_32BIT_ONLY)
void ASMCALL osd_draw_char_transparent_8(long*,BYTE*,long,long,int,long,long);
void ASMCALL osd_draw_char_transparent_16(long*,BYTE*,long,long,int,long,long);
void ASMCALL osd_draw_char_transparent_24(long*,BYTE*,long,long,int,long,long);
#endif
#if defined(SSE_VID_32BIT_ONLY_410)
void ASMCALL osd_draw_char_transparent_32(long*source_ad,DWORD*draw_mem,long x,
                                 long y,int draw_line_length,long colour,long h);
#else
void ASMCALL osd_draw_char_transparent_32(long*source_ad,BYTE*draw_mem,long x,
                                 long y,int draw_line_length,long colour,long h);
#endif
#if !defined(SSE_VID_32BIT_ONLY)
void ASMCALL osd_draw_char_clipped_transparent_8(long*,BYTE*,long,long,int,long,long,RECT*);
void ASMCALL osd_draw_char_clipped_transparent_16(long*,BYTE*,long,long,int,long,long,RECT*);
void ASMCALL osd_draw_char_clipped_transparent_24(long*,BYTE*,long,long,int,long,long,RECT*);
#endif
#if defined(SSE_VID_32BIT_ONLY_410)
void ASMCALL osd_draw_char_clipped_transparent_32(long* src,DWORD* dst,long x,
                                           long y,int l,long c,long s,RECT* cr);
#else
void ASMCALL osd_draw_char_clipped_transparent_32(long* src,BYTE* dst,long x,
                                           long y,int l,long c,long s,RECT* cr);
#endif
#if !defined(SSE_VID_32BIT_ONLY)
void ASMCALL osd_blueize_line_16_555(int,int,int),ASMCALL osd_blueize_line_16_565(int,int,int);
void ASMCALL osd_blueize_line_24(int,int,int);
#endif
void ASMCALL osd_blueize_line_32(int x,int y,int w);
#if !defined(SSE_VID_32BIT_ONLY)
void palette_convert_16_555(int),palette_convert_line_16_565(int);
void palette_convert_line_24(int);
#endif
void palette_convert_line_32(int);
#if !defined(SSE_VID_32BIT_ONLY)
void ASMCALL osd_black_box_8(void*,int,int,int,int,long);
void ASMCALL osd_black_box_16(void*,int,int,int,int,long);
void ASMCALL osd_black_box_24(void*,int,int,int,int,long);
#endif
void ASMCALL osd_black_box_32(void* dst,int x,int y,int w,int h,long l);
} // extern "C"


#pragma pack(push, 8)

struct TOsdControl {
  enum EOsdControl {NO_SCROLLER,WANT_SCROLLER,SCROLLING};
  TOsdControl();
  void HdLed(int time=HD_TIMER);
  void Trace(char *fmt,...); // yellow message in top left and/or statusbar
  void UpdateTraceTo();
#if defined(SSE_OSD_SHOW_TIME)
  DWORD StartingTime; // record time on cold reset
  DWORD StoppingTime; // to adjust when stopping/restarting
#endif
  DWORD MessageTimer;
  long ScrollerPosition;
  //long ScrollerColour;
  BYTE ScrollerPhase;
  BYTE TraceTo; // BIT_0 OSD BIT_1 Statusbar
  //EasyStr ScrollText;
  char m_OsdMessage[OSD_MESSAGE_LENGTH+1]; // +null as usual
  BYTE show_plasma,show_speed,show_icons,show_cpu;
  bool show_disk_light,show_scrollers;
  bool no_draw,disable,bOsdDrawn;
};

extern TOsdControl OsdControl;

#pragma pack(pop)


#endif//OSD_DECLA_H
