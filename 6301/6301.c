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
FILE: 6301.c
DESCRIPTION: Code to interface Steem SSE with a modified version of
sim68xx (Copyright (c) 1994-1996 Arne Riiber) to emulate at a lower level
the HD6301V1 chip that was in the ST, as described in 'HD6301V1 on ST.txt'.
This emulation was supposed to run as terminal on a DOS PC. A precise
integration with another serial device (MC6850 ACIA in the ST) wasn't the 
focus.
The functions in original files have been modified where necessary, some
have been added, in 6301.c, and in ireg.c (most interesting ones). 
This is C, not C++. 6301.c includes other C files.
ASSERT will only work properly in the Visual Studio IDE (or equivalent).
I went out of my way to have TRACE working just like in the rest of Steem
SSE.
---------------------------------------------------------------------------*/

#include "6301.h"

#include <ikbd.h>
#include <options.h>

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

// variables from Steem (declared as extern "C") there
extern unsigned char  stick[8]; // joysticks
extern double cpu_cycles_multiplier;

// our variables that Steem must see
COUNTER_VAR cycles_run=0; 

// additional variables for our module
unsigned int mouse_x_counter;
unsigned int mouse_y_counter;

// Debug facilities
#if defined(TRACE_6301)
#define TRACE printf
#else
#define TRACE
#endif

#define ASSERT(x)
#define error printf
#define warning TRACE

#define abs_quick(i) ( (i>=0) ? (i) : -(i))

// base
#include "cpu.c"
#include "fprinthe.c"
#include "memsetl.c"
#include "symtab.c"  
//#include "tty.c"
// arch
// m68xx
#include "memory.c"
#include "opfunc.c" // will include alu.c (arithmetic/logic unit)
#include "reg.c"
// h6301
#include "instr.c"
#include "ireg.c"
#include "optab.c"
#include "sci.c"
#include "timer.c"
#include "callstac.c"

// Interface with Steem

BYTE* hd6301_init() {
  return mem_init();
}


hd6301_destroy() {
  TRACE("6301: destroy object\n");
  if(ram) 
    free(ram);
  ram=NULL;
}


hd6301_reset(int Cold) {
  TRACE("6301 emu cpu reset (cold %d)\n",Cold);
  cpu_reset();
  if(Cold)
  {
    WORD rnd=rand()%16;
    memset(ram, 0, 0x80); 
    memset(iram,0,NIREGS);// also all internal registers
    mouse_x_counter=MOUSE_MASK;
    mouse_y_counter=MOUSE_MASK;
    // internal mouse state is random: A239/Jumping Jackson, Droid SE WIP
    mouse_x_counter=_rotl(mouse_x_counter,rnd);
    mouse_y_counter=_rotl(mouse_y_counter,rnd);
    //TRACE("Mouse mask %X\n",mouse_x_counter); // 333... 666 999 CCC
    Ikbd.current_m68_cycle=0;
  }
  iram[TRCSR]=0x20;
  mem_putw (OCR, 0xFFFF);
}

 
void hd6301_run_clocks(COUNTER_VAR clocks) {
  // Called by Steem to run some cycles (per scanline or to update before IO)
  int pc;
  COUNTER_VAR starting_cycles=cpu.ncycles;

  // make sure our 6301 is running OK
  if(!cpu_isrunning())
  {
    TRACE("6301 starting cpu\n");
    cpu_start();
  }
  if(iram[TRCSR]&1)
  {
    TRACE("6301 waking up (PC %X cycles %lu)\n",reg_getpc(),cpu.ncycles);
    iram[TRCSR]&=~1;
  }
  pc=reg_getpc();

  while(!Ikbd.Crashed && ((cpu.ncycles-starting_cycles) < clocks))
  {
    instr_exec (); // execute one instruction
  }
}

hd6301_receive_byte(u_char byte_in) {
  //ASSERT(byte_in==Ikbd.rdrs);
  return sci_in(&byte_in,1);
}


hd6301_load_save(int one_if_save,unsigned char *buffer) {
  // A function to help the memory snapshot facility of Steem
  unsigned char *i=buffer; // stack on Steem's side
  //ASSERT(buffer);
  //ASSERT(ram);
  if(!ram)
    return 0; // run-time anti-crash
  // cpu registers
  if(one_if_save)
    memmove(i,&regs,sizeof(regs));
  else
    memmove(&regs,i,sizeof(regs));
  i+=sizeof(regs);
  // cpu
  if(one_if_save)
    memmove(i,&cpu,sizeof(cpu));
  else
    memmove(&cpu,i,sizeof(cpu));
  i+=sizeof(cpu);
  // ram
  if(one_if_save)
  {
    TRACE("6301 Snapshot - save RAM\n");
    memmove(i,&ram[0x80],128);
  }
  else
  {
    TRACE("Snapshot - load RAM\n");
    memmove(&ram[0x80],i,128);
  }
  i+=128;
  // iregs 
  if(one_if_save)
    memmove(i,&iram,sizeof(iram));
  else
    memmove(&iram,i,sizeof(iram));
  i+=sizeof(iram);
  return (int)(i-buffer);
}


WORD hd6301_peek(int addr) {
  int value;
  switch(addr) {
  case -1: // fake addr <0
    value=reg_getpc();
    break;
  case -2:
    value=reg_getccr();
    break;
  case -3:
    value=reg_getsp();
    break;
  case -4:
    value=reg_getaccd();
    break;
  case -5:
    value=reg_getix();
    break;
  case -6:
    value=mem_getb(reg_getpc());
    break;
  case 0: case 1: case 4: case 5: // write-only
    value=iram[addr];
    break;
  default:
    value=mem_getb (addr);
  }
  return (WORD)value;
}



void hd6301_poke(int addr, BYTE value) {
  if(addr>=0 && addr<=255)
    ram[addr] = value;
}

BYTE hd6301_check_for_tx_byte() {
  return (!(iram[TRCSR]&TDRE));
}

BYTE hd6301_read_tx_byte() {
  BYTE result = 0;
    if (!(iram[TRCSR]&TDRE)) {
        result = Ikbd.tdr;
        iram[TRCSR] |= TDRE;
    }
    return result;
}

