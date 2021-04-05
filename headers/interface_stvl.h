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
FILE: interface_stvl.h
DESCRIPTION: Declarations for Steem's STVL interface.
STVL is a plugin for ST emulators able to emulate the video logic of an STF
or an STE at low level.
---------------------------------------------------------------------------*/

#pragma once
#ifndef INTERFACE_STVL_H
#define INTERFACE_STVL_H

#if defined(SSE_VID_STVL)

#include "conditions.h"
#include "steemh.h"
#include "parameters.h"
#include <stvl/stvl.h>

extern HMODULE hStvl; //dll
extern TStvl Stvl;
extern WORD render_vstart, render_vend;
extern WORD render_hstart;
extern WORD render_scanline_length;
extern DWORD *draw_mem_line_ptr;

// pointers 
extern DWORD (STVL_CALLCONV *video_logic_init)(TStvl *pStvl);
extern void (STVL_CALLCONV *video_logic_reset)(TStvl *pStvl,bool Cold);
extern void (STVL_CALLCONV *video_logic_stf_run)(TStvl *pStvl,int cycles);
extern void (STVL_CALLCONV *video_logic_ste_run)(TStvl *pStvl,int cycles);
extern void (STVL_CALLCONV *video_logic_update)(TStvl *pStvl);
//extern void (STVL_CALLCONV*video_logic_update_pal)(TStvl *pStvl,int n,WORD pal);
void StvlUpdate();
void StvlInit();
void cb_mfp_de_transition();

extern COUNTER_VAR acc_cycles;
// 1 low-level video, no acceleration
// 2 low-level video, acceleration
// It's quite a long list and each one needs a definition!
void InstructionTimeStf1Idle(int t);
void InstructionTimeStf1WS(int t);
void InstructionTimeStf1PrefetchOnly();
void InstructionTimeStf1PrefetchFinal();
void InstructionTimeStf1PrefetchTotal();
void InstructionTimeStf1ReadB();
void InstructionTimeStf1Read();
void InstructionTimeStf1Write();
void InstructionTimeStf1WriteB();
void InstructionTimeStf2Idle(int t);
void InstructionTimeStf2WS(int t);
void InstructionTimeStf2PrefetchOnly();
void InstructionTimeStf2PrefetchFinal();
void InstructionTimeStf2PrefetchTotal();
void InstructionTimeStf2ReadB();
void InstructionTimeStf2Read();
void InstructionTimeStf2Write();
void InstructionTimeStf2WriteB();
void InstructionTimeMegaSt1Idle(int t);
void InstructionTimeMegaSt1WS(int t);
void InstructionTimeMegaSt1PrefetchOnly();
void InstructionTimeMegaSt1PrefetchFinal();
void InstructionTimeMegaSt1PrefetchTotal();
void InstructionTimeMegaSt1ReadB();
void InstructionTimeMegaSt1Read();
void InstructionTimeMegaSt1Write();
void InstructionTimeMegaSt1WriteB();
void InstructionTimeMegaSt1BltRead();
void InstructionTimeMegaSt1BltWrite();
void InstructionTimeMegaSt2Idle(int t);
void InstructionTimeMegaSt2WS(int t);
void InstructionTimeMegaSt2PrefetchOnly();
void InstructionTimeMegaSt2PrefetchFinal();
void InstructionTimeMegaSt2PrefetchTotal();
void InstructionTimeMegaSt2ReadB();
void InstructionTimeMegaSt2Read();
void InstructionTimeMegaSt2Write();
void InstructionTimeMegaSt2WriteB();
void InstructionTimeMegaSt2BltRead();
void InstructionTimeMegaSt2BltWrite();
void InstructionTimeSte1Idle(int t);
void InstructionTimeSte1WS(int t);
void InstructionTimeSte1PrefetchOnly();
void InstructionTimeSte1PrefetchFinal();
void InstructionTimeSte1PrefetchTotal();
void InstructionTimeSte1ReadB();
void InstructionTimeSte1Read();
void InstructionTimeSte1Write();
void InstructionTimeSte1WriteB();
void InstructionTimeSte1BltRead();
void InstructionTimeSte1BltWrite();
#if defined(SSE_MEGASTE) // we don't do accelerated
void InstructionTimeMegaSte1Idle(int t);
void InstructionTimeMegaSte1WS(int t);
void InstructionTimeMegaSte1PrefetchOnly();
void InstructionTimeMegaSte1PrefetchFinal();
void InstructionTimeMegaSte1PrefetchTotal();
void InstructionTimeMegaSte1ReadB();
void InstructionTimeMegaSte1Read();
void InstructionTimeMegaSte1Write();
void InstructionTimeMegaSte1WriteB();
#endif
void InstructionTimeSte2Idle(int t);
void InstructionTimeSte2WS(int t);
void InstructionTimeSte2PrefetchOnly();
void InstructionTimeSte2PrefetchFinal();
void InstructionTimeSte2PrefetchTotal();
void InstructionTimeSte2ReadB();
void InstructionTimeSte2Read();
void InstructionTimeSte2Write();
void InstructionTimeSte2WriteB();
void InstructionTimeSte2BltRead();
void InstructionTimeSte2BltWrite();

#endif//#if defined(SSE_VID_STVL)

#endif//#ifndef INTERFACE_STVL_H

