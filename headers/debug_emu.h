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
FILE: debug_emu.h
DESCRIPTION: Declarations for debug_emu, utilities for the Debugger.
struct TDebugPluginfo
---------------------------------------------------------------------------*/

#pragma once
#ifndef DEBUGEMU_DECLA_H
#define DEBUGEMU_DECLA_H

#ifdef DEBUG_BUILD

#include <easystr.h>
#include <dynamicarray.h>
#include <conditions.h>
#include <SSE.h>


enum EDebugEmu {
  DRU_OFF=0,DRU_VBL,DRU_SCANLINE,DRU_CYCLE,DRU_INSTCHANGE,
  BREAK_IRQ_SPURIOUS=16, BREAK_IRQ_HBL_IDX, BREAK_IRQ_VBL_IDX, 
  BREAK_IRQ_LINEA_IDX, BREAK_IRQ_LINEF_IDX, BREAK_IRQ_TRAP_IDX,
  NUM_BREAK_IRQS,HIST_MENU_SIZE=20,
  //HISTORY_SIZE=1000,
  HISTORY_SIZE=15000, // enough for a frame
  MONITOR_MODE_STOP=2,MONITOR_MODE_LOG=3,
  MAX_BREAKPOINTS=30,
  CYCLE_COL_SPEED=4,
  CRASH_NOTIFICATION_BOMBS_DISPLAYED=1,CRASH_NOTIFICATION_NEVER,
  CRASH_NOTIFICATION_NOT_TOS,
};

struct m68k_exception;

void breakpoint_check();

int debug_get_ad_mode(MEM_ADDRESS ad);
WORD debug_get_ad_mask(MEM_ADDRESS ad,bool read);

extern int pc_history_idx;

extern bool debug_in_trace,debug_wipe_log_on_reset;
#if !defined(SSE_DEBUGGER_NODRAW)
extern bool redraw_on_stop,redraw_after_trace ;
extern COLORREF debug_gun_pos_col;
void debug_update_drawing_position(int* pHorz=NULL);
void update_display_after_trace();
#endif
void debug_update_cycle_counts();

char *reg_name(int n);
MEM_ADDRESS oi(MEM_ADDRESS,int);
extern int how_big_is_0000;
extern char reg_name_buf[8];

void stop_new_program_exec();
void debug_check_break_on_irq(int);
void debug_run_start(),debug_run_end();
void debug_trace_crash(m68k_exception &);
void debug_vbl();
void logfile_wipe();
void debug_reset();

extern int crash_notification;
extern bool stop_on_blitter_flag;
extern int stop_on_user_change;
extern int stop_on_next_program_run;

extern int stop_on_next_reset;

extern bool debug_first_instruction;

extern Str runstate_why_stop;
extern DWORD debug_cycles_since_VBL,debug_cycles_since_HBL;
extern COUNTER_VAR debug_ACT;
extern DWORD debug_USP,debug_SSP;
extern MEM_ADDRESS debug_VAP;
extern COUNTER_VAR debug_time_to_timer_timeout[4];
#if defined(SSE_DEBUGGER)
extern int debug_timer_prescale[4];
extern int debug_timer_data[4];
extern int debug_timer_count[4];
extern int debug_timer_ticks[4];
#endif
extern DWORD debug_frame_interrupts;
void debug_check_for_events();
void iolist_debug_add_pseudo_addresses();


extern int debug_cycle_colours ;
extern int debug_screen_shift ;


void debug_hit_mon(MEM_ADDRESS ad,int read);
void debug_hit_io_mon_write(MEM_ADDRESS ad,int val);

#if defined(SSE_DEBUGGER_MONITOR_RANGE)
/*  Adding range check: is ad between ad1 and ad2
    We use the first 2 watches
*/
  //bool debug_check_wr_check_range(MEM_ADDRESS ad,int num,MEM_ADDRESS *adarr,bool wr);
  bool debug_check_wr_check_range(MEM_ADDRESS ad,int num,MEM_ADDRESS *adarr); //390 warning

