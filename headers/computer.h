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

FILE: computer.h
DESCRIPTION: Declarations for computer components.
struct TStats
---------------------------------------------------------------------------*/

#pragma once
#ifndef COMPUTER_H
#define COMPUTER_H

#include "conditions.h"
#include "parameters.h"
#include "glue.h"
#include "device_map.h"
#include "cpu.h"
#include "mmu.h"
#include "shifter.h"
#include "acia.h"
#include "ikbd.h"
#include "midi.h"
#include "mfp.h"
#include "stports.h"
#include "rs232.h"
#include "fdc.h"
#include "psg.h"
#include "blitter.h"
#include "reset.h"
#include "emulator.h"
#include "run.h"
#include "hd_gemdos.h"
#include "acsi.h"
#include "dma.h"
#include "floppy_drive.h"
#include "floppy_disk.h"
#include "tos.h"
#include "printer.h"
#ifdef SSE_DEBUG
#include "osd.h"
#endif

extern TGlue Glue;
extern TMC68000 Cpu;
extern TMmu Mmu;
extern TShifter Shifter;
extern TMC68901 Mfp;
extern TDma Dma;
extern TWD1772 Fdc;
extern TSF314 FloppyDrive[3];
extern TFloppyDisk FloppyDisk[3];
extern TYM2149 Psg;
extern TBlitter Blitter;
extern TMC6850 acia[2]; 
//extern "C" THD6301 Ikbd; // in ikbd.h, for 6301.c
extern TLMC1992 Microwire;
extern TTos Tos;
#if defined(SSE_MEGASTE)
extern TMegaSte MegaSte;
extern TCpu16 Cpu16;
#endif
#if defined(SSE_ACSI_LASER)
extern TSLM804 Laser;
#endif
#if defined(SSE_PRINTER)
extern TSMM804 Printer;
#endif

void ComputerRestore();

#endif//#ifndef COMPUTER_H

