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
#if defined(SSE_IKBD_6301_DISASSEMBLE_ROM) // disassemble ST's 6301 rom
  dump_rom();
#endif
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

#ifndef RPI
#if defined(SSE_HD6301_LL)

  // byte has just been received by ACIA?
  if(!acia[ACIA_IKBD].LineRxBusy && !(iram[TRCSR]&TDRE))
    acia[ACIA_IKBD].LineRxBusy=3; // indicates it's time to send byte in TDR
  // first byte of a series to be sent?
  else if(acia[ACIA_IKBD].LineRxBusy==2 && cpu.ncycles-Ikbd.time_of_tdr_to_tdrs>=0)
    acia[ACIA_IKBD].LineRxBusy=3; // indicates it's time to send byte in TDR

  // send byte to ACIA now?
  if(acia[ACIA_IKBD].LineRxBusy==3)
  {
    Ikbd.tdrs=Ikbd.tdr; // move TDR to TDRS
    iram[TRCSR]|=TDRE; // set TDRE
    TRACE("6301 TDR->TDRS %X\n",Ikbd.tdrs);
    keyboard_buffer_write(Ikbd.tdrs); // call Steem's ikbd function
    //ASSERT(acia[ACIA_IKBD].LineRxBusy==1);
  }
#endif
#endif

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

#ifdef SSE_DEBUG
/*
 * instr_print - print (unassemble) an instruction
 */
instr_print (addr)
  u_short addr;
{
  u_short    pc = addr;
#ifdef SSE_HD6301_LL
  u_char     op = pc<256?ram[pc]:mem_getb (pc);
#else
  u_char     op = mem_getb (pc);
#endif
  struct opcode *opptr  = &opcodetab[op];
  char    *symname;

  /*
   * In case somebody changes the opcodemap, verify it
   */

  ASSERT (opptr->op_value == op);

  if(pc==mem_getw(0xFFF0)) // SS
    TRACE("\nSCI interrupt start\n");

  if(pc==mem_getw(0xFFF4)) // SS
    TRACE("\nOCF interrupt start\n");

  printf ("%04x\t", pc);

  if (opptr->op_n_operands == 0)
  {
    printf ("%02x\t\t",  mem_getb (pc));
    printf (opptr->op_mnemonic, mem_getb (pc + 1));
  }
  else if (opptr->op_n_operands == 1)
  {
    printf ("%02x %02x\t\t",  mem_getb (pc), mem_getb (pc+1));
    printf (opptr->op_mnemonic, mem_getb (pc + 1));
    if (symname = sym_find_name (mem_getb (pc + 1)))
      printf ("\t%s", symname);
    // SS: branches, compute handy absolute addresses
    if(opptr->op_value>=0x20 && opptr->op_value<=0x2F
      ||opptr->op_value==0x8d)
    {
      int disp=mem_getb (pc + 1);
      if(disp&0x80) // negative displacement
        disp=-(0xFF-disp+1);
      TRACE(" (%X)", pc+2+disp);
    }
  }
  else
  {
    printf ("%02x %02x %02x\t", mem_getb (pc),
      mem_getb (pc + 1), mem_getb (pc + 2));
    printf (opptr->op_mnemonic, mem_getw (pc + 1));
    if (symname = sym_find_name (mem_getw (pc + 1)))
      printf ("\t%s", symname);
  }
  putchar('\n');
  if(opptr->op_value==0x39 || opptr->op_value==0x3b)
    putchar('\n');
  return opptr->op_n_operands + 1;
}
#endif
