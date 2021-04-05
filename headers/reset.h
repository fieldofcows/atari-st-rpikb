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
FILE: reset.h
DESCRIPTION: Declarations for the reset operations.
---------------------------------------------------------------------------*/

#pragma once
#ifndef RESET_DECLA_H
#define RESET_DECLA_H

#define RESET_COLD 0
#define RESET_CHANGESETTINGS 0
#define RESET_STOP 0
#define RESET_BACKUP 0
#define RESET_WARM BIT_0
#define RESET_NOCHANGESETTINGS BIT_1
#define RESET_NOSTOP BIT_2
#define RESET_NOBACKUP BIT_3
#define RESET_STAGE2 BIT_4 // can check GUI events anytime
#define RESET_COUNT BIT_5 // stats

void power_on();
void reset_peripherals(bool Cold);
void reset_st(DWORD flags);
void agenda_reset(int flags);

#endif//#ifndef RESET_DECLA_H
