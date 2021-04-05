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
FILE: screen_saver.h
DESCRIPTION: ?
class TScreenSaver
---------------------------------------------------------------------------*/
#pragma once
#ifndef SCREEN_SAVER_DECLA_H
#define SCREEN_SAVER_DECLA_H

#include <wordwrapper.h>
#include "osd.h"

#pragma pack(push, 8)

class TScreenSaver {
private:
#ifdef WIN32
  HWND handle;
  HBITMAP buffer_bitmap;
  HDC buffer_dc;
  LPOSDDRAWCHARPROC draw_char_routine;
  static TScreenSaver* instance;
  TWordWrapper *word_wrapper;
  static const char* WINDOW_CLASS_NAME;
  BYTE *buffer_memory;
  EasyStr message_text;
  static LONG mousePos;
  static UINT_PTR timer_id;
  static int screen_width, screen_height;
  static const int BUFFER_WIDTH, BUFFER_HEIGHT;
  static const int ANIM_LENGTH;
  static const int COLOUR_SPEED;
  static int screen_saver_timeout_seconds;
  int animation_counter;
  int anim_col[3], anim_col_next[3], anim_col_target[3];
  int buffer_line_length;
  int buffer_size;  
#if defined(SSE_WINDOWS_XP)
  static DWORD screen_saver_activate_time;
#else
  static ULONGLONG screen_saver_activate_time;
#endif
  int message_x, message_y;
#endif
#ifdef UNIX
  static TScreenSaver* instance;
  static int screen_width, screen_height;
  static const int BUFFER_WIDTH, BUFFER_HEIGHT;
  static const int ANIM_LENGTH;
  static const int COLOUR_SPEED;
  static DWORD screen_saver_activate_time;
#endif
  bool buffer_valid;
  static bool timer_running;
  volatile static bool screensaver_active;
  static bool postponed;
  static BOOL screen_saver_turned_on;
  bool initBuffer();
  void initAnimation();
  void initMessage();
  void deleteBuffer();
  void animate();
  void advanceColour();
  void paint();

  static void updateTimer();

public:
  static void activate();
  static void hide();

  static void prepareTimer();
  static void killTimer();
  static void postpone();

  static bool isActive();

#ifdef WIN32
  static void checkMessage(MSG* mess);
  static LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
  static VOID CALLBACK activationTimerProc(HWND, UINT, UINT_PTR, DWORD);
  static VOID CALLBACK animationTimerProc(HWND, UINT, UINT_PTR, DWORD);
#endif

  static int getTextWidth(char*, int);

  TScreenSaver();
};

#pragma pack(pop)

#endif//#ifndef SCREEN_SAVER_DECLA_H

