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

DOMAIN: Debug, GUI
FILE: dwin_edit.h
DESCRIPTION: Declarations for an edit window.
---------------------------------------------------------------------------*/

#pragma once
#ifndef DWINEDIT_DECLA_H
#define DWINEDIT_DECLA_H

#ifdef DEBUG_BUILD

#include <conditions.h>

/////////////////////////////// DWin_edit ////////////////////////////////////

extern WNDPROC Old_edit_WndProc;
extern void* DWin_edit_subject;
extern int DWin_edit_subject_type;
extern int DWin_edit_subject_index;
extern int DWin_edit_subject_col;
extern MEM_ADDRESS DWin_edit_subject_ad;
enum EDWin_edit_subject_content{DWESC_HEX=0,DWESC_TEXT,DWESC_BIN,DWESC_DISA};
extern EDWin_edit_subject_content DWin_edit_subject_content;
extern bool DWin_edit_is_being_temporarily_defocussed;
LRESULT CALLBACK DWin_edit_WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
void set_DWin_edit(int type,void*subject,int n,int col);
void DWin_edit_finish(bool);

#endif//#ifdef DEBUG_BUILD

#endif//DWINEDIT_DECLA_H

