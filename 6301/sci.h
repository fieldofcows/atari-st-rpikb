/* Automagically generated Wed Sep  6 21:57:33  1995 - dont edit */
#ifndef H6301_SCI_H
#define H6301_SCI_H

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/*
 * serial_int - check for serial interrupt
 *
 * Return nonzero if interrupt generated, zero if no interrupt
 */

// using the status register 
#define serial_int()\
  (((ireg_getb (TRCSR) & RDRF) && (ireg_getb (TRCSR) & RIE))\
   || ((ireg_getb (TRCSR) & TDRE) && (ireg_getb (TRCSR) & TIE)))
extern int sci_in P_((u_char *s, int nbytes));
extern int sci_print P_((void));
extern u_char trcsr_getb P_((u_int offs));
extern int trcsr_putb P_((u_int offs, u_char value));
extern u_char rdr_getb P_((u_int offs));
extern int tdr_putb P_((u_int offs, u_char value));

#undef P_
#endif /* H6301_SCI_H */
