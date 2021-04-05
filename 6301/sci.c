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
/*
 * sci_in - input to SCI
 *
 * increment number of outstanding rx interrupts
 */

/*  This function is called by Steem when a byte sent to the 6301 is supposed
    to have been received in rdrs.
*/

sci_in(s, nbytes)
    u_char *s;
int nbytes;
{
#if defined(SSE_DEBUG)
  u_char trcsr = iram[TRCSR];
  //  ASSERT(nbytes==1);
  //  ASSERT(trcsr&RE);
  if (trcsr & WU) // bug?
    TRACE("6301 in standby mode\n");
#endif
  // detect OVR condition, set flag at once (unlike ACIA)
  if (iram[TRCSR] & RDRF)
  {
    TRACE("6301 OVR RDR %X RDRS %X SR %X->%X\n", Ikbd.rdr, Ikbd.rdrs, iram[TRCSR], iram[TRCSR] | ORFE);
    iram[TRCSR] |= ORFE; // hardware sets overrun bit
  }
  else
  {
    Ikbd.rdr = *s;
    ireg_putb(RDR, Ikbd.rdr);
    TRACE("6301 RDR %X\n", Ikbd.rdr);
  }
  iram[TRCSR] |= RDRF; // set RDRF

}

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

u_char trcsr_getb(offs)
    u_int offs;
{
  unsigned char rv;
  rv = ireg_getb(TRCSR);
  return rv;
}

/*
 *  trcsr_putb - enable/disable tx/rx interrupt
 *
 *  Sets global interrupt flag if tx interrupt is enabled
 *  so main loop can execute interrupt vector
 */
trcsr_putb(offs, value)
    u_int offs;
u_char value;
{

  //  ASSERT(value&RE); // Receive is never disabled by program
  if (value & 1)
    TRACE("Set 6301 stand-by\n");

  // bits 5-7 of TRCSR can't be set by software
  value &= 0x1F;
  value |= (iram[0x11] & 0xE0); // add RO bits 5-7
  ireg_putb(TRCSR, value);
}

/*
 * rdr_getb - return the byte in the SCI receive data register
 *
 * If cpu is running (not memory dump), eat the "received" byte,
 * decrement number of outstanding rx interrupts
 * Assume RIE is enabled.
 */
u_char rdr_getb(offs)
    u_int offs;
{
  if (cpu_isrunning())
  {
    /*
     * If recvbuf is not empty, eat a byte from it
     * into RDR
     */

    if (iram[TRCSR] & RDRF)
    {
      TRACE("6301 (PC %X) reads RDR %X\n", reg_getpc(), Ikbd.rdr);
      iram[TRCSR] &= ~RDRF;
    }
    if (iram[TRCSR] & ORFE)
    {
      TRACE("6301 clear OVR\n");
      iram[TRCSR] &= ~ORFE; // clear overrun bit - we don't check if read TRCSR first
    }

  }
  return ireg_getb(RDR);
}

/*
 * tdr_putb - called to output a character
 *
 * Sets global interrupt flag if Tx interrupt is enabled
 * to signalize main loop to execute sci interrupt vector
 */

tdr_putb(offs, value)
    u_int offs;
u_char value;
{
  u_char trcsr;

  ireg_putb(TDR, value);
  /*
   * trcsr & TDRE is always non-zero, thus we can
   * start generating tx int. request immediately
   */

  trcsr = trcsr_getb(TRCSR);

  /*  Double buffer allows 1 byte waiting in TDR while another is being
    shifted, but not more. 
    TDRE = 0 while a byte is waiting in TDR.
*/

  Ikbd.tdr = value;
  TRACE("6301 TDR %X\n", Ikbd.tdr);
  iram[TRCSR] &= ~TDRE;
  // starting a transmission

}
