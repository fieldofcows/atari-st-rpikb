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
FILE: acia.h
DESCRIPTION: Declarations for high-level ACIA emulation.
struct TMC6850
acia.h must be compatible with C and with C++, it is included by 6310.c.
---------------------------------------------------------------------------*/

#pragma once
#ifndef ACIA_H
#define ACIA_H

#include <binary.h>

#ifdef __cplusplus
#include "options.h"
#ifndef RPI
#include "emulator.h"
#include "steemh.h"
#include "debug.h"
#include "options.h"
#endif
#endif

#if defined(RPI) && defined(__cplusplus)
extern "C" double cpu_cycles_multiplier; // used by 6301
#endif

enum EAcia {  
  ACIA_OVERRUN_NO=0,ACIA_OVERRUN_COMING,ACIA_OVERRUN_YES,
  ACIA_IKBD=0,ACIA_MIDI,
  ACIA_CD1=BIT_0,ACIA_CD2=BIT_1,ACIA_WS1=BIT_2,ACIA_WS2=BIT_3,
  ACIA_WS3=BIT_4,ACIA_TC1=BIT_5,ACIA_TC2=BIT_6,ACIA_RIE=BIT_7,
  ACIA_RDRF=BIT_0,ACIA_TDRE=BIT_1,ACIA_DCD=BIT_2,ACIA_RTS=BIT_3,
  ACIA_FE=BIT_4,ACIA_OVRN=BIT_5,ACIA_PE=BIT_6,ACIA_IRQ=BIT_7          
};

#pragma pack(push, 8)

#ifdef __cplusplus
extern "C" {
#endif

struct TMC6850 {
  // DATA
  // we keep the start of the old structure for snapshot compatibility
  int clock_divide;
  int rx_delay__unused;
  BYTE rx_irq_enabled;
  BYTE rx_not_read;
  int overrun;
  int tx_flag;
  BYTE tx_irq_enabled;
  BYTE data;
  BYTE irq;
  COUNTER_VAR last_tx_write_time;
  COUNTER_VAR last_rx_read_time;
  BYTE LineRxBusy;// receiveing from 6301 or MIDI
  BYTE ByteWaitingRx; // unused
  BYTE ByteWaitingTx; // unused
  BYTE LineTxBusy;// transmitting to 6301 or MIDI
  BYTE cr;    // control 
  BYTE sr;    // status
  BYTE rdr;   // receive data 
  BYTE tdr;   // transmit data
  BYTE rdrs;  // receive data shift
  BYTE tdrs;  // transmit data shift
  BYTE Id;
  COUNTER_VAR TimeRx,TimeTx;
#ifdef __cplusplus
  // FUNCTIONS
  inline bool IrqForTx() {
    return ((cr&ACIA_TC1) && !(cr&ACIA_TC2));
  }
  static void BusJam();
  bool CheckIrq();
  void TransmitTDR();
  inline int TransmissionTime() {
/*  
Compute cycles according to bits 0-1 of cr
SS1 SS0                  Speed (bit/s)
 0   0    Normal            500000
 0   1    Div by 16          31250 (ST: MIDI)   cr&1=1
 1   0    Div by 64         7812.5 (ST: IKBD)   cr&1=0
MIDI transmission is 4 times faster than IKBD
*/
    int cycles=(cr&1) ? (10*16*16) : (10*16*64); //2560 for MIDI, 10240 for 6301
    cycles*=TICKS8;
  #ifndef RPI
    if(SSEConfig.CpuBoosted)
    {
      double m=cycles*cpu_cycles_multiplier;
      cycles=(int)m;
    }
  #endif
    return cycles;
  }
#endif
};

extern struct TMC6850 acia[2]; // 6301 needs the declaration

#pragma pack(pop)

#ifdef __cplusplus
}//extern "C"

#define ACIA_CHECK_IRQ(i) {acia[i].CheckIrq(); \
  mfp_gpip_set_bit(MFP_GPIP_ACIA_BIT,!( (acia[ACIA_IKBD].sr&ACIA_IRQ) \
      || (acia[ACIA_MIDI].sr&ACIA_IRQ))); \
  }

int ACIAClockToHBLS(int ClockDivide,bool MIDI_In=false);
void ACIA_Reset(BYTE nACIA,bool Cold);
void ACIA_SetControl(int nACIA,BYTE Val);
void agenda_acia_tx_delay_IKBD(int),agenda_acia_tx_delay_MIDI(int);

#endif

#endif//#ifndef ACIA_H
