/* Automagically generated Wed Oct  9 07:41:03 GMT+0100 1996 - dont edit */
#ifndef M68XX_ALU_H
#define M68XX_ALU_H

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif


/* ../../src/arch/m68xx/alu.c */
 int alu_addbyte P_((u_char val1, u_char val2, u_char carry));
extern int alu_addword P_((u_int val1, u_int val2, u_char carry));
extern int alu_andbyte P_((u_char val1, u_char val2));
extern int alu_bittestbyte P_((u_char value));
extern int alu_bittestword P_((u_int value));
extern int alu_clrbyte P_((u_char value));
extern int alu_combyte P_((u_char value));
extern int alu_decbyte P_((u_char value));
extern int alu_decword P_((u_int value));
extern int alu_incbyte P_((u_char value));
extern int alu_incword P_((u_int value));
extern int alu_negbyte P_((u_char value));
extern int alu_orbyte P_((u_char val1, u_char val2));
extern int alu_xorbyte P_((u_char val1, u_char val2));
extern int alu_shlbyte P_((u_int operand, u_char lsbit));
extern int alu_shrbyte P_((u_int operand, u_char msbit));
extern int alu_shlword P_((u_int operand, u_char lsbit));
extern int alu_shrword P_((u_int operand, u_char msbit));
extern int alu_subbyte P_((u_char val1, u_char val2, u_char carry));
extern int alu_subword P_((u_int val1, u_int val2, u_char carry));
extern int alu_testbyte P_((u_char operand));

#undef P_
#endif /* M68XX_ALU_H */
