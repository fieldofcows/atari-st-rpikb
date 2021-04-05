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

DOMAIN: Rendering
FILE: palette.h
DESCRIPTION: Declarations for ST and PC palette.
---------------------------------------------------------------------------*/

#pragma once
#ifndef PALETTE_DECLA_H
#define PALETTE_DECLA_H

#include "conditions.h"
#include "parameters.h"


void palette_copy();
void palette_flip();
void palette_convert_all();
void palette_convert(int n);
void palette_prepare(bool get);
void palette_remove();
void make_palette_table(int brightness,int contrast);

extern long palette_table[4096]; // SS 16K!
extern long logpal[257];
extern BYTE palette_exec_mem[64+PAL_EXTRA_BYTES];
extern short col_brightness,col_contrast;
extern char *rgb_txt[3];
extern short col_gamma[3]; // RGB  // gcc gamma() reserved! -> col_gamma
extern int palhalf;
#ifdef WIN32
extern HPALETTE winpal;
#endif
#ifdef UNIX
extern const long standard_palette[18][2];
extern Colormap colormap;
extern XColor new_pal[257];
#endif

#endif//PALETTE_DECLA_H
