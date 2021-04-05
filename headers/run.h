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
FILE: run.h
DESCRIPTION: Declarations for Steem's central run() function.
struct TEvent
---------------------------------------------------------------------------*/

#pragma once
#ifndef RUN_DECLA_H
#define RUN_DECLA_H

#include "conditions.h"

#define RUNSTATE_RUNNING  0
#define RUNSTATE_STOPPING 1
#define RUNSTATE_STOPPED  2

#define ABSOLUTE_SYS_TIME (sys_timer-sys_cycles) // was ABSOLUTE_CPU_TIME
#define A_S_T ABSOLUTE_SYS_TIME // was ACT


#define INTERRUPT_START_TIME_WOBBLE  \
          sys_cycles&=-4*TICKS8; \
          CPU_BUS_IDLE((EIGHT_MILLION-(ABSOLUTE_SYS_TIME-shifter_cycle_base)/TICKS8)%10);   

#define CALC_VIDEO_FREQ_IDX           \
            switch(Glue.VideoFreq){        \
              case 50:      VideoFreqIdx=0; break;  \
              case 60:      VideoFreqIdx=1; break;   \
              default:      VideoFreqIdx=2;   \
            }

#ifdef DEBUG_BUILD

void event_debug_stop();
#define CHECK_BREAKPOINT                     \
        if (debug_num_bk){ \
          if (debug_first_instruction==0) breakpoint_check();     \
        }   \
        if (LITTLE_PC==trace_over_breakpoint){ \
          if (runstate==RUNSTATE_RUNNING) runstate=RUNSTATE_STOPPING;                 \
        } 

#define SET_WHY_STOP(s) runstate_why_stop=s;
extern int stem_runmode;

#else

#define CHECK_BREAKPOINT
#define SET_WHY_STOP(s)

#endif

typedef void(*EVENTPROC)();

#pragma pack(push, 8)

struct TEvent {
  EVENTPROC event;
  COUNTER_VAR time;
};

#pragma pack(pop)

void run(); // can be called in an apart thread
void AdaptCpuBoost();
void prepare_next_event();
void event_timer_a_timeout();
void event_timer_b_timeout();
void event_timer_c_timeout();
void event_timer_d_timeout();
void event_scanline();
void event_timer_b();
void event_start_vbl();
void event_vbl_interrupt();
#if USE_PASTI
void event_pasti_update();
#endif
// SSE
void event_trigger_vbi();
void event_wd1772(); //1 event for FDC: various parts of its program
void event_driveA_ip(); // 1 event for each drive: index pulse
void event_driveB_ip();
void event_acia();
void event_dummy();

extern int runstate;

// fast_forward_max_speed=(1000 / (max %/100)); 0 for unlimited
extern int fast_forward,fast_forward_max_speed;
extern bool fast_forward_stuck_down;
extern int slow_motion,slow_motion_speed;
extern int run_speed_ticks_per_second;
extern bool disable_speed_limiting;
extern int run_start_time;
UNIX_ONLY(extern bool RunWhenStop; )
extern DWORD avg_frame_time,avg_frame_time_timer,frame_delay_timeout,timer;
extern DWORD speed_limit_wait_till;
extern int avg_frame_time_counter;
extern DWORD auto_frameskip_target_time;
extern int frameskip, frameskip_count;
extern bool flashlight_flag;
extern COUNTER_VAR time_of_event_acia;
extern COUNTER_VAR time_of_next_event;
extern COUNTER_VAR time_of_next_timer_b;
extern COUNTER_VAR time_of_last_hbl_interrupt,time_of_last_vbl_interrupt;
extern COUNTER_VAR sys_timer_at_res_change;
extern EVENTPROC event_vector;
extern EVENTPROC event_mfp_timer_timeout[4];
extern int CyclesPerScanlineAtStartOfVbl;

#endif//RUN_DECLA_H
