/* Automagically generated Wed Oct  9 07:33:34 GMT+0100 1996 - dont edit */
#ifndef CALLSTAC_H
#define CALLSTAC_H

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

extern int callstack_push P_((unsigned int addr));
extern int callstack_pop P_((void));
extern int callstack_peek_addr P_((void));
extern int callstack_peek_stack P_((void));
extern int callstack_nelem P_((void));
extern int callstack_print P_((void));
extern int callstack_trace P_((int on));

#undef P_
#endif /* CALLSTAC_H */
