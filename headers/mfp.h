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
FILE: mfp.h
DESCRIPTION: Declarations for Steem's high-level emulation of the infamous
Motorola MC68901 MFP (Multi Function Processor).
TODO: move some variables to the Mfp object
struct TMC68901IrqInfo, TMC68901
---------------------------------------------------------------------------*/

#pragma once
#ifndef MFP_DECLA_H
#define MFP_DECLA_H

#include "emulator.h"
#include "steemh.h"
#include "binary.h"
#include "options.h"
#include "debug.h"


// bad imitation of a normal distribution
#define TB_TIME_WOBBLE ( ((rand()&6)+(rand()&6)+(rand()&6)+(rand()&6))/(4*TICKS8)  )

enum EMfp {
  // Registers
  MFPR_GPIP=0, // ff fa01 MFP General Purpose I/O
  MFPR_AER=1, // ff fa03 MFP Active Edge
  MFPR_DDR=2, // ff fa05 MFP Data Direction
  MFPR_IERA=3, // ff fa07 MFP Interrupt Enable A
  MFPR_IERB=4, // ff fa09 MFP Interrupt Enable B
  MFPR_IPRA=5, // ff fa0b MFP Interrupt Pending A
  MFPR_IPRB=6, // ff fa0d MFP Interrupt Pending B
  MFPR_ISRA=7, // ff fa0f MFP Interrupt In-Service A
  MFPR_ISRB=8, // ff fa11 MFP Interrupt In-Service B
  MFPR_IMRA=9, // ff fa13 MFP Interrupt Mask A
  MFPR_IMRB=10, // ff fa15 MFP Interrupt Mask B
  MFPR_VR=11, // ff fa17 MFP Vector
  MFPR_TACR=12, // ff fa19 MFP Timer A Control
  MFPR_TBCR=13, // ff fa1b MFP Timer B Control
  MFPR_TCDCR=14, // ff fa1d MFP Timers C and D Control
  MFPR_TADR=15, // ff fa1f  MFP Timer A Data
  MFPR_TBDR=16, // ff fa21  MFP Timer B Data
  MFPR_TCDR=17, // ff fa23  MFP Timer C Data
  MFPR_TDDR=18, // ff fa25  MFP Timer D Data
  // RS232
  MFPR_SCR=19, // ff fa27 MFP Sync Character
  MFPR_UCR=20, // ff fa29 MFP USART Control
  MFPR_RSR=21, // ff fa2b MFP Receiver Status
  MFPR_TSR=22, // ff fa2d MFP Transmitter Status
  MFPR_UDR=23, // ff fa2f MFP USART Data
  // Interrupts
  MFP_INT_MONOCHROME_MONITOR_DETECT=15,
  MFP_INT_RS232_RING_INDICATOR=14,
  MFP_INT_TIMER_A=13,
  MFP_INT_RS232_RECEIVE_BUFFER_FULL=12,
  MFP_INT_RS232_RECEIVE_ERROR=11,
  MFP_INT_RS232_TRANSMIT_BUFFER_EMPTY=10,
  MFP_INT_RS232_TRANSMIT_ERROR=9,
  MFP_INT_TIMER_B=8,
  MFP_INT_FDC_AND_DMA=7,
  MFP_INT_ACIA=6,
  MFP_INT_TIMER_C=5,
  MFP_INT_TIMER_D=4,
  MFP_INT_BLITTER=3,
  MFP_INT_RS232_CTS=2,
  MFP_INT_RS232_DCD=1,
  MFP_INT_CENTRONICS_BUSY=0,
  // GPIP bits
  MFP_GPIP_CENTRONICS_BIT=0,
  MFP_GPIP_DCD_BIT=1,
  MFP_GPIP_CTS_BIT=2,
  MFP_GPIP_BLITTER_BIT=3,
  MFP_GPIP_ACIA_BIT=4,
  MFP_GPIP_FDC_BIT=5,
  MFP_GPIP_RING_BIT=6,
  MFP_GPIP_MONO_BIT=7,

  MFP_GPIP_CENTRONICS_MASK=BIT_0,
  MFP_GPIP_DCD_MASK=BIT_1,
  MFP_GPIP_CTS_MASK=BIT_2,
  MFP_GPIP_BLITTER_MASK=BIT_3,
  MFP_GPIP_ACIA_MASK=BIT_4,
  MFP_GPIP_FDC_MASK=BIT_5,
  MFP_GPIP_RING_MASK=BIT_6,
  MFP_GPIP_MONO_MASK=BIT_7,

  // TIMERS
  MFP_TIMER_A=0,MFP_TIMER_B,MFP_TIMER_C,MFP_TIMER_D,
  MFP_TIMER_DELAY_MASK=7,MFP_TIMER_EVENT_COUNT=8
};

#if defined(SSE_DEBUG) || defined(BCC_BUILD)
extern char* mfp_reg_name[]; //3.8.2
#endif

