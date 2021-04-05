/* Automagically generated Wed Sep  6 21:57:33  1995 - dont edit */
#ifndef H6301_SCI_H
#define H6301_SCI_H

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

#if !defined(SSE_HD6301_LL)
extern int  rxinterrupts; /* Number of outstanding rx interrupts */
extern int  txinterrupts; /* Number of outstanding tx interrupts */
#endif

/*
 * serial_int - check for serial interrupt
 *
 * Return nonzero if interrupt generated, zero if no interrupt
 */

#if defined(SSE_HD6301_LL) 
// using the status register 
#define serial_int()\
  (((ireg_getb (TRCSR) & RDRF) && (ireg_getb (TRCSR) & RIE))\
   || ((ireg_getb (TRCSR) & TDRE) && (ireg_getb (TRCSR) & TIE)))
#else
#define serial_int()\
  ((rxinterrupts && (ireg_getb (TRCSR) & RIE))\
   || (txinterrupts && (ireg_getb (TRCSR) & TIE)))
#endif
extern int sci_in P_((u_char *s, int nbytes));
extern int sci_print P_((void));
extern u_char trcsr_getb P_((u_int offs));
extern int trcsr_putb P_((u_int offs, u_char value));
extern u_char rdr_getb P_((u_int offs));
extern int tdr_putb P_((u_int offs, u_char value));

#undef P_
#endif /* H6301_SCI_H */