#define DEBUG_CHECK_WR_B(ad,num,adarr,maskarr,hit,wr) \
  if (num){ \
    WORD mask=WORD((ad & 1) ? 0x00ff:0xff00); \
    MEM_ADDRESS test_ad=ad & ~1;  \
    if(Debug.MonitorRange&&debug_check_wr_check_range(test_ad,num,adarr/*,wr*/)) \
      hit(ad,wr);\
    else\
    for (int ii=0;ii<num;ii++){ \
      if (adarr[ii]==test_ad){ \
        if (mask & maskarr[ii]){ \
          hit(ad,wr); \
          break; \
        }   \
      }         \
    }            \
  }

#define DEBUG_CHECK_WR_W(ad,num,adarr,hit,wr) \
  if (num){ \
    if(Debug.MonitorRange&&debug_check_wr_check_range(ad,num,adarr/*,wr*/)) \
      hit(ad,wr);\
    else\
    for (int ii=0;ii<num;ii++){ \
      if (adarr[ii]==ad){ \
        hit(ad,wr); \
        break; \
      }         \
    }            \
  }


//ad+2 is hypothesis
#define DEBUG_CHECK_WR_L(ad,num,adarr,hit,wr) \
  if (num){ \
    if(Debug.MonitorRange&&debug_check_wr_check_range(ad,num,adarr/*,wr*/)) \
      hit(ad,wr),hit(ad+2,wr);\
    else\
    for (int ii=0;ii<num;ii++){ \
      if (adarr[ii]==ad){ \
        hit(ad,wr); \
        break; \
      }         \
      if (adarr[ii]==ad+2){ \
        hit(ad+2,wr); \
        break; \
      }         \
    }            \
  }


#else

#define DEBUG_CHECK_WR_B(ad,num,adarr,maskarr,hit,wr) \
  if (num){ \
    WORD mask=WORD((ad & 1) ? 0x00ff:0xff00); \
    MEM_ADDRESS test_ad=ad & ~1;  \
    for (int i=0;i<num;i++){ \
      if (adarr[i]==test_ad){ \
        if (mask & maskarr[i]){ \
          hit(ad,wr); \
          break; \
        }   \
      }         \
    }            \
  }

#define DEBUG_CHECK_WR_W(ad,num,adarr,hit,wr) \
  if (num){ \
    for (int i=0;i<num;i++){ \
      if (adarr[i]==ad){ \
        hit(ad,wr); \
        break; \
      }         \
    }            \
  }

#define DEBUG_CHECK_WR_L(ad,num,adarr,hit,wr) \
  if (num){ \
    for (int i=0;i<num;i++){ \
      if (adarr[i]==ad){ \
        hit(ad,wr); \
        break; \
      }         \
      if (adarr[i]==ad+2){ \
        hit(ad+2,wr); \
        break; \
      }         \
    }            \
  }

#endif


#define DEBUG_CHECK_WRITE_B(ad) DEBUG_CHECK_WR_B((ad),debug_num_mon_writes,debug_mon_write_ad,debug_mon_write_mask,debug_hit_mon,0)
#define DEBUG_CHECK_WRITE_W(ad) DEBUG_CHECK_WR_W((ad),debug_num_mon_writes,debug_mon_write_ad,debug_hit_mon,0)
#define DEBUG_CHECK_WRITE_L(ad) DEBUG_CHECK_WR_L((ad),debug_num_mon_writes,debug_mon_write_ad,debug_hit_mon,0)
#define DEBUG_CHECK_READ_B(ad) DEBUG_CHECK_WR_B((ad),debug_num_mon_reads,debug_mon_read_ad,debug_mon_read_mask,debug_hit_mon,1)
#define DEBUG_CHECK_READ_W(ad) DEBUG_CHECK_WR_W((ad),debug_num_mon_reads,debug_mon_read_ad,debug_hit_mon,1)
#define DEBUG_CHECK_READ_L(ad) DEBUG_CHECK_WR_L((ad),debug_num_mon_reads,debug_mon_read_ad,debug_hit_mon,1)

