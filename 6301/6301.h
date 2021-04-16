/*
 * Atari ST Raspberry Pi IKDB Emulator
 * Copyright (C) 2021 Roy Hopkins
 * 
 * This file has been copied and modified from the Steem SSE project.
 * The original copyright notice has been retained below this one.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

unsigned int _rotr(unsigned int Data, unsigned int Bits);
unsigned int _rotl(unsigned int Data, unsigned int Bits);

typedef int64_t COUNTER_VAR;
typedef unsigned char BYTE;
typedef unsigned short WORD;

// variables that Steem must see
extern COUNTER_VAR cycles_run; 

// functions used by Steem
BYTE* hd6301_init();
int hd6301_destroy(); // like a C++ destructor
int hd6301_reset(int Cold); 
void hd6301_run_clocks(COUNTER_VAR clocks);
int hd6301_receive_byte(u_char byte_in); // just passing through
void hd6301_tx_empty(int empty);
int hd6301_sci_busy();

#define MOUSE_MASK 0x33333333 // 20bit on real HW?

extern unsigned int mouse_x_counter;
extern unsigned int mouse_y_counter;

extern int crashed;

#define USE_PROTOTYPES 

#ifdef __cplusplus
}                       // extern "C" {
#endif

#endif // HD6301_H
