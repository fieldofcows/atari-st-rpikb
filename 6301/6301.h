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
FILE: 6301.c
DESCRIPTION: Declarations to interface Steem SSE with a modified version of
sim68xx (Copyright (c) 1994-1996 Arne Riiber) to emulate at a lower level
the HD6301V1 chip that was in the ST, as described in 'HD6301V1 on ST.txt'.
This emulation was supposed to run as terminal on a DOS PC. A precise
integration with another serial device (MC6850 ACIA in the ST) wasn't the 
focus.
The functions in original files have been modified where necessary, some
have been added, in 6301.c, and in ireg.c (most interesting ones). 
This is C, not C++. 6301.c includes other C files.
All functions and variables beginning with hd6301 are declared in this
6301 module and are meant to be visible to Steem (interface) by way of
inclusion of this file.
---------------------------------------------------------------------------*/

#pragma once
#ifndef HD6301_H
#define HD6301_H

#include <SSE.h>
#include <parameters.h>
#include <conditions.h>
#include <stdio.h>
#include <sys/types.h>

#if defined(SSE_HD6301_LL)

// variables that Steem must see
extern COUNTER_VAR cycles_run; 

// functions used by Steem
BYTE* hd6301_init();
int hd6301_destroy(); // like a C++ destructor
int hd6301_reset(int Cold); 
int hd6301_run_cycles(COUNTER_VAR to_m68_cycle);
int hd6301_load_save(int one_if_save, unsigned char *buffer); // for snaphot
int hd6301_receive_byte(u_char byte_in); // just passing through
WORD hd6301_peek(int addr);
void hd6301_poke(int addr, BYTE value);
BYTE hd6301_check_for_tx_byte();
BYTE hd6301_read_tx_byte();

#define MOUSE_MASK 0x33333333 // 20bit on real HW?

extern unsigned int mouse_x_counter;
extern unsigned int mouse_y_counter;

#define USE_PROTOTYPES 

#if defined(BCC_BUILD)//defined in makefile
#pragma warn- 8008
#pragma warn- 8019 // code has no effect
#pragma warn- 8045
#pragma warn- 8060
#pragma warn- 8061
#pragma warn- 8065
#pragma warn- 8066
#pragma warn- 8070
#pragma warn- 8071
#endif


// for use by Steem Debugger
#if defined(SSE_DEBUGGER)
extern 
#ifdef __cplusplus
"C" 
#endif
int hd6301_dump_ram();
extern 
#ifdef __cplusplus
"C" 
#endif
int hd6301_copy_ram(unsigned char *ptr);
#endif//dbg

#endif //#if defined(SSE_HD6301_LL)

#endif // HD6301_H