#define DEBUG_CHECK_WRITE_IO_B(ad,v) DEBUG_CHECK_WR_B((ad),debug_num_mon_writes_io,debug_mon_write_ad_io,debug_mon_write_mask_io,debug_hit_io_mon_write,v)
#define DEBUG_CHECK_WRITE_IO_W(ad,v) DEBUG_CHECK_WR_W((ad),debug_num_mon_writes_io,debug_mon_write_ad_io,debug_hit_io_mon_write,v)
#define DEBUG_CHECK_WRITE_IO_L(ad,v) DEBUG_CHECK_WR_L((ad),debug_num_mon_writes_io,debug_mon_write_ad_io,debug_hit_io_mon_write,v)
#define DEBUG_CHECK_READ_IO_B(ad) DEBUG_CHECK_WR_B((ad),debug_num_mon_reads_io,debug_mon_read_ad_io,debug_mon_read_mask_io,debug_hit_mon,1)
#define DEBUG_CHECK_READ_IO_W(ad) DEBUG_CHECK_WR_W((ad),debug_num_mon_reads_io,debug_mon_read_ad_io,debug_hit_mon,1)
#define DEBUG_CHECK_READ_IO_L(ad) DEBUG_CHECK_WR_L((ad),debug_num_mon_reads_io,debug_mon_read_ad_io,debug_hit_mon,1)


extern int debug_num_bk ,debug_num_mon_reads ,debug_num_mon_writes ;
extern int debug_num_mon_reads_io ,debug_num_mon_writes_io ;

extern MEM_ADDRESS debug_bk_ad[MAX_BREAKPOINTS],debug_mon_read_ad[MAX_BREAKPOINTS],debug_mon_write_ad[MAX_BREAKPOINTS];
extern MEM_ADDRESS debug_mon_read_ad_io[MAX_BREAKPOINTS],debug_mon_write_ad_io[MAX_BREAKPOINTS];
extern WORD debug_mon_read_mask[MAX_BREAKPOINTS],debug_mon_write_mask[MAX_BREAKPOINTS];
extern WORD debug_mon_read_mask_io[MAX_BREAKPOINTS],debug_mon_write_mask_io[MAX_BREAKPOINTS];

extern MEM_ADDRESS trace_over_breakpoint;


extern COUNTER_VAR debug_run_until,debug_run_until_val;

extern int monitor_mode,breakpoint_mode;
extern bool break_on_irq[NUM_BREAK_IRQS];


extern MEM_ADDRESS pc_history[HISTORY_SIZE];
extern short pc_history_y[HISTORY_SIZE]; // scanline
extern short pc_history_c[HISTORY_SIZE]; // linecycle

extern int pc_history_idx ;
extern BYTE debug_send_alt_keys ,debug_send_alt_keys_vbl_countdown ;

extern int PASCAL debug_plugin_read_mem(DWORD,BYTE*,int);
extern int PASCAL debug_plugin_write_mem(DWORD,BYTE*,int);
typedef void PASCAL DEBUGPLUGIN_INITPROC(void**,char*);
typedef void PASCAL DEBUGPLUGIN_ACTIVATEPROC(int);
typedef void PASCAL DEBUGPLUGIN_CLOSEPROC();

#pragma pack(push, 8)

struct TDebugPluginfo { // SS I know of no such plugin
  HINSTANCE hDll;
  DEBUGPLUGIN_INITPROC *Init;
  DEBUGPLUGIN_ACTIVATEPROC *Activate;
  DEBUGPLUGIN_CLOSEPROC *Close;
  char Menu[512];
};

#pragma pack(pop)

extern void *debug_plugin_routines[];
extern DynamicArray<TDebugPluginfo> debug_plugins;

extern HWND DbgCreatePushButton(char* caption,int x,int y,int &wid,HWND owner,
                                int menu);


#endif//#ifdef DEBUG_BUILD

#endif// DEBUGEMU_DECLA_H
