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

DOMAIN: I/O
FILE: rs232.cpp
DESCRIPTION: Declarations for RS232 (Recommended Standard 232) serial port
emulation.
---------------------------------------------------------------------------*/

#pragma once
#ifndef RS232_DECLA_H
#define RS232_DECLA_H

#include "conditions.h"

void RS232_VBL(int=0); // parameter not used
void RS232_CalculateBaud(bool Div16,BYTE cr,bool SetBaudNow);
BYTE RS232_ReadReg(int Reg);
void RS232_WriteReg(int Reg,BYTE NewVal);

extern bool UpdateBaud;
extern BYTE rs232_recv_byte;
extern bool rs232_recv_overrun;
extern DWORD rs232_bits_per_word,rs232_hbls_per_word;

void agenda_serial_sent_byte(int);
void agenda_serial_break_boundary(int);
void agenda_serial_loopback_byte(int NewVal);

#endif//#ifndef RS232_DECLA_H