extern BYTE mfp_gpip_no_interrupt;

#define MFP_GPIP_COLOUR     0x80
#define MFP_GPIP_NOT_COLOUR 0x7F
#define MFP_CLK 2451
#define MFP_CLK_EXACT Mfp.xtal // it is used only in rs232
#define MFP_S_BIT (Mfp.reg[MFPR_VR] & BIT_3)

extern double CpuMfpRatio;
#if defined(SSE_CPU_MFP_RATIO_OPTION)
extern DWORD CpuCustomHz;
#endif

#pragma pack(push, 8)


struct TMC68901IrqInfo {
  unsigned int IsGpip:1;  // bad decision as sizeof is 4, but this struct is only really
  unsigned int IsTimer:1; // used by Stats report anyway, so performance doesn't matter
  unsigned int Timer:4;
};


struct TMC68901 {
  // FUNCTIONS
  TMC68901();
  void Restore();
  void Reset(bool Cold);
  int UpdateNextIrq(COUNTER_VAR at_time);
  void ComputeNextTimerB();
  // DATA
  COUNTER_VAR IrqSetTime;//unused - kept for snapshot compatibility
  COUNTER_VAR IackTiming;//unused
  COUNTER_VAR WriteTiming;//unused
  double Period[4]; // to record the period as double (debug use only for now)
  WORD IPR;//unused
  char Wobble[4]; // can be negative
  BYTE Counter[4],Prescale[4]; // to hold real values
  BYTE LastRegisterWritten;
  BYTE LastRegisterFormerValue;
  BYTE LastRegisterWrittenValue;
  BYTE Vector;//unused
  char NextIrq; //-1 reset
  char LastIrq;
  char IrqInService;//unused
  bool Irq;  // asserted or not, problem, we're not in real-time
  bool WritePending;
  TMC68901IrqInfo IrqInfo[16];
  //400
  BYTE reg[24]; // 24 directly addressable internal registers, each 8bit
  WORD *ier,*ipr,*isr,*imr; // for word access //we're little endian!!!
  COUNTER_VAR time_of_last_tb_tick;
  int nTimeouts[4];//debug
  BYTE tbctr_old;//another hack
  //402
  void calc_interrupts_enabled();
  void calc_timers_enabled();
  void calc_timer_period(int t);
  inline void interrupt_pend(int irq,COUNTER_VAR when_set);
  void UpdateTimerControls();
  inline BYTE GetTimerControlRegister(int n);
  BYTE timer_control[4]; // meta
  BYTE gpip_buffer; // looks like this thing should be persistent
  //410
  inline BYTE RegAorB(int irq);
  inline BYTE IrqBit(int irq);
  void SetTimerReg(int const n,BYTE const lobyte);
  int CalcTimerCounter(int ti,COUNTER_VAR t);
  void InitTimers();
  void Interrupt(int irq);
#ifdef DEBUG_BUILD
  bool SetPending(int irq,COUNTER_VAR when_set);
#else
  void SetPending(int irq,COUNTER_VAR when_set);
#endif
  void TimerATick();
  int SyncXtal(COUNTER_VAR system_time);
  int xtal;
  void UpdateXtal();
  int timer_period_xtal[4]; // to record the timeout interval in xtal cycles
};

#pragma pack(pop)


extern const BYTE mfp_timer_irq[4];
extern const BYTE mfp_gpip_irq[8];
extern const int mfp_timer_prescale[16];
extern int mfp_timer_counter[4];
extern COUNTER_VAR mfp_timer_timeout[4];
extern COUNTER_VAR sys_time_of_first_mfp_tick;
extern bool mfp_timer_enabled[4];
extern bool mfp_timer_check[4];
extern int mfp_timer_period[4];
extern int mfp_timer_period_fraction[4];
extern int mfp_timer_period_current_fraction[4];
extern bool mfp_timer_period_change[4];
extern bool mfp_interrupt_enabled[16];
void mfp_gpip_set_bit(int bit,bool set);

#define BYTE_00_TO_256(x) ( (int) ((unsigned char) (( (unsigned char)x )-1))  +1 )
#define MFP_IRQ ( *Mfp.ier & *Mfp.ipr & *Mfp.imr & (~*Mfp.isr) )

extern BYTE interrupt_i_bit[17];
extern BYTE interrupt_i_ab[17];

BYTE TMC68901::RegAorB(int irq) {
  return interrupt_i_ab[irq];
}

BYTE TMC68901::IrqBit(int irq) {
  return interrupt_i_bit[irq];
}

BYTE TMC68901::GetTimerControlRegister(int n) {
#ifndef SSE_LEAN_AND_MEAN
  n&=3;
#endif
  return timer_control[n];
}


void TMC68901::interrupt_pend(int irq,COUNTER_VAR when_set) {
  if(mfp_interrupt_enabled[irq])
  {
    SetPending(irq,when_set);
  }
}

#endif//MFP_DECLA_H
