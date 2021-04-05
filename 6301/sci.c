/* <<<                                  */
/* Copyright (c) 1994-1996 Arne Riiber. */
/* All rights reserved.                 */
/* >>>                                  */
#include <stdio.h>

#include "defs.h"
#include "chip.h"
#include "cpu.h"
#include "ireg.h"
#include "sci.h"

/*
This part of the emu deals with serial I/O between the 6301 and its CPU, in
our case (ST), via the ACIA. 

Registers:
------------------------------------------------------------------------------
  * Serial Communication Interface
------------------------------------------------------------------------------
$0010    | B | RMCR  | Rate & Mode Control Register                   | RW
$0011    | B | TRCSR | Transmit/Receive Control & Status Register     | RW
$0012    | B | RDR   | Receive Data Register                          | R0
$0013    | B | TDR   | Transmit Data Register                         | W0
------------------------------------------------------------------------------
*/

/*
 * Pseudo-received data buffer used by rdr_getb() routines
 */
#if !defined(SSE_HD6301_LL)
static u_char recvbuf[BUFSIZE];
static int  rxindex      = 0; /* Index of first byte in recvbuf */
       int  rxinterrupts = 0; /* Number of outstanding rx interrupts */
       int  txinterrupts = 0; /* Number of outstanding tx interrupts */
#endif
/*
 * sci_in - input to SCI
 *
 * increment number of outstanding rx interrupts
 */

/*  This function is called by Steem when a byte sent to the 6301 is supposed
    to have been received in rdrs.
*/

sci_in (s, nbytes)
  u_char *s;
int nbytes;
{
#if defined(SSE_HD6301_LL)

#if  defined(SSE_DEBUG)
  u_char trcsr=iram[TRCSR];
//  ASSERT(nbytes==1);
//  ASSERT(trcsr&RE);
  if(trcsr&WU) // bug?
    TRACE("6301 in standby mode\n");
#endif
  // detect OVR condition, set flag at once (unlike ACIA)
  if(iram[TRCSR]&RDRF)
  {
    TRACE("6301 OVR RDR %X RDRS %X SR %X->%X\n",Ikbd.rdr,Ikbd.rdrs,iram[TRCSR],iram[TRCSR]|ORFE);
    iram[TRCSR]|=ORFE; // hardware sets overrun bit
  }
  else
  {
    Ikbd.rdr=*s;
#if defined(SSE_IKBD_RTC) && defined(SSE_IKBDI) // It is of course a dangerous hack
    if(OPTION_BATTERY6301==2)
    {
      if(Ikbd.CurrentCommand==0x1B && Ikbd.CurrentParameter==1) // TIME-OF-DAY CLOCK SET
        Ikbd.rdr-=0xA0;
    }
#endif
    ireg_putb (RDR, Ikbd.rdr);
    TRACE("6301 RDR %X\n",Ikbd.rdr);
  }
  iram[TRCSR]|=RDRF; // set RDRF

#else

  int i;
  for (i=0; i<nbytes; i++)
    if (rxinterrupts < BUFSIZE)
      recvbuf[rxinterrupts++] = s[i];
    else
      warning ("sci_in:: buffer full\n");
  rec_byte=*s;

#endif
}

#if !defined(SSE_HD6301_LL) 
sci_print ()
{
  printf ("sci recvbuf:\n");
  fprinthex (stdout, recvbuf + rxindex, rxinterrupts);
}
#endif

/*
TRCSR

This register controls the communications.
-Bit 0 [$1] (RW) : Wake Up, when set to 1, wait until ten 1 appear on the line,
              then it switches to zero.
-Bit 1 [$2] (RW) : Transmit Enable
-Bit 2 [$4] (RW) : Transmit Interrupt Enable
-Bit 3 [$8] (RW) : Receive Enable
-Bit 4 [$10] (RW) : Receive Interrupt Enable
-Bit 5 [$20] (RO) : Transmit Data Register Empty, is set to 1 when a byte has been
              sent, and is set to 0 when TRCSR is read then TDR is written.
-Bit 6 [$40] (RO) : Overrun or Framing Error, is set to 1 when a byte is received if
              the previous byte was not read, and is set to 0 when TRCSR then
              RDR are read.
-Bit 7 [$80] (RO) : Receive Data Register Full, is set to 1 when a byte have been
              received, and is set to zero when TRSCR then RDR are read.

The read-only bits are set and cleared by the hardware.

Bit 5 is read-only, but the ROM tries to set it (writing $2A,$3A,$3E)

The following 2 functions trcsr_getb and trcsr_putb are used when the program
reads or writes the status/control register $11 (often).

*/



/*
 * trcsr_getb - always return Transmit Data Reg. Empty = 1
 */

