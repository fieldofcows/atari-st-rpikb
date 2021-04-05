/* Automagically generated Wed Oct  9 07:33:53 GMT+0100 1996 - dont edit */
#ifndef FPRINTHE_H
#define FPRINTHE_H

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

extern int fprinthex P_((FILE *fp, unsigned char *buf, int len));

#undef P_
#endif /* FPRINTHE_H */
