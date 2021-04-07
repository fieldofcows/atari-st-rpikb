/* <<<                                  */
/* Copyright (c) 1994-1996 Arne Riiber. */
/* All rights reserved.                 */
/* >>>                                  */
#include <assert.h>
#include <stdio.h>

#include "defs.h"
#include "chip.h"
#include "cpu.h"
#include "ireg.h"
#include "memory.h"
#include "optab.h"
#include "reg.h"
#include "sci.h"
#include "timer.h"
#include "THD6301.h"

#ifdef USE_PROTOTYPES
#include "instr.h"
#endif


/*
 *  reset - jump to the reset vector
 */
reset () 
{
  reg_setpc (mem_getw (RESVECTOR));
  reg_setiflag (1);
}


/*
 * instr_exec - execute an instruction
 */
instr_exec ()
{

  /*
   * Get opcode from memory,
   * inc program counter to point to first operand,
   * Decode and execute the opcode.
   */
  struct opcode *opptr;
  int interrupted = 0;    /* 1 = HW interrupt occured */

#ifndef M6800

  if (!reg_getiflag ()) 
  {
    /*
     * Check for interrupts in priority order
     */
    if ((ireg_getb (TCSR) & OCF) && (ireg_getb (TCSR) & EOCI)) {
      int_addr (OCFVECTOR);
      interrupted = 1;
    } 
    else if (serial_int ()) {
      int_addr (SCIVECTOR);
      interrupted = 1;
    }
  }
#if 0
  /*
   * 6301 Address check error: Trap if address is outside RAM/ROM
   * This must be modified to check mode (single-chip/extended) first
   */
  if ((reg_getpc() <= ramstart) || reg_getpc() >  ramend)
  {
    error ("instr_exec: Address error: %04x\n", reg_getpc());
    trap();   /* Highest pri vector after Reset */
  }
#endif /* 0 */
#endif /* M6800 */

  if (interrupted) /* Prepare cycle count for register stacking */
  {
    opptr = &opcodetab[0x3f]; /* SWI */
  }
  else
  {
    int pc=reg_getpc (); 
    if(!(pc>=0x80&&pc<0xFFFF)) // eg bad snapshot
    {
      TRACE("pc=%x, 6301 emu is hopelessly crashed!\n",pc);
      Ikbd.Crashed=1;
      return -1;
    }

    opptr = &opcodetab [mem_getb (reg_getpc ())];
    reg_incpc (1);
    (*opptr->op_func) ();
//    ASSERT(iram[7]!=0xf0);
  }
  
  cpu_setncycles (cpu_getncycles () + opptr->op_n_cycles);
  timer_inc (opptr->op_n_cycles);
  return 0;
}

