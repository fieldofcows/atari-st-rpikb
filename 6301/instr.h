#ifndef INSTR_H
#define INSTR_H

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif


extern int reset P_((void));
extern int instr_exec P_((void));
extern int instr_print P_((u_short addr));

#undef P_
#endif /* INSTR_H */
