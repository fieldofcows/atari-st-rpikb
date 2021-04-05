/* <<<                                  */
/* Copyright (c) 1994-1996 Arne Riiber. */
/* All rights reserved.                 */
/* >>>                                  */

/*
 *  Memory access
 */
#ifdef unix
#include <malloc.h>
#endif

#ifdef __MSDOS__
#include <alloc.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>

#include "defs.h"   /* general definitions */
#include "chip.h"   /* chip specific: NIREGS */
#include "ireg.h"   /* ireg_getb/putb_func[], ireg_start/end */

#include "memory.h"

/*
 * Memory variables
 *
 * Addresses used by mem_getb/putb should be set with command line options.
 * Disabling internal regs can be done by declaring intreg_start > intreg_end
 */
u_int ram_start;    /* 0x0000; */
u_int ram_end;    /* 0xFFFF; */
u_char  *ram=0;     /* was [65536]; modified for MSDOS compilers */
#if !defined(SSE_IKBD_6301_DISABLE_BREAKS)
u_char *breaks;     /* Physical storage for breakpoints */
int    break_flag;    /* Non-zero if an address containing a
           breakpoint has been accessed by mem_xxx */
u_int  break_addr;    /* Last breakpoint address accessed */
#endif

/*
 * mem_init - initialize memory area
 */
u_char *
mem_init ()
{
#if defined(SSE_IKBD_6301_MINIRAM)
  u_int size = (256+4096);
#else
  u_int size = MEMSIZE; /* MEMSIZE also hard coded in command.c */
#endif
  if (ram == NULL) {
    if ((ram = malloc (size)) == NULL) {
      perror ("Couldn't allocate ram");
      return NULL;
    }
    //printf("6301: ram %d allocated OK\n",size); //SS
    TRACE("6301: ram %d allocated OK\n",size); //SS
    ram_start = 0;
#if defined(SSE_IKBD_6301_MINIRAM)
    ram_end   = MEMSIZE - 1;
#else
    ram_end   = size - 1;
#endif
    memset (ram, 0, 256);
  } else {
    printf ("ram already allocated\n");
  }
#if !defined(SSE_IKBD_6301_DISABLE_BREAKS)
  /*
   * This is done here since ram and breaks are sisters
   */
  if (breaks == NULL) { // it seemed to count on it = 0?
    /*
     * First time initialization.
     * Allocate space for breakpoints storage
     */
    if (!(breaks = malloc (size))) {
      perror ("Couldn't allocate breaks");
      exit (errno);
    }
    printf("init breaks\n");
    memset (breaks, 0, size);
  } else {
    printf ("breaks already allocated\n");
  }
#endif
  return ram;
}
 

/*
 * mem_inramrom - returns 1 if valid ram or rom address
 *
 * Used by mem print routines
 */

static
mem_inramrom (addr)
  u_int addr;
{
  return 1;
}

/* 
 * mem_print_ascii - print ram/rom memory area in ascii
 */
mem_print_ascii (startaddr, nbytes)
  u_int startaddr;
  u_int nbytes;
{
  u_int i, addr, value;

  for (i = 0, addr = startaddr; i < nbytes; i++, addr++)
    if (mem_inramrom (addr))
#ifdef SSE_HD6301_LL
      if (isprint(value = ((addr<NIREGS)?iram[addr]:ram[addr]) ))
#else
      if (isprint(value = mem_getb (addr)))
#endif
        putchar (value);
      else
        putchar ('.');
}

/*
 * mem_print - CPU independent memory dump (print only ram/rom area)
 */
mem_print (startaddr, nbytes, nperline)
  u_int startaddr;
  u_int nbytes;
  u_int nperline;
{
  u_int i, j, addr;

  for (i = 0, addr = startaddr; i < nbytes; )
  {
    printf ("%04x\t", addr);
    for (j = 0; j < nperline; i++, j++, addr++)
      if (i >= nbytes)
        printf ("   ");
      else if(mem_inramrom(addr))
#ifdef SSE_HD6301_LL
      {
        if(addr<NIREGS)
          printf ("%02x ", iram[addr]);
        else
          printf ("%02x ", ram[addr]);
      }
#else
        printf ("%02x ", mem_getb (addr));
#endif
      else
        printf ("-- ");
    putchar (' ');
    mem_print_ascii (addr - nperline, (nbytes < nperline) ? nbytes : nperline);
    putchar ('\n');
  }
  return addr;
}

