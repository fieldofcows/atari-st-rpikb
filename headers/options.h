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

DOMAIN: GUI
FILE: options.h
DESCRIPTION: Declarations for Steem's option dialog (TOptionBox) that allows
the user to change Steem's many options to their heart's delight.
struct TOptionBox, TOption, TConfig
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSEOPTION_H
#define SSEOPTION_H
/*  options defines structures TOption (player choices) and TConfig 
    (internal use).
    This file is seen by 6301.c
*/

#include "SSE.h" 

#ifdef RPI

// RDH: Need to investigate what this does
#define OPTION_BATTERY6301 0

typedef struct {
  BYTE Port0Joy;
} RPIConfig;

#if defined(__cplusplus)
extern "C" RPIConfig SSEConfig;
#else
extern RPIConfig SSEConfig;
#endif

#endif // RPI

#endif//#ifndef SSEOPTION_H
