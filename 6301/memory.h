
/*
 *  Memory access
 *
 *  This file contains memory accesss functions that typically vary
 *  with chip and board. As is, it provides support for a single I/O
 *  block through the 'ireg' interface.
 *
 *  For additional I/O (or special memory) blocks, 'mem_getb' will 
 *  probably have to be modified.
 *
 *  Functions reside in this file to make inlining possible.
 */
#ifndef MEMORY_H
#define MEMORY_H

#include "defs.h"   /* general definitions */
#include "chip.h"   /* chip specific: NIREGS */
#include "ireg.h"   /* chip specific: ireg_getb/putb_func[], ireg_start/end */

#define MEMSIZE 65536   /* Size of ram and breakpoint arrays */
/*
 * Memory variables
 *
 * Addresses used by mem_getb/putb should be set with command line options.
 * Disabling internal regs can be done by declaring ireg_start > ireg_end
 */
extern u_int  ram_start;  /* First valid RAM address */
extern u_int  ram_end;  /* Last valid RAM address */
extern u_char  *ram;    /* Physical storage for simulated RAM */

/*
 * The array 'breaks' is a sister array to 'ram'
 * where each address containing nonzero is a breakpoint.
 *
 * 'break_flag' is set if the breakpoint array 'breaks[i]' is set
 * for address 'i'.
 * There is currently no breaks_start and breaks_end variables,
 * so another start value than 0 will not work.
 */
#if !defined(SSE_IKBD_6301_DISABLE_BREAKS)
extern u_char *breaks;    /* Physical storage for breakpoints */
extern int    break_flag; /* Non-zero if an address containing a
           breakpoint has been accessed by mem_xxx */
extern u_int  break_addr; /* Last breakpoint address accessed */
#endif

/*
 *  mem_getb - called to get a byte from an address
 */
static u_char
mem_getb (addr)
  u_int addr;
{
  int offs = addr - ireg_start;
#if !defined(SSE_IKBD_6301_DISABLE_BREAKS)
  if (breaks[addr]) {
    break_flag = 1; /* Signal execution loop to stop */
    break_addr = addr;
  }
#endif
  //ASSERT(!ireg_start);
  if (offs >= 0 && offs < NIREGS) {
    if(offs==DDR1||offs==DDR2||offs==DDR3||offs==DDR4
      ||offs==TDR) 
    {
#if !defined(NDEBUG)
    if(reg_getpc()!=0xB5)
      TRACE("NOTE PC %X reading WO register %X\n",reg_getpc(),addr);
#endif
      return 0xFF; // as in HD6301V1 doc
    }
    if (ireg_getb_func[offs])
    {  
      return (*ireg_getb_func[offs])(offs);    
    } else
      return iram[offs];
    } else if (addr >= ram_start && addr <= ram_end) {
#if defined(SSE_IKBD_6301_MINIRAM)
      if(addr>=0xF000)
        return ram[addr-0xF000+256];
      else if(addr<0x80||addr>=256)
        return 0xff; // error
      else
#endif
    return ram[addr];
  } else {
#if defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
    error ("mem_getb: addr=%04x\n",addr);
#else
    error ("mem_getb: addr=%04x, subroutine %04x\n",
      addr, callstack_peek_addr ());
#endif
    return 0;
  }
}

static u_short
mem_getw (addr)
  u_int addr;
{
  /* Make sure hi byte is accessed first */
  u_char hi = mem_getb (addr);
  u_char lo = mem_getb (addr + 1);
  return (hi << 8) | lo;
}

/*
 * mem_putb - called to write a byte to an address
 */ 
static void
mem_putb (addr, value)
  u_int   addr;
  u_char  value;
{
  int offs = addr - ireg_start; /* Address of on-chip memory */
  //ASSERT(addr != 0x83);
#if !defined(SSE_IKBD_6301_DISABLE_BREAKS)
  if (breaks[addr]) {
    break_flag = 1; /* Signal execution loop to stop */
    break_addr = addr;
  }
#endif
  if (offs >= 0 && offs < NIREGS) {
    if(offs==RDR||offs==FRC||offs==ICR)
    {
#if !defined(NDEBUG)
      TRACE("NOTE PX %X writing %X on RO register %X\n",reg_getpc(),addr, value);
#endif
      return;
    }
    if (ireg_putb_func[offs])
      (*ireg_putb_func[offs])(offs, value);
    else
      iram [offs] = value;
  } else if (addr >= ram_start && addr <= ram_end) {
#if defined(SSE_IKBD_6301_MINIRAM)
      if(addr>=0xF000)
        ram[addr-0xF000+256]=value;
      else if(addr<0x80||addr>=256)
        ; // error
      else
#endif
    ram [addr] = value;
  } else {
#if defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
    error ("mem_putb: addr=%04x\n", addr);
#else
    error ("mem_putb: addr=%04x, subroutine %04x\n",
      addr, callstack_peek_addr ());
#endif
  }
}

static void
mem_putw (addr, value)
  u_int addr;
  u_int value;
{
  mem_putb (addr, value >> 8);    /* hi byte */
  mem_putb (addr + 1, value & 0xFF);  /* lo byte */
}

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif


/* memory.c */
extern u_char *mem_init        P_(());
extern int     mem_print_ascii P_((u_int startaddr, u_int nbytes));
extern int     mem_print       P_((u_int startaddr, u_int nbytes, u_int nperline));

extern int mem_free P_(());
#undef P_

#endif
