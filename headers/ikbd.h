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

DOMAIN: Emu
FILE: ikbd.h
DESCRIPTION: Declarations for the keyboard (IKBD) emulation.
struct THD6301
This file is used by 6301.c.
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSE6301_H
#define SSE6301_H

#ifdef __cplusplus 
#include "steemh.h"
#include <conditions.h>
#include <parameters.h>
#include "THD6301.h"
#include "6301.h"

#define IKBD_MOUSE_MODE_OFF 0x12

extern "C" BYTE ST_Key_Down[128]; // used by 6301
extern "C" int mousek; 
extern int ikbd_joy_poll_line;
extern BYTE keyboard_buffer[MAX_KEYBOARD_BUFFER_SIZE];
extern WORD keyboard_buffer_length; // 0-MAX_KEYBOARD_BUFFER_SIZE-1
extern BYTE mouse_speed; // 1-19
extern BYTE disable_input_vbl_count;
extern int mouse_vbl_delta_x,mouse_vbl_delta_y;
extern bool mouse_vbl_delta;
extern const int ikbd_clock_days_in_mon[13];
extern const int ikbd_clock_max_val[6];
extern BYTE no_set_cursor_pos;
#ifdef WIN32
// When task switching is turned off we have to manually update these keys at the VBL
extern BYTE TaskSwitchVKList[4];
extern bool CutTaskSwitchVKDown[4];
#endif
extern "C" void keyboard_buffer_write(BYTE src); // called by the 6301 emu
extern "C" void keyboard_buffer_write_n_record(BYTE src);
void agenda_keyboard_replace(int);
void keyboard_buffer_write_string(int s1,...);
bool ikbd_keys_disabled();
void ikbd_mouse_move(int x,int y,int mouseb,
                     int max_mouse_move=IKBD_DEFAULT_MOUSE_MOVE_MAX);
void IKBD_VBL();
void agenda_ikbd_process(int src);  //intelligent keyboard handle byte
void ikbd_run_start(bool reset);
void ikbd_reset(bool hardware_reset);
void agenda_keyboard_reset(int SendF1);
void ikbd_report_abs_mouse(int abs_mousek_flags);
void ikbd_send_joystick_message(int jn);
void ikbd_set_clock_to_correct_time();
void UpdateSTKeys();
#endif//#ifdef __cplusplus 

extern
#ifdef __cplusplus
"C"
#endif
struct THD6301 Ikbd; // 6301.c must know about it

#endif//#ifndef SSE6301_H