u_char trcsr_getb (offs)
  u_int offs;
{
#ifndef SSE_HD6301_LL
  char c;
#endif
  unsigned char rv;
#ifndef SSE_HD6301_LL
  /*
   * Check if user has typed a key, prevent simulator overrun
   * if data in recvbuf, return RDRF
   */
  if (!rxinterrupts && (c = tty_getkey (0))) /* Typed a key */
    sci_in (&c, 1);
#endif

  rv=ireg_getb (TRCSR);

  return rv;
}


/*
 *  trcsr_putb - enable/disable tx/rx interrupt
 *
 *  Sets global interrupt flag if tx interrupt is enabled
 *  so main loop can execute interrupt vector
 */
trcsr_putb (offs, value)
  u_int  offs;
  u_char value;
{

#ifdef SSE_HD6301_LL 

//  ASSERT(value&RE); // Receive is never disabled by program
  if(value&1)
    TRACE("Set 6301 stand-by\n");
  
  // bits 5-7 of TRCSR can't be set by software
  value&=0x1F;  
  value|=(iram[0x11]&0xE0); // add RO bits 5-7
  ireg_putb (TRCSR, value);

#else

  u_char trcsr; //ST
  trcsr = trcsr_getb (TRCSR);
  /*
   * trcsr & TDRE is always non-zero, thus we can
   * start generating tx int. request immediately
   */
  if (trcsr & TIE)
    txinterrupts = 1;
  else
    txinterrupts = 0;

#endif

}

/*
 * rdr_getb - return the byte in the SCI receive data register
 *
 * If cpu is running (not memory dump), eat the "received" byte,
 * decrement number of outstanding rx interrupts
 * Assume RIE is enabled.
 */
u_char rdr_getb (offs)
  u_int  offs;
{
  if (cpu_isrunning ()) {
    /*
     * If recvbuf is not empty, eat a byte from it
     * into RDR
     */
#if defined(SSE_HD6301_LL)

    if(iram[TRCSR]&RDRF) 
    {
      TRACE("6301 (PC %X) reads RDR %X\n",reg_getpc(),Ikbd.rdr);
      iram[TRCSR]&=~RDRF;
    }
    if(iram[TRCSR]&ORFE) 
    {
      TRACE("6301 clear OVR\n");
      iram[TRCSR]&=~ORFE; // clear overrun bit - we don't check if read TRCSR first
    }

#else

    if (rxinterrupts) {
      rec_byte=recvbuf[rxindex];
      ireg_putb (RDR, recvbuf[rxindex++]);
      rxinterrupts--;
    }
    /*
     * If the cpu has read all bytes in recvbuf[]
     * make recvbuf[] ready for more user sci data input
     */
    if (rxinterrupts == 0)
      rxindex = 0;

#endif
  }
  return ireg_getb (RDR);
}

/*
 * tdr_putb - called to output a character
 *
 * Sets global interrupt flag if Tx interrupt is enabled
 * to signalize main loop to execute sci interrupt vector
 */

tdr_putb (offs, value)
  u_int  offs;
  u_char value;
{ 
  u_char trcsr;

  ireg_putb (TDR, value);
#if !defined(SSE_HD6301_LL) // this would pollute our trace (wondered what it was)
  io_putb (value); 
#endif
  /*
   * trcsr & TDRE is always non-zero, thus we can
   * start generating tx int. request immediately
   */

  trcsr = trcsr_getb (TRCSR);

#if defined(SSE_HD6301_LL)
/*  Double buffer allows 1 byte waiting in TDR while another is being
    shifted, but not more. 
    TDRE = 0 while a byte is waiting in TDR.
*/

#if defined(SSE_IKBD_RTC) && defined(SSE_IKBDI)
  // 1C INTERROGATE TIME-OF-DAT CLOCK
  if(Ikbd.LastCommand==0x1C && regs.accd.a==1 && Ikbd.tdrs==0xfc) 
  {
    if(OPTION_BATTERY6301==2) // dangerous hack
    {
      value += 0xA0;
      Ikbd.LastCommand = -1;
    }
  }
#endif
  Ikbd.tdr=value;
  TRACE("6301 TDR %X\n",Ikbd.tdr);
  iram[TRCSR]&=~TDRE;
  // starting a transmission
  if(!acia[ACIA_IKBD].LineRxBusy)
  {
    // Implement a one bit delay before TDR->TDRS like in the ACIA.
    int cycles_for_one_bit=128; // normally depends on RMCR, we use the ST value
    Ikbd.time_of_tdr_to_tdrs=cpu.ncycles+cycles_for_one_bit;
    acia[ACIA_IKBD].LineRxBusy=2;
  }

#else

  if (trcsr & TIE)
    txinterrupts = 1;

#endif

}
