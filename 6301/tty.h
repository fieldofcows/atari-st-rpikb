/* Automagically generated Wed Oct  9 07:33:58 GMT+0100 1996 - dont edit */
#ifndef TTY_H
#define TTY_H

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

extern int tty_noblock P_((int fd, char *tty_orig));
extern int tty_restore P_((int fd, char *tty));
extern int tty_getkey P_((int fd));

#undef P_
#endif /* TTY_H */
