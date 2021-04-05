/* Automagically generated Wed Oct  9 07:33:56 GMT+0100 1996 - dont edit */
#ifndef SYMTAB_H
#define SYMTAB_H

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

extern char *sym_find_name P_((int value));
extern int sym_find_value P_((char *name, int *value));
extern int sym_add P_((int value, char *name));
extern int sym_readfile P_((char *loadfile, char *symfile));

#undef P_
#endif /* SYMTAB_H */
