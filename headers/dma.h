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
FILE: dma.h
DESCRIPTION: Declarations for the high-level emulation of the DMA (Direct
Memory Access) chip.
struct TDma
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSEDMA_H
#define SSEDMA_H

#define DMA_ADDRESS_IS_VALID_R (dma_address<himem)
#define DMA_ADDRESS_IS_VALID_W (dma_address<himem && dma_address>=MEM_FIRST_WRITEABLE)


#pragma pack(push, 8)

struct TDma {
/*
    ff 8606   R       |-------------xxx|   DMA Status (Word Access)
                                    |||
                                    || ----   _Error Status (1=OK)
                                    | -----   _Sector Count Zero Status
                                     ------   _Data Request Inactive Status

    ff 8606   W       |-------xxxxxxxx-|   DMA Mode Control (Word Access)
                              ||||||||     0  Reserved (0)
                              ||||||| -----1  A0 lines of FDC/HDC
                              |||||| ------2  A1 ................ 
                              ||||| -------3  HDC (1) / FDC (0) Register Select
                              |||| --------4  Sector Count Register Select
                              |||0         5  Reserved (0)
                              || ----------6  Disable (1) / Enable (0) DMA
                              | -----------7  FDC DRQ (1) / HDC DRQ (0) 
                               ------------8  Write (1) / Read (0)
   See notes in dma.cpp, not all bits are used.
*/
  // ENUM
  enum EDma {SR_DRQ=BIT_2,SR_COUNT=BIT_1,SR_NO_ERROR=BIT_0,
    CR_WRITE=BIT_8,CR_DRQ_FDC_OR_HDC=BIT_7,CR_DISABLE=BIT_6/*RESERVED*/,
    CR_COUNT_OR_REGS=BIT_4,CR_HDC_OR_FDC=BIT_3/*CS INT 0 EXT 1*/,CR_A1=BIT_2,
    CR_A0=BIT_1,CR_RESERVED=(BIT_5|BIT_0)};
  // FUNCTIONS
  TDma();
  void AddToFifo(BYTE data);
  BYTE GetFifoByte();
  void IncAddress();
  void Drq(BYTE manager);
  void RequestTransfer(BYTE bufnum);
  void TransferBytes();
  // DATA
  BYTE Fifo[2][16];
  COUNTER_VAR last_act;// (debug build)
  WORD mcr; // mode control register
  WORD Counter; // DMA Sector Count Register (16 bit word wide, only the least
                // significant byte of which is currently implemented).
  WORD ByteCount; // 1-512 for sectors
  WORD Datachunk; // to check # 16byte parts (unused)
  BYTE sr; // status register
  bool Request;
  BYTE BufferInUse;
  BYTE Fifo_idx[2]; // v410, note full struct wasn't L/S in snapshot before
};

#pragma pack(pop)

#endif//SSEDMA_H
