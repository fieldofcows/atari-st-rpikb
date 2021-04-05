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

#ifdef UNIX
unsigned int _rotr(unsigned int Data, unsigned int Bits) {
  return ((Data >> Bits) | (Data << (32-Bits)));
}
unsigned int _rotl(unsigned int Data, unsigned int Bits) {
  return ((Data << Bits) | (Data >> (32-Bits)));
}

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

#endif

#ifdef VC_BUILD
#pragma warning (disable : 4013 4100 4127 4131 4431 4245 4706 4710 4716)
#pragma warning (disable : 28278)
#endif


// variables from Steem (declared as extern "C") there
extern unsigned char  stick[8]; // joysticks
extern double cpu_cycles_multiplier;
extern BYTE ST_Key_Down[128];
extern int mousek;

// our variables that Steem must see
COUNTER_VAR cycles_run=0; 

// additional variables for our module
unsigned char rec_byte;
unsigned int mouse_x_counter;
unsigned int mouse_y_counter;

// Debug facilities
// ASSERT
#undef ASSERT
#if defined(_MSC_VER) && defined(_DEBUG)
#if defined(SSE_X64_DEBUG)
#define ASSERT(x) {if(!(x)) {TRACE("Assert failed: %s\n",#x);\
                  DebugBreak();}}
#else
#define ASSERT(x) {if(!(x)) {TRACE("Assert failed: %s\n",#x);\
                   _asm{int 0x03}}}
#endif
#else 
#if !defined(NDEBUG) ||(defined(VC_BUILD) && defined(DEBUG_BUILD))
#define ASSERT(x) {if(!(x)) TRACE("Assert failed: %s\n",#x);} //just a trace
#else
#define ASSERT(x) // release
#endif
#endif
// TRACE
#undef TRACE
#if defined(SSE_DEBUG) 
// we use this trick because Trace is a struct function
// it goes to Trace.txt or the IDE
// TRACE here is the equivalent of TRACE_LOG in Steem SSE
void (*hd6301_trace)(char *fmt,...);
#undef LOGSECTION
#define TRACE Debug.LogSection=LOGSECTION_IKBD,hd6301_trace
#elif defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE printf
#else
#define TRACE printf
#endif

//#define SSE_IKBD_6301_ROM_KEYTABLE //array vs. code...
// don't undef, Steem counts on that, it just marks the mods:
#define SSE_IKBD_6301_DISABLE_BREAKS // to save 64k RAM (we still consume 64k)
#define SSE_IKBD_6301_DISABLE_CALLSTACK // to save 3k on the PC stack
#define SSE_IKBD_6301_MINIRAM // save close to 60k, at last

// constructing our module (OBJ) the good old Steem way, hem

#ifdef MINGW_BUILD
#pragma GCC diagnostic ignored "-Wreturn-type"
#define __max(a,b) ((a)>(b) ? (a):(b))
#define __min(a,b) ((a)>(b) ? (b):(a))
#endif

#define abs_quick(i) ( (i>=0) ? (i) : -(i))

#define error printf // saves headache
#define warning TRACE //printf 
// base
#if !defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
#include "callstac.c"
#endif
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

// Interface with Steem

BYTE* hd6301_init() {
  return mem_init();
}


hd6301_destroy() {
  TRACE("6301: destroy object\n");
  if(ram) 
    free(ram);
  ram=NULL;
#if !defined(SSE_IKBD_6301_DISABLE_BREAKS)
  if(breaks) 
    free(breaks);
  breaks=NULL;
#endif
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

 
hd6301_run_cycles(COUNTER_VAR to_m68_cycle) {
  // Called by Steem to run some cycles (per scanline or to update before IO)
  COUNTER_VAR cycles_to_run=(to_m68_cycle-Ikbd.current_m68_cycle)/HD6301_CYCLE_DIVISOR;
  int pc;
  COUNTER_VAR starting_cycles=cpu.ncycles;
  if(cycles_to_run<-256 || cycles_to_run>255)
  {
    Ikbd.current_m68_cycle=to_m68_cycle; //safety
    return 0;
  }
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
#ifndef RPI
  if(OPTION_HACKS && !(pc>=0xF000 && pc<=0xFFFF || pc>=0x80 && pc<=0xFF))
  {
    TRACE("PC out of range, resetting chip\n"); 
    reset(); // hack
  }
#endif
  while(!Ikbd.Crashed && (cycles_run<cycles_to_run))
  {
    instr_exec (); // execute one instruction
    cycles_run=cpu.ncycles-starting_cycles;
  }
  Ikbd.ChipCycles=cpu.ncycles; // HD6301 object doesn't know cpu.ncycles
  cycles_run*=HD6301_CYCLE_DIVISOR;
  Ikbd.current_m68_cycle+=cycles_run;
  cycles_run=0;
  return (int)cycles_run; //unused
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
#if !defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
  // callstack - it's a debug feature, useless for us, lots of space
  if(one_if_save)
    memmove(i,&callstack,sizeof(callstack));
  else
    memmove(&callstack,i,sizeof(callstack));
  i+=sizeof(callstack);
#endif
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


// Debug

#if defined(SSE_IKBD_6301_DISASSEMBLE_ROM)

dump_rom() {
  int i;
  TRACE("************************************************************\n");
  TRACE("* This disassembly of the Atari ST's HD6301V1 ROM was made *\n");
  TRACE("* by Sim6xxx as modified for Steem SSE                     *\n");
  TRACE("************************************************************\n\n\n");
  for(i=0xF000;i<0xFFFF;i++)
  {
    // don't decode data, + indication
    // data bytes
    if(i==0xFF6e || i==0xffed)
      mem_print (i, 1, 1);
    else if(i>=0xFF6F)
    {
      mem_print (i, 2, 2);
      i++;
    } 
    // data groups
    else if(i==0xf2f3)
    {
      TRACE("eg Scancodes are here below\n");
      mem_print (i, 0xF370-0xf2f3+1,14); 
      i+=0xF370-0xf2f3;
    }
    else if(i==0xf679)//arrows +...
    {
      mem_print (i, 7,2); 
      i+=7;
    }
    else if(i==0xf87a)//arrows
    {
      mem_print (i, 4,2); 
      i+=4-1;
    }
    else if(i==0xf930)
    {
      mem_print (i, 0xf990-0xf930,2); 
      i+=0xf990-0xf930-1;
    }
    else if(i==0xfed0)
    {
      mem_print (i, 0xfee1-0xfed0, 0xfee1-0xfed0); 
      i+=0xfee1-0xfed0;
    }
    // instructions
    else
      i+=instr_print (i)-1;
  }//nxt
}

#endif

#if defined(SSE_DEBUGGER)

hd6301_dump_ram() { // commanded by Debugger
  int i;
  printf("6301 RAM dump\n    \t00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
  mem_print (0,256,16);
  for(i=0x80;i<256;i++)
      i+=instr_print (i)-1;
}


hd6301_copy_ram(unsigned char *ptr) {
  //  We copy the memory instead of having direct access.
  int i;
  if(!ram)//during init
    return -1;
  for(i=0;i<NIREGS;i++)
    ptr[i]=iram[i];
  for(i=0x16;i<256;i++)
    ptr[i]=mem_getb (i);
  return 0;
}

#endif

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

