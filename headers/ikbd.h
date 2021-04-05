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
#ifndef IKBD_H
#define IKBD_H

#define TICKS8 1 //  8MHz
#define HD6301_CYCLE_DIVISOR (8*TICKS8) // the 6301 runs at 1MHz (verified by Stefan jL)

#ifdef __cplusplus 
#include "THD6301.h"
#include "6301.h"

extern "C" BYTE ST_Key_Down[128]; // used by 6301
extern "C" int mousek; 
#endif//#ifdef __cplusplus 

extern
#ifdef __cplusplus
"C"
#endif
struct THD6301 Ikbd; // 6301.c must know about it

#endif
