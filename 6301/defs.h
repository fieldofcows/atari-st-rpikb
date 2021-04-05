#ifndef DEFS_H
#define DEFS_H

#ifndef u_char
#define u_char unsigned char
#endif

#if defined(__STDC__)||defined(__GNUC__)||defined(__cplusplus)||defined(__TURBOC__)
#define s_char signed char
#else
#define s_char char    /* Sun cc for example */
#endif

#ifndef u_int
#define u_int unsigned int
#endif

#ifndef u_short
#define u_short unsigned short
#endif

#ifndef u_long
#define u_long unsigned long
#endif
/*  SS
#ifndef bool
#define bool u_char
#endif
*/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BUFSIZE
#define BUFSIZE    1024
#define MAXBUFSIZE 1024
#endif

#ifndef LSB
#define LSB(x) ((x)&0xFF)
#endif

#ifndef MSB
#define MSB(x) LSB((x)>>8)
#endif

#ifndef NULL
#define NULL 0
#endif

#ifdef __MSDOS__
/*
 * Want to allocate and set memory arrays of size 65536, malloc() can't do it.
 * Turboc doesn't include needed prototype for farmalloc() if __STDC__ 
 * is defined (-A option), so we include it here.
 */
#define malloc farmalloc
#define memset memsetl
extern void *memsetl (void *s, int c, unsigned long n); /* supplied by us */
extern void *farmalloc (unsigned long nbytes);
#endif

#if defined(unix) || defined(__unix) || defined(__unix__)
#define HAS_TERMIO
#endif

#endif /* DEFS_H */


