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
FILE: d2.h
DESCRIPTION: Declarations for d2, part of the Debugger.
---------------------------------------------------------------------------*/

#pragma once
#ifndef D2_DECLA_H
#define D2_DECLA_H

#define D2_PC_RELATIVE_PC ((MEM_ADDRESS)dpc)

#define D2_GET_SOURCE_B d2_jump_get_source_b[(IRD&BITS_543)>>3]()
#define D2_GET_SOURCE_W d2_jump_get_source_w[(IRD&BITS_543)>>3]()
#define D2_GET_SOURCE_L d2_jump_get_source_l[(IRD&BITS_543)>>3]()

#define D2_GET_DEST_B d2_jump_get_dest_b[(IRD&BITS_543)>>3]()
#define D2_GET_DEST_W d2_jump_get_dest_w[(IRD&BITS_543)>>3]()
#define D2_GET_DEST_L d2_jump_get_dest_l[(IRD&BITS_543)>>3]()

#define D2_GET_DEST_B_NOT_A d2_jump_get_dest_b_not_a[(IRD&BITS_543)>>3]()
#define D2_GET_DEST_W_NOT_A d2_jump_get_dest_w_not_a[(IRD&BITS_543)>>3]()
#define D2_GET_DEST_L_NOT_A d2_jump_get_dest_l_not_a[(IRD&BITS_543)>>3]()

#define D2_GET_DEST_B_NOT_A_OR_D d2_jump_get_dest_b_not_a_or_d[(IRD&BITS_543)>>3]()
#define D2_GET_DEST_W_NOT_A_OR_D d2_jump_get_dest_w_not_a_or_d[(IRD&BITS_543)>>3]()
#define D2_GET_DEST_L_NOT_A_OR_D d2_jump_get_dest_l_not_a_or_d[(IRD&BITS_543)>>3]()


#define D2_GET_DEST_B_NOT_A_WITH_SR d2_jump_get_dest_b_not_a_with_sr[(IRD&BITS_543)>>3]()
#define D2_GET_DEST_W_NOT_A_WITH_SR d2_jump_get_dest_w_not_a_with_sr[(IRD&BITS_543)>>3]()
#define D2_GET_DEST_L_NOT_A_WITH_SR d2_jump_get_dest_l_not_a_with_sr[(IRD&BITS_543)>>3]()

#define D2_GET_IMMEDIATE_B \
  d2_src_b=EasyStr("#$")+itoa(d2_fetchB(),d2_t_buf,16); \
  trace_add_entry("source immediate: ",d2_src_b.c_str(),TDE_BEFORE,false,1,\
    (MEM_ADDRESS*)(dpc+1));\
  dpc+=2
#define D2_GET_IMMEDIATE_W \
  d2_src_w=EasyStr("#$")+itoa(d2_fetchW(),d2_t_buf,16); \
  trace_add_entry("source immediate: ",d2_src_w.c_str(),TDE_BEFORE,false,2,\
    (MEM_ADDRESS*)dpc);\
  dpc+=2

#define D2_GET_IMMEDIATE_L \
  d2_src_l=EasyStr("#$")+itoa(d2_fetchL(),d2_t_buf,16); \
  trace_add_entry("source immediate: ",d2_src_l.c_str(),TDE_BEFORE,false,4,\
    (MEM_ADDRESS*)dpc);\
  dpc+=4

#define d2_src_b d2_src
#define d2_src_w d2_src
#define d2_src_l d2_src
#define D2_BIT_SHIFT_TO_dM_GET_SOURCE         \
  if(IRD&BIT_5){                               \
    d2_src_w=D2_dN;                           \
    trace_add_entry("source register: ",reg_name(PARAM_N),TDE_BEFORE,true,2,(MEM_ADDRESS*)&Cpu.r[PARAM_N]); \
  }else{                                      \
    d2_src_w=EasyStr("#")+((((PARAM_N)-1)&7)+1);   \
  }
#if 1
extern char str_d[8][3];
extern char str_a[8][3];
extern char str_a_brackets[8][5];
#define D2_dN EasyStr(str_d[PARAM_N])
#define D2_aN EasyStr(str_a[PARAM_N])
#define D2_dM EasyStr(str_d[PARAM_M])
#define D2_aM EasyStr(str_a[PARAM_M])
#define D2_BRACKETS_aN EasyStr(str_a_brackets[PARAM_N])
#define D2_BRACKETS_aM EasyStr(str_a_brackets[PARAM_M])
#elif 0
#define D2_dN EasyStr("d")+PARAM_N
#define D2_aN EasyStr("a")+PARAM_N
#define D2_dM EasyStr("d")+PARAM_M
#define D2_aM EasyStr("a")+PARAM_M
#define D2_BRACKETS_aN EasyStr("(a")+PARAM_N+")"
#define D2_BRACKETS_aM EasyStr("(a")+PARAM_M+")"
#else
#define D2_dN EasyStr("d")+("0\0001\0002\0003\0004\0005\0006\0007\000"+2*PARAM_N)
#define D2_aN EasyStr("a")+("0\0001\0002\0003\0004\0005\0006\0007\000"+2*PARAM_N)
#define D2_dM EasyStr("d")+("0\0001\0002\0003\0004\0005\0006\0007\000"+2*PARAM_M)
#define D2_aM EasyStr("a")+("0\0001\0002\0003\0004\0005\0006\0007\000"+2*PARAM_M)
#define D2_BRACKETS_aN EasyStr("(a")+("0\0001\0002\0003\0004\0005\0006\0007\000"+2*PARAM_N)+")"
#define D2_BRACKETS_aM EasyStr("(a")+("0\0001\0002\0003\0004\0005\0006\0007\000"+2*PARAM_M)+")"
#endif
#define D2_IRIWO d2_iriwo()
#define D2_IRIWO_N d2_iriwo_N()
#define D2_IRIWO_PC d2_iriwo_pc()


EasyStr disa_d2(MEM_ADDRESS,WORD pir=0);

extern char d2_t_buf[200];
extern DWORD d2_peekinvalid;

BYTE d2_peek(MEM_ADDRESS ad);
WORD d2_dpeek(MEM_ADDRESS ad);
DWORD d2_lpeek(MEM_ADDRESS ad);

bool d2_poke(MEM_ADDRESS ad,BYTE val);
bool d2_dpoke(MEM_ADDRESS ad,WORD val);
bool d2_lpoke(MEM_ADDRESS ad,LONG val);
void d2_routines_init();
void debug_change_upper();
void set_up_reg_browser();

extern EasyStr d2_src,d2_dest,d2_command,d2_pc_rel_ex;
extern WORD d2_ap;
extern int d2_n_movem_regs;
extern char str_pc[3],str_pc_rel[5];

#endif//D2_DECLA_H
