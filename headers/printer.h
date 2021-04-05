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
FILE: printer.h
DESCRIPTION: Declarations for Steem's printer emulation
struct TSMM804, TSLM804, TSLM804Param
---------------------------------------------------------------------------*/
#pragma once
#ifndef STPRINTER_DECLA_H
#define STPRINTER_DECLA_H

#include <portio.h>
#include <stports.h>
#include <mymisc.h>
#include <data_union.h>

#pragma pack(push, 8)

#if defined(SSE_PRINTER)

extern TSTPort STPort[NSTPORTS];

struct TSMM804 { // SMM804 is Atari's Epson-compatible dot matrix printer
  enum EEpson {US,FR,DE,UK,DKI,SW,IT,SPI,JP,NW,DKII,SPII,LAT,KO,LEG=64};
  TSMM804(),~TSMM804();
  void Close(FILE *fp=NULL);
  void Bold(BYTE p),Italics(BYTE p),Underline(BYTE p); // to set on/off
  void HandleByte(BYTE Byte);
  void Reset();
  void StartRtf(FILE *fp_rtf); // to spare code
  pbm_image pbm_descr;
  FILE *fp_rtf,*fp_pbm;
  short GraphicColumns,GraphicBand;
  int GraphicsIdx;
  BYTE Command[34-16];
  //BYTE HTab[32],HPos;
  BYTE VTab[16],VPos;
  BYTE CommandParameter;
  BYTE nParameters;
  BYTE International; // see EEpson
  BYTE Colour; // for "light" on B/W printer
  BYTE LastChar;
  bool ReceivingCommand;
  bool bItalics,bBold,bUnderline,bSubscript,bSuperscript;
};

#endif//#if defined(SSE_PRINTER)


#if defined(SSE_ACSI_LASER)

#define NLASER_PARAMETERS 37 //?

struct TSLM804Param {
  static void copy_config_to_buffer(TSLM804Param *config,BYTE *buffer);
  static void copy_buffer_to_config(TSLM804Param *config,BYTE *buffer);  
  BYTE nbytes;
  DU16 pageh;
  DU16 pagew;
  DU16 marginh;
  DU16 marginw;
  BYTE feed;
  DU16 resh;
  DU16 resw;
  BYTE timeout;
  DU16 scantime;
  DU16 pagecount;
  DU16 inputcap;
  DU16 outputcap;
  BYTE duplex;
};


struct TSLM804 {
  TSLM804();
  void Reset();
  int bytes_per_page();
  BYTE get_fifo_byte();
  void fill_fifo();
  TSLM804Param Param;
  int line_bytes,page_bytes;
  int no_dma; // dbg
  int print_idx,bytes_to_transfer;
  BYTE fifo[2];
  BYTE controller_buffer[64],buffer_idx;
  BYTE pages_to_print;
  bool busy;
};

#endif//#if defined(SSE_ACSI_LASER)

#pragma pack(pop)

#endif//#ifndef STPRINTER_DECLA_H
