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
FILE: device_map.h
DESCRIPTION: Declarations for device read/write through the bus.
---------------------------------------------------------------------------*/

#pragma once
#ifndef IORW_DECLA_H
#define IORW_DECLA_H

#include <conditions.h>
#include <data_union.h>

/*
               The ST I/O space ranges from ff 0000 to ff  ffff,  with
          MC68000 and MC6800 peripheral internal registers starting at
          ff fa00 and ff fc00 respectively.
*/

WORD io_read(MEM_ADDRESS addr);
BYTE io_read_b(MEM_ADDRESS addr);
void io_write(MEM_ADDRESS addr, DU16 io_src_w);
void io_write_b(MEM_ADDRESS addr,BYTE io_src_b);
void io_write_w(MEM_ADDRESS addr,WORD io_src_w);

#endif//IORW_DECLA_H
