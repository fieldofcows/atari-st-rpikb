/* <<<                                  */
/* Copyright (c) 1994-1996 Arne Riiber. */
/* All rights reserved.                 */
/* >>>                                  */
/*====================================================================*/
/* opfunc.c - functions that execute Motorola 68xx instructions */
/*====================================================================*/

#include "defs.h"

#ifdef USE_PROTOTYPES
/* Function prototypes */
#include "alu.h"
#include "reg.h"
#include "opfunc.h"
#endif

#include "memory.h"
#include "reg.h"
#include "alu.c"  /* To get inline functions */

/*
 *  CCR flag shorthands
 */
#define C (reg_getcflag())
#define Z (reg_getzflag())
#define V (reg_getvflag())
#define N (reg_getnflag())



/*====================================================================*/
/* opfunc.c - functions that execute Motorola 6800 instructions */
/*====================================================================*/

/*
 *  Space-savers
 */


/*
 * Functions returning a memory address
 */
#if defined(NDEBUG)
getaddr_dir ()  {return mem_getb (reg_postincpc (1));}
#else
getaddr_dir ()  {
  int operand_addr=reg_postincpc(1);
  int addr=mem_getb (operand_addr);
  return addr;
}
#endif
getaddr_ext ()  {return mem_getw (reg_postincpc (2));}
getaddr_ix  ()  {return (mem_getb (reg_postincpc (1)) + reg_getix()) & 0xffff;}

/*
 * Functions returning the value of a memory address
 */
u_char getbyte_imm () {return mem_getb (reg_postincpc (1));}
u_char getbyte_dir () {return mem_getb (getaddr_dir ());}
u_char getbyte_ext () {return mem_getb (getaddr_ext ());}
u_char getbyte_ix  () {return mem_getb (getaddr_ix  ());}
u_short getword_imm () {return mem_getw (reg_postincpc (2));}
#if defined(NDEBUG)
u_short getword_dir () {return mem_getw (getaddr_dir ());}
#else
u_short getword_dir () {
  int word_addr=getaddr_dir ();
  u_short word_value=mem_getw (word_addr);
  return word_value;
}
#endif
u_short getword_ext () {return mem_getw (getaddr_ext ());}
u_short getword_ix  () {return mem_getw (getaddr_ix  ());}


/*
 * branch_expr - branch to offset pointed to by pc if expr is true
 */
branch_expr (expr)
  int expr;
{
  s_char offs = getbyte_imm ();
  reg_incpc (expr ? offs : 0);
}

/*
 * Stack operators (SP points to current free stack location)
 */
pushbyte (value) u_char value;  {mem_putb (reg_postdecsp (1), value);}
pushword (value) u_int value; {pushbyte (value); pushbyte (value >> 8);}
popbyte ()
{
  return (mem_getb (reg_preincsp (1)));
}
popword ()
{
  u_int hibyte = popbyte ();
  u_int lobyte = popbyte ();
  return (hibyte << 8) | lobyte;
}

/*
 * Functions operating on memory address, no advance of PC (except jump class)
 */
asr_addr (addr)
  u_int addr;
{
  u_int operand = mem_getb (addr);
  mem_putb (addr, alu_shrbyte (operand, operand & 0x80));
}
clr_addr (addr) {mem_putb (addr, alu_clrbyte (mem_getb (addr)));}
com_addr (addr) {mem_putb (addr, alu_combyte (mem_getb (addr)));}
dec_addr (addr) {mem_putb (addr, alu_decbyte (mem_getb (addr)));}
jsr_addr (addr)
{
  pushword (reg_getpc ());  /* Return address */
  reg_setpc (addr);
#if !defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
  callstack_push (addr);  /* subroutine ref. */
#endif
}
inc_addr (addr)   {mem_putb (addr, alu_incbyte (mem_getb (addr)));}
lsl_addr (addr)   {mem_putb (addr, alu_shlbyte (mem_getb (addr), 0));}
lsr_addr (addr)   {mem_putb (addr, alu_shrbyte (mem_getb (addr), 0));}
neg_addr (addr)   {mem_putb (addr, alu_negbyte (mem_getb (addr)));}
rol_addr (addr)   {mem_putb (addr, alu_shlbyte (mem_getb (addr), C));}
ror_addr (addr)   {mem_putb (addr, alu_shrbyte (mem_getb (addr), C));}
staa_addr (addr)  {mem_putb (addr, alu_bittestbyte (reg_getacca ()));}
stab_addr (addr)  {mem_putb (addr, alu_bittestbyte (reg_getaccb ()));}
sts_addr (addr)   {mem_putw (addr, alu_bittestword (reg_getsp ()));}
stx_addr (addr)   {mem_putw (addr, alu_bittestword (reg_getix ()));}
tst_addr (addr)   {alu_testbyte (mem_getb (addr));}
/*
 * int_addr - generate interrupt at the given vector address
 */
int_addr (addr)
  u_int addr;
{
  pushword (reg_getpc ());
  pushword (reg_getix());
  pushbyte (reg_getacca ());
  pushbyte (reg_getaccb ());
  pushbyte (reg_getccr());
  reg_setpc (mem_getw (addr));
#if !defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
  callstack_push (reg_getpc ());               /* new subroutine ref. */
#endif
  reg_setiflag (1);
}


/****************************************************************************
 *  Functions simulating CPU instruction execution
 ****************************************************************************/
aba_inh ()  {reg_setacca (alu_addbyte (reg_getacca (), reg_getaccb (), 0));}

adca_imm () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_imm (), C));}
adca_dir () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_dir (), C));}
adca_ext () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_ext (), C));}
adca_ind_x () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_ix  (), C));}

adcb_imm () {reg_setaccb (alu_addbyte (reg_getaccb (), getbyte_imm (), C));}
adcb_dir () {reg_setaccb (alu_addbyte (reg_getaccb (), getbyte_dir (), C));}
adcb_ext () {reg_setaccb (alu_addbyte (reg_getaccb (), getbyte_ext (), C));}
adcb_ind_x () {reg_setaccb (alu_addbyte (reg_getaccb (), getbyte_ix  (), C));}

adda_imm () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_imm (), 0));}
adda_dir () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_dir (), 0));}
adda_ext () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_ext (), 0));}
adda_ind_x () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_ix  (), 0));}

addb_imm () {reg_setaccb (alu_addbyte (reg_getaccb (), getbyte_imm (), 0));}
addb_dir () {reg_setaccb (alu_addbyte (reg_getaccb (), getbyte_dir (), 0));}
addb_ext () {reg_setaccb (alu_addbyte (reg_getaccb (), getbyte_ext (), 0));}
addb_ind_x () {reg_setaccb (alu_addbyte (reg_getaccb (), getbyte_ix  (), 0));}

anda_imm () {reg_setacca (alu_andbyte (reg_getacca (), getbyte_imm ()));}
anda_dir () {reg_setacca (alu_andbyte (reg_getacca (), getbyte_dir ()));}
anda_ext () {reg_setacca (alu_andbyte (reg_getacca (), getbyte_ext ()));}
anda_ind_x () {reg_setacca (alu_andbyte (reg_getacca (), getbyte_ix ()));}

andb_imm () {reg_setaccb (alu_andbyte (reg_getaccb (), getbyte_imm ()));}
andb_dir () {reg_setaccb (alu_andbyte (reg_getaccb (), getbyte_dir ()));}
andb_ext () {reg_setaccb (alu_andbyte (reg_getaccb (), getbyte_ext ()));}
andb_ind_x () {reg_setaccb (alu_andbyte (reg_getaccb (), getbyte_ix  ()));}

asr_ext ()  {asr_addr (getaddr_ext ());}
asr_ind_x ()  {asr_addr (getaddr_ix ());}
asra_inh () {reg_setacca (alu_shrbyte (reg_getacca (), reg_getacca () & 0x80));}
asrb_inh () {reg_setaccb (alu_shrbyte (reg_getaccb (), reg_getaccb () & 0x80));}

bcc_rel ()  {branch_expr (C == 0);}
bcs_rel ()  {branch_expr (C);}
beq_rel ()  {branch_expr (Z);}
bge_rel ()  {branch_expr ((N ^ V) == 0);}
bgt_rel ()  {branch_expr ((Z | (N ^ V)) == 0);}
bhi_rel ()  {branch_expr ((C | Z ) == 0);}
 
bita_imm () {alu_bittestbyte (reg_getacca () & getbyte_imm ());}
bita_dir () {alu_bittestbyte (reg_getacca () & getbyte_dir ());}
bita_ext () {alu_bittestbyte (reg_getacca () & getbyte_ext ());}
bita_ind_x () {alu_bittestbyte (reg_getacca () & getbyte_ix  ());}

bitb_imm () {alu_bittestbyte (reg_getaccb () & getbyte_imm ());}
bitb_dir () {alu_bittestbyte (reg_getaccb () & getbyte_dir ());}
bitb_ext () {alu_bittestbyte (reg_getaccb () & getbyte_ext ());}
bitb_ind_x () {alu_bittestbyte (reg_getaccb () & getbyte_ix  ());}

ble_rel ()  {branch_expr ((Z | (N ^ V)) == 1);}
bls_rel ()  {branch_expr ((C | Z) == 1);}
blt_rel ()  {branch_expr ((N ^ V) == 1);}
bmi_rel ()  {branch_expr (N);}
bne_rel ()  {branch_expr (Z == 0);}
bpl_rel ()  {branch_expr (N == 0);}
bra_rel ()  {branch_expr (1);}
bsr_rel ()
{
  int offs = (s_char) getbyte_imm ();
  jsr_addr (offs + reg_getpc ()); /* preserve pc evaluation order */
}
bvc_rel ()  {branch_expr (V==0);}
bvs_rel ()  {branch_expr (V);}

cba_inh ()  {alu_subbyte (reg_getacca (), reg_getaccb (), 0);}

clc_inh ()  {reg_setcflag (0);}
cli_inh ()  {reg_setiflag (0);}

clr_ext ()  {clr_addr (getaddr_ext ());}
clr_ind_x ()  {clr_addr (getaddr_ix  ());}
clra_inh () {reg_setacca (alu_clrbyte (reg_getacca ()));}
clrb_inh () {reg_setaccb (alu_clrbyte (reg_getaccb ()));}
clv_inh ()  {reg_setvflag (0);}

cmpa_imm () {alu_subbyte (reg_getacca (), getbyte_imm (), 0);}
cmpa_dir () {alu_subbyte (reg_getacca (), getbyte_dir (), 0);}
cmpa_ext () {alu_subbyte (reg_getacca (), getbyte_ext (), 0);}
cmpa_ind_x () {alu_subbyte (reg_getacca (), getbyte_ix  (), 0);}

cmpb_imm () {alu_subbyte (reg_getaccb (), getbyte_imm (), 0);}
cmpb_dir () {alu_subbyte (reg_getaccb (), getbyte_dir (), 0);}
cmpb_ext () {alu_subbyte (reg_getaccb (), getbyte_ext (), 0);}
cmpb_ind_x () {alu_subbyte (reg_getaccb (), getbyte_ix  (), 0);}

com_ext ()  {com_addr (getaddr_ext ());}
com_ind_x ()  {com_addr (getaddr_ix  ());}
coma_inh () {reg_setacca (alu_combyte (reg_getacca ()));}
comb_inh () {reg_setaccb (alu_combyte (reg_getaccb ()));}

cpx_imm ()  {alu_subword (reg_getix (), getword_imm (), 0);}
cpx_dir ()  {alu_subword (reg_getix (), getword_dir (), 0);}
cpx_ext ()  {alu_subword (reg_getix (), getword_ext (), 0);}
cpx_ind_x ()  {alu_subword (reg_getix (), getword_ix  (), 0);}

/*
 *  DAA - Decimal adjust sum of 2 BCD digits in A to two BCD nibbles in A
 *
 *  Flags: NZVC
 */
daa_inh ()
{
  u_int result= reg_getacca ();

  if (reg_gethflag ())
    result+= 0x06;
  if ((result& 0x0f) > 9)
    result+= 6;
  if (C)
    result+= 0x60;
  if ((result& 0xf0) > 0x90)
    result+= 0x60;
  reg_setacca (alu_bittestbyte (result));
  if (result & 0x100)
    reg_setcflag (1);
}

dec_ext ()  {dec_addr (getaddr_ext ());}
dec_ind_x ()  {dec_addr (getaddr_ix ());}
deca_inh () {reg_setacca (alu_decbyte (reg_getacca ()));}
decb_inh () {reg_setaccb (alu_decbyte (reg_getaccb ()));}

des_inh ()  {reg_incsp (-1);}
dex_inh ()  {reg_setix (alu_decword (reg_getix ()));}

eora_imm () {reg_setacca (alu_xorbyte (reg_getacca (), getbyte_imm ()));}
eora_dir () {reg_setacca (alu_xorbyte (reg_getacca (), getbyte_dir ()));}
eora_ext () {reg_setacca (alu_xorbyte (reg_getacca (), getbyte_ext ()));}
eora_ind_x () {reg_setacca (alu_xorbyte (reg_getacca (), getbyte_ix  ()));}

eorb_imm () {reg_setaccb (alu_xorbyte (reg_getaccb (), getbyte_imm ()));}
eorb_dir () {reg_setaccb (alu_xorbyte (reg_getaccb (), getbyte_dir ()));}
eorb_ext () {reg_setaccb (alu_xorbyte (reg_getaccb (), getbyte_ext ()));}
eorb_ind_x () {reg_setaccb (alu_xorbyte (reg_getaccb (), getbyte_ix  ()));}

inc_ext ()  {inc_addr (getaddr_ext ());}
inc_ind_x ()  {inc_addr (getaddr_ix ());}
inca_inh () {reg_setacca (alu_incbyte (reg_getacca ()));}
incb_inh () {reg_setaccb (alu_incbyte (reg_getaccb ()));}
ins_inh ()  {reg_incsp (1);}
inx_inh ()  {reg_setix (alu_incword (reg_getix ()));}

jmp_ext ()  {reg_setpc (getaddr_ext ());}
jmp_ind_x ()  {reg_setpc (getaddr_ix ());}
jsr_ext ()  {jsr_addr (getaddr_ext ());}
jsr_ind_x ()  {jsr_addr (getaddr_ix ());}

ldaa_imm () {reg_setacca (alu_bittestbyte (getbyte_imm ()));}
ldaa_dir () {reg_setacca (alu_bittestbyte (getbyte_dir ()));}
ldaa_ext () {reg_setacca (alu_bittestbyte (getbyte_ext ()));}
ldaa_ind_x () {reg_setacca (alu_bittestbyte (getbyte_ix ()));}

ldab_imm () {reg_setaccb (alu_bittestbyte (getbyte_imm ()));}
ldab_dir () {reg_setaccb (alu_bittestbyte (getbyte_dir ()));}
ldab_ext () {reg_setaccb (alu_bittestbyte (getbyte_ext ()));}
ldab_ind_x () {reg_setaccb (alu_bittestbyte (getbyte_ix ()));}

lds_imm ()  {reg_setsp (alu_bittestword (getword_imm ()));}
lds_dir ()  {reg_setsp (alu_bittestword (getword_dir ()));}
lds_ext ()  {reg_setsp (alu_bittestword (getword_ext ()));}
lds_ind_x ()  {reg_setsp (alu_bittestword (getword_ix  ()));}

ldx_imm ()  {reg_setix (alu_bittestword (getword_imm ()));}
ldx_dir ()  {reg_setix (alu_bittestword (getword_dir ()));}
ldx_ext ()  {reg_setix (alu_bittestword (getword_ext ()));}
ldx_ind_x ()  {reg_setix (alu_bittestword (getword_ix  ()));}

lsl_ext ()  {lsl_addr (getaddr_ext ());}
lsl_ind_x ()  {lsl_addr (getaddr_ix ());}
lsla_inh () {reg_setacca (alu_shlbyte (reg_getacca (), 0));}
lslb_inh () {reg_setaccb (alu_shlbyte (reg_getaccb (), 0));}
lsr_ext ()  {lsr_addr (getaddr_ext ());}
lsr_ind_x ()  {lsr_addr (getaddr_ix ());}
lsra_inh () {reg_setacca (alu_shrbyte (reg_getacca (), 0));}
lsrb_inh () {reg_setaccb (alu_shrbyte (reg_getaccb (), 0));}

neg_ext ()  {neg_addr (getaddr_ext ());}
neg_ind_x ()  {neg_addr (getaddr_ix ());}
nega_inh () {reg_setacca (alu_negbyte (reg_getacca ()));}
negb_inh () {reg_setaccb (alu_negbyte (reg_getaccb ()));}
nop_inh ()  {}
oraa_imm () {reg_setacca (alu_orbyte (reg_getacca (), getbyte_imm ()));}
oraa_dir () {reg_setacca (alu_orbyte (reg_getacca (), getbyte_dir ()));}
oraa_ext () {reg_setacca (alu_orbyte (reg_getacca (), getbyte_ext ()));}
oraa_ind_x () {reg_setacca (alu_orbyte (reg_getacca (), getbyte_ix ()));}

orab_imm () {reg_setaccb (alu_orbyte (reg_getaccb (), getbyte_imm ()));}
orab_dir () {reg_setaccb (alu_orbyte (reg_getaccb (), getbyte_dir ()));}
orab_ext () {reg_setaccb (alu_orbyte (reg_getaccb (), getbyte_ext ()));}
orab_ind_x () {reg_setaccb (alu_orbyte (reg_getaccb (), getbyte_ix  ()));}

psha_inh () {pushbyte (reg_getacca ());}
pshb_inh () {pushbyte (reg_getaccb ());}
pula_inh () {reg_setacca (popbyte ());}
pulb_inh () {reg_setaccb (popbyte ());}

rol_ext ()  {rol_addr (getaddr_ext ());}
rol_ind_x ()  {rol_addr (getaddr_ix  ());}
rola_inh () {reg_setacca (alu_shlbyte (reg_getacca (), C));}
rolb_inh () {reg_setaccb (alu_shlbyte (reg_getaccb (), C));}
ror_ext ()  {ror_addr (getaddr_ext ());}
ror_ind_x ()  {ror_addr (getaddr_ix ());}
rora_inh () {reg_setacca (alu_shrbyte (reg_getacca (), C));}
rorb_inh () {reg_setaccb (alu_shrbyte (reg_getaccb (), C));}
rti_inh ()
{
  reg_setccr  (popbyte ());
  reg_setaccb (popbyte ());
  reg_setacca (popbyte ());
  reg_setix   (popword ());
  reg_setpc   (popword ());
}
rts_inh ()  {reg_setpc (popword ());}
sba_inh ()  {reg_setacca (alu_subbyte (reg_getacca (), reg_getaccb (), 0));}
sbca_imm () {reg_setacca (alu_subbyte (reg_getacca (), getbyte_imm (), C));}
sbca_dir () {reg_setacca (alu_subbyte (reg_getacca (), getbyte_dir (), C));}
sbca_ext () {reg_setacca (alu_subbyte (reg_getacca (), getbyte_ext (), C));}
sbca_ind_x () {reg_setacca (alu_subbyte (reg_getacca (), getbyte_ix  (), C));}
sbcb_imm () {reg_setaccb (alu_subbyte (reg_getaccb (), getbyte_imm (), C));}
sbcb_dir () {reg_setaccb (alu_subbyte (reg_getaccb (), getbyte_dir (), C));}
sbcb_ext () {reg_setaccb (alu_subbyte (reg_getaccb (), getbyte_ext (), C));}
sbcb_ind_x () {reg_setaccb (alu_subbyte (reg_getaccb (), getbyte_ix  (), C));}
sec_inh ()  {reg_setcflag (1);}
sei_inh ()  {reg_setiflag (1);}
sev_inh ()  {reg_setvflag (1);}

staa_dir () {staa_addr (getaddr_dir ());}
staa_ext () {staa_addr (getaddr_ext ());}
staa_ind_x () {staa_addr (getaddr_ix ());}
stab_dir () {stab_addr (getaddr_dir ());}
stab_ext () {stab_addr (getaddr_ext ());}
stab_ind_x () {stab_addr (getaddr_ix  ());}
sts_dir ()  {sts_addr (getaddr_dir ());}
sts_ext ()  {sts_addr (getaddr_ext ());}
sts_ind_x ()  {sts_addr (getaddr_ix  ());}
stx_dir ()  {stx_addr (getaddr_dir ());}
stx_ext ()  {stx_addr (getaddr_ext ());}
stx_ind_x ()  {stx_addr (getaddr_ix ());}

suba_imm () {reg_setacca (alu_subbyte (reg_getacca (), getbyte_imm (), 0));}
suba_dir () {reg_setacca (alu_subbyte (reg_getacca (), getbyte_dir (), 0));}
suba_ext () {reg_setacca (alu_subbyte (reg_getacca (), getbyte_ext (), 0));}
suba_ind_x () {reg_setacca (alu_subbyte (reg_getacca (), getbyte_ix  (), 0));}

subb_imm () {reg_setaccb (alu_subbyte (reg_getaccb (), getbyte_imm (), 0));}
subb_dir () {reg_setaccb (alu_subbyte (reg_getaccb (), getbyte_dir (), 0));}
subb_ext () {reg_setaccb (alu_subbyte (reg_getaccb (), getbyte_ext (), 0));}
subb_ind_x () {reg_setaccb (alu_subbyte (reg_getaccb (), getbyte_ix  (), 0));}

swi_inh ()  {int_addr (0xFFFC);}
tab_inh ()  {reg_setaccb (alu_bittestbyte (reg_getacca ()));}
tap_inh ()  {reg_setccr (reg_getacca ());}
tba_inh ()  {reg_setacca (alu_bittestbyte (reg_getaccb()));}
tpa_inh ()  {reg_setacca (reg_getccr ());}
/*
 * trap - called when an unknown opcode is found
 *
 * 6301 fetches trap vector at $FFEE,
 * hope 6800 does the same
 */
trap ()
{
#if defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
  u_int  routine = 0;
#else
  u_int  routine = callstack_peek_addr ();
#endif
  char  *p       = (char *) sym_find_name (routine);
  warning ("trap: pc:%04x\nSubroutine: %04x %s\n",
     reg_getpc (), routine, p ? p : "");
  int_addr (0xffee); /* Trap vector 6301 */
}
tst_ext ()  {alu_testbyte (getbyte_ext ());}
tst_ind_x ()  {alu_testbyte (getbyte_ix ());}
tsta_inh () {alu_testbyte (reg_getacca ());}
tstb_inh () {alu_testbyte (reg_getaccb ());}
tsx_inh ()  {reg_setix (reg_getsp () + 1);}
txs_inh ()  {reg_setsp (reg_getix () - 1);}
wai_inh ()
{
/*
 *  clock (12);
 *  while (no_interrupts)
 *    clock (1);
 */
}

/*====================================================================*/
/* 6801 extensions to 6800 */
/*====================================================================*/

abx_inh ()    {reg_setix (reg_getix () + reg_getaccb ());}
addd_imm ()   {reg_setaccd (alu_addword (reg_getaccd () ,getword_imm (), 0));}
addd_dir ()   {reg_setaccd (alu_addword (reg_getaccd () ,getword_dir (), 0));}
addd_ext ()   {reg_setaccd (alu_addword (reg_getaccd () ,getword_ext (), 0));}
addd_ind_x () {reg_setaccd (alu_addword (reg_getaccd () ,getword_ix  (), 0));}
asld_inh ()   {lsld_inh ();}  /*  Equal to Logical Shift Left */
brn_rel ()    {branch_expr (0);}
jsr_dir ()    {jsr_addr (getaddr_dir ());}
ldd_imm ()    {reg_setaccd (alu_bittestword (getword_imm ()));}
#if 1 || defined(NDEBUG)
ldd_dir ()    {reg_setaccd (alu_bittestword (getword_dir ()));}
#else
ldd_dir ()    {
  int word_dir=getword_dir ();
  int tmp=alu_bittestword (word_dir);
  //ASSERT(tmp==word_dir);
  reg_setaccd (word_dir);
}
#endif
ldd_ext ()    {reg_setaccd (alu_bittestword (getword_ext ()));}
ldd_ind_x ()  {reg_setaccd (alu_bittestword (getword_ix ()));}
lsld_inh ()   {reg_setaccd (alu_shlword (reg_getaccd (), 0));}
lsrd_inh ()   {reg_setaccd (alu_shrword (reg_getaccd (), 0));}
mul_inh ()
{
  reg_setaccd (reg_getacca () * reg_getaccb ());
  reg_setcflag (reg_getaccb () & 0x80);
}
pshx_inh () {pushword (reg_getix ());}
pulx_inh () {reg_setix (popword ());}
std_dir ()  {mem_putw (getaddr_dir (), alu_bittestword (reg_getaccd ()));}
std_ext ()  {mem_putw (getaddr_ext (), alu_bittestword (reg_getaccd ()));}
std_ind_x ()  {mem_putw (getaddr_ix (),  alu_bittestword (reg_getaccd ()));}
subd_imm ()   {reg_setaccd (alu_subword (reg_getaccd (), getword_imm (), 0));}
subd_dir ()   {reg_setaccd (alu_subword (reg_getaccd (), getword_dir (), 0));}
subd_ext ()   {reg_setaccd (alu_subword (reg_getaccd (), getword_ext (), 0));}
subd_ind_x () {reg_setaccd (alu_subword (reg_getaccd (), getword_ix  (), 0));}

#ifndef M6805 /* 6805 does not use these */

/*====================================================================*/
/* h6301/opfunc.c - functions that execute 6301 specific instructions */
/*====================================================================*/

/* 6301 = 6801 + aim/eim/oim/tim/xgdx/slp */

/*
 *  M op IMM -> M   NZ,V=0
 *
 * where op is one of aim/eim/oim/tim,
 * has the following memory layout:
 *
 * dir  : mem = (opcode,imm,addr)
 * ind_x: mem = (opcode,imm,offs)
 *      ^
 *      |
 *      +--- mem[cpu.pc]
 */


/*
 *  aim - And Immediate with Memory
 */
aim_dir ()
{
  u_int immed = mem_getb (reg_getpc());
  u_int addr  = mem_getb (reg_getpc() + 1);

  mem_putb (addr, alu_andbyte (immed, mem_getb (addr)));
  reg_incpc (2);
}

aim_ind_x ()
{
  u_int immed = mem_getb (reg_getpc());
  u_int offs  = mem_getb (reg_getpc() + 1);
  u_int addr  = reg_getix() + offs;

  mem_putb (addr, alu_andbyte (immed, mem_getb (addr)));
  reg_incpc (2);
}


/*
 * eim - Exclusive or Immediate with Memory
 */
eim_dir ()
{
  u_int immed = mem_getb (reg_getpc());
  u_int addr  = mem_getb (reg_getpc() + 1);

  mem_putb (addr, alu_xorbyte (immed, mem_getb (addr)));
  reg_incpc (2);
}

eim_ind_x ()
{
  u_int immed = mem_getb (reg_getpc());
  u_int offs  = mem_getb (reg_getpc() + 1);
  u_int addr  = reg_getix() + offs;

  mem_putb (addr, alu_xorbyte (immed, mem_getb (addr)));
  reg_incpc (2);
}


/*
 * oim - Or (inclusive) Immediate with Memory
 */
oim_dir () // 72
{
  u_int immed = mem_getb (reg_getpc());
  u_int addr  = mem_getb (reg_getpc() + 1);
  mem_putb (addr, alu_orbyte (immed, mem_getb (addr)));
  reg_incpc (2);
}

oim_ind_x () 
{
  u_int immed = mem_getb (reg_getpc());
  u_int offs  = mem_getb (reg_getpc() + 1);
  u_int addr  = reg_getix() + offs;

  mem_putb (addr, alu_orbyte (immed, mem_getb (addr)));
  reg_incpc (2);
}


/*
 * tim - Test Immediate with Memory
 */
tim_dir () //7B
{
  u_int immed = mem_getb (reg_getpc());
  u_int addr  = mem_getb (reg_getpc() + 1);

  alu_andbyte (immed, mem_getb (addr));
  reg_incpc (2);
}

tim_ind_x () //6B
{
  u_int immed = mem_getb (reg_getpc());
  u_int offs  = mem_getb (reg_getpc() + 1);
  u_int addr  = reg_getix() + offs;

  alu_andbyte (immed, mem_getb (addr));
  reg_incpc (2);
}


xgdx_inh ()
{
  u_int old_x = reg_getix();
  reg_setix (reg_getaccd ());
  reg_setaccd (old_x);
}

slp_inh ()
{
  printf("slp_inh\n");
  /* cpu.state = sleep; */
}


/*====================================================================*/
/* opfunc.c - 6811 functions executing opcodes */
/*====================================================================*/

/*
 * On entry, pc points to operand
 */

/*
 * Functions returning a memory address
 */
getaddr_iy  ()  {return (mem_getb (reg_postincpc (1)) + reg_getiy ()) & 0xffff;}

/*
 * Functions returning the value of a memory address
 */
getbyte_iy  () { return mem_getb (getaddr_iy ());}
getword_iy  () { return mem_getw (getaddr_iy ());}


/*
 *  Space savers
 */

/*
 * xxx_addr routines differ from 6800 in that PC is incremented
 * for those instructions that require more than operand
 */

/*
 * bset_addr - set bits in 'mask' operand for the byte at address 'addr'
 */
bset_addr (addr)
  u_int addr;
{
  u_char mask = getbyte_imm ();
  mem_putb (addr, alu_orbyte (mem_getb (addr), mask));
}

/*
 * bclr_addr - clear bits in 'mask' operand for the byte at address 'addr'
 */
bclr_addr (addr)
  u_int addr;
{
  u_char mask = getbyte_imm ();
  mem_putb (addr, alu_andbyte (mem_getb (addr), ~mask));
}

int_6811 (addr)
  u_int addr;
{
  pushword (reg_getpc ());
  pushword (reg_getiy ());
  pushword (reg_getix ());
  pushbyte (reg_getacca ());
  pushbyte (reg_getaccb ());
  pushbyte (reg_getccr ());
  reg_setpc (mem_getw (addr));
#if !defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
  callstack_push (reg_getpc ());               /* new subroutine ref. */
#endif
  reg_setiflag (1);
}

/*
 * ABY - Add B to Y (set no flags)
 */
aby_inh ()  {reg_setiy (reg_getiy () + reg_getaccb ());}

adca_ind_y () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_iy (), C));}
adcb_ind_y () {reg_setaccb (alu_addbyte (reg_getaccb (), getbyte_iy (), C));}
adda_ind_y () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_iy (), 0));}
addb_ind_y () {reg_setaccb (alu_addbyte (reg_getaccb (), getbyte_iy (), 0));}
addd_ind_y () {reg_setaccd (alu_addword (reg_getaccd (), getword_iy (), 0));}
anda_ind_y () {reg_setacca (alu_andbyte (reg_getacca (), getbyte_iy ()));}
andb_ind_y () {reg_setaccb (alu_andbyte (reg_getaccb (), getbyte_iy ()));}
asl_ind_y ()  {lsl_ind_y ();}
asr_ind_y ()  {asr_addr (getaddr_iy ());}

/*
 * bclr m,n - clr bits in bitmask 'n' of location 'm'
 */
bclr_dir () {bclr_addr (getaddr_dir ());}
bclr_ind_x () {bclr_addr (getaddr_ix ());}
bclr_ind_y () {bclr_addr (getaddr_iy ());}

/*
 * Bit Test A with Memory, set N, Z, V flags
 */
bita_ind_y () {alu_bittestbyte (reg_getacca () & getbyte_iy ());}
bitb_ind_y () {alu_bittestbyte (reg_getaccb () & getbyte_iy ());}

/*
 * brclr m,n,r - branch relative offset 'r' if the bits in bitmask 'n'
 *     of location 'm' are zero
 */
brclr_dir ()  {branch_expr (!(getbyte_dir () & getbyte_imm ()));}
brclr_ind_x ()  {branch_expr (!(getbyte_ix  () & getbyte_imm ()));}
brclr_ind_y ()  {branch_expr (!(getbyte_iy  () & getbyte_imm ()));}

/*
 * brset m,n,r - branch relative offset 'r' if the bits in bitmask 'n'
 *     of location 'm' are set
 */
brset_dir ()  {branch_expr (getbyte_dir () & getbyte_imm ());}
brset_ind_x ()  {branch_expr (getbyte_ix  () & getbyte_imm ());}
brset_ind_y ()  {branch_expr (getbyte_iy  () & getbyte_imm ());}


/*
 * bset m,n - set bits in bitmask 'n' of location 'm'
 */
bset_dir () {bset_addr (getaddr_dir ());}
bset_ind_x () {bset_addr (getaddr_ix ());}
bset_ind_y () {bset_addr (getaddr_iy ());}

clr_ind_y ()  {clr_addr (getaddr_iy ());}

cmpa_ind_y () {alu_subbyte (reg_getacca (), getbyte_iy (), 0);}
cmpb_ind_y () {alu_subbyte (reg_getaccb (), getbyte_iy (), 0);}
com_ind_y ()  {com_addr    (getaddr_iy ());}
cpd_imm ()  {alu_subword (reg_getaccd (), getword_imm (), 0);}
cpd_dir ()  {alu_subword (reg_getaccd (), getword_dir (), 0);}
cpd_ext ()  {alu_subword (reg_getaccd (), getword_ext (), 0);}
cpd_ind_x ()  {alu_subword (reg_getaccd (), getword_ix  (), 0);}
cpd_ind_y ()  {alu_subword (reg_getaccd (), getword_iy  (), 0);}
cpx_ind_y ()  {alu_subword (reg_getix (),   getword_iy  (), 0);}
cpy_imm ()  {alu_subword (reg_getiy (),   getword_imm (), 0);}
cpy_dir ()  {alu_subword (reg_getiy (),   getword_dir (), 0);}
cpy_ext ()  {alu_subword (reg_getiy (),   getword_ext (), 0);}
cpy_ind_x ()  {alu_subword (reg_getiy (),   getword_ix  (), 0);}
cpy_ind_y ()  {alu_subword (reg_getiy (),   getword_iy  (), 0);}
dec_ind_y ()  {dec_addr    (getaddr_iy ());}
dey_inh ()  {reg_setiy   (alu_decword (reg_getiy ()));}
eora_ind_y () {reg_setacca (alu_xorbyte (reg_getacca (), getbyte_iy ()));}
eorb_ind_y () {reg_setaccb (alu_xorbyte (reg_getaccb (), getbyte_iy ()));}

/*
 *  IDIV - Integer divide
 *
 *  D/X -> X, D%X -> D    Z,V=0,C
 *
 *  partially implemented - enough for most cases
 */
idiv_inh ()
{
  u_int d = reg_getaccd ();
  u_int x = reg_getix ();
  u_int result;

  reg_setvflag (0);

  if (x == 0)
  {
    reg_setix (-1);
    reg_setaccd (0);
    reg_setcflag (1);
    //return;
  }


  else //SS
  {
  reg_setix (d / x);
  result = d % x;
  reg_setaccd (result);
  reg_setzflag (result == 0);
  reg_setcflag (result > 65535); /* 0 */
  }
}

/*
 *  FDIV - Fractional divide 16 by 16 (partially implemented)
 *
 *  Not sure about flags here - should be tested!
 *  Is C to be computed from d/x or d%x?
 *
 *  D/X -> X, D%X -> D    ZVC
 *
 *  partially implemented - enough for most cases
 */
fdiv_inh ()
{
  u_int d = reg_getaccd ();
  u_int x = reg_getix ();


  if (d > x)
  {
    reg_setvflag (1);
    //return;
  }

  else //SS
  {
  reg_setix    ((d << 16) / x);
  reg_setaccd  ((d << 16) % x);
  reg_setzflag (reg_getaccd() == 0);
  reg_setvflag ((d & 0x8000) == (x & 0x8000) &&
          (x & 0x8000) != (reg_getaccd() & 0x8000));
  reg_setcflag (reg_getaccd() > 65535); /* 0 */
  }
}

inc_ind_y ()  {inc_addr  (getaddr_iy ());}
iny_inh ()  {reg_setiy (alu_incword (reg_getiy ()));}

jmp_ind_y ()  {reg_setpc (getaddr_iy ());}
jsr_ind_y ()  {jsr_addr  (getaddr_iy ());}

ldaa_ind_y () {reg_setacca (alu_bittestbyte (getbyte_iy ()));}
ldab_ind_y () {reg_setaccb (alu_bittestbyte (getbyte_iy ()));}
ldd_ind_y ()  {reg_setaccd (alu_bittestword (getword_iy ()));}
ldx_ind_y ()  {reg_setix (alu_bittestword (getword_iy ()));}
lds_ind_y ()  {reg_setsp (alu_bittestword (getword_iy ()));}
ldy_imm ()  {reg_setiy (alu_bittestword (getword_imm ()));}
ldy_dir ()  {reg_setiy (alu_bittestword (getword_dir ()));}
ldy_ext ()  {reg_setiy (alu_bittestword (getword_ext ()));}
ldy_ind_x ()  {reg_setiy (alu_bittestword (getword_ix ()));}
ldy_ind_y ()  {reg_setiy (alu_bittestword (getword_iy ()));}
lsl_ind_y ()  {lsl_addr  (getaddr_iy ());}
lsr_ind_y ()  {lsr_addr  (getaddr_iy ());}
neg_ind_y ()  {neg_addr  (getaddr_iy ());}
oraa_ind_y () {reg_setacca (alu_orbyte (reg_getacca (), getbyte_iy ()));}
orab_ind_y () {reg_setaccb (alu_orbyte (reg_getaccb (), getbyte_iy ()));}
pshy_inh () {pushword (reg_getiy ());}
puly_inh () {reg_setiy (popword ());}
rol_ind_y ()  {rol_addr (getaddr_iy ());}
ror_ind_y ()  {ror_addr (getaddr_iy ());}
rti_6811 ()
{
  reg_setccr (popbyte ());
  reg_setaccb (popbyte ());
  reg_setacca (popbyte ());
  reg_setix (popword ());
  reg_setiy (popword ());
  reg_setpc (popword ());
}
sbca_ind_y () {reg_setacca (alu_subbyte (reg_getacca (), getbyte_iy (), 0));}
sbcb_ind_y () {reg_setaccb (alu_subbyte (reg_getaccb (), getbyte_iy (), 0));}
staa_ind_y () {mem_putb (getaddr_iy  (), alu_bittestbyte (reg_getacca ()));}
stab_ind_y () {mem_putb (getaddr_iy  (), alu_bittestbyte (reg_getaccb ()));}
std_ind_y ()  {mem_putw (getaddr_iy  (), alu_bittestword (reg_getaccd ()));}
sts_ind_y ()  {mem_putw (getaddr_iy  (), alu_bittestword (reg_getsp ()));}
stx_ind_y ()  {mem_putw (getaddr_iy  (), alu_bittestword (reg_getix ()));}
sty_dir ()  {mem_putw (getaddr_dir (), alu_bittestword (reg_getiy ()));}
sty_ext ()  {mem_putw (getaddr_ext (), alu_bittestword (reg_getiy ()));}
sty_ind_x ()  {mem_putw (getaddr_ix  (), alu_bittestword (reg_getiy ()));}
sty_ind_y ()  {mem_putw (getaddr_iy  (), alu_bittestword (reg_getiy ()));}
stop_inh () {reg_setpc (reg_getpc () - 1);} /* Infinite loop */
suba_ind_y () {reg_setacca (alu_subbyte (reg_getacca (), getbyte_iy (), 0));}
subb_ind_y () {reg_setaccb (alu_subbyte (reg_getaccb (), getbyte_iy (), 0));}
subd_ind_y () {reg_setaccd (alu_subword (reg_getaccd (), getword_iy (), 0));}
swi_6811 () {int_6811 (0xfff6);}
test_inh () {reg_setpc (reg_getpc () -1);} /* Infinite loop */
trap_6811 ()
{
#if defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
  u_int  routine = 0;
#else
  u_int  routine = callstack_peek_addr ();
#endif
  char  *p       = (char *) sym_find_name (routine);

  warning ("trap: pc:%04x\nSubroutine: %04x %s\n",
     reg_getpc (), routine, p ? p : "");
  int_6811 (0xfff8);
}
tst_ind_y ()  {alu_testbyte (getbyte_iy ());}
tsy_inh ()  {reg_setiy (reg_getsp () + 1);}
tys_inh ()  {reg_setsp (reg_getiy () - 1);}
xgdy_inh ()
{
  u_int old_y = reg_getiy ();
  reg_setiy (reg_getaccd ());
  reg_setaccd (old_y);
}

#else /* compiling for M6805 */

/*====================================================================*/
/* m6805instr.c - 6805 specific instructions */
/*====================================================================*/

/*
 * Functions returning an address (6805 specific address modes)
 */
getaddr_ix0 () {return reg_getix ();}
getaddr_ix1 () {return reg_getix () + getbyte_imm ();} /* incs pc w/1 */
getaddr_ix2 () {return reg_getix () + getword_imm ();} /* incs pc w/2 */

/*
 * Functions returning the value of a memory address
 */
getbyte_ix0 () {return mem_getb (getaddr_ix0 ());}
getbyte_ix1 () {return mem_getb (getaddr_ix1 ());}
getbyte_ix2 () {return mem_getb (getaddr_ix2 ());}

/* set/clear bit 'm' of address 'addr' */
bset_bit (addr, m) {mem_putb (addr, mem_getb (addr) |  (1 << m));}
bclr_bit (addr, m) {mem_putb (addr, mem_getb (addr) & ~(1 << m));}

getirq ()
{

#include <stdio.h>

    while (1)
    {
        printf ("\nIRQ line status (0,1): ");
        fflush (stdout);
        switch (getchar ())
        {
            case '0':
                return 0;
            case '1':
                return 1;
        }
    }
}

/*
 * CPU simulating functions
 */
adc_ix0 () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_ix0 (), C));}
adc_ix1 () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_ix1 (), C));}
adc_ix2 () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_ix2 (), C));}

add_ix0 () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_ix0 (), 0));}
add_ix1 () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_ix1 (), 0));}
add_ix2 () {reg_setacca (alu_addbyte (reg_getacca (), getbyte_ix2 (), 0));}

and_ix0 () {reg_setacca (alu_andbyte (reg_getacca (), getbyte_ix0 ())); }
and_ix1 () {reg_setacca (alu_andbyte (reg_getacca (), getbyte_ix1 ())); }
and_ix2 () {reg_setacca (alu_andbyte (reg_getacca (), getbyte_ix2 ())); }

asl_ix0 () { lsl_addr (getaddr_ix0 ());}
asl_ix1 () { lsl_addr (getaddr_ix1 ());}
asl_ix2 () { lsl_addr (getaddr_ix2 ());}
asl_dir () { lsl_addr (getaddr_dir ());}
aslx_inh() { reg_setix (alu_shlbyte (reg_getix (), 0)); }

asr_ix0 () { asr_addr (getaddr_ix0 ());}
asr_ix1 () { asr_addr (getaddr_ix1 ());}
asr_ix2 () { asr_addr (getaddr_ix2 ());}
asr_dir () { asr_addr (getaddr_dir ());}
asrx_inh() {
  u_char msbit = reg_getix () & 0x80;
  reg_setix (alu_shrbyte (reg_getix (), msbit));
}

/* brset n dir,offs - Branch if bit 'n' of location 'dir' is set */
brset0_btb () {branch_expr (getbyte_dir () & (1 << 0));}
brset1_btb () {branch_expr (getbyte_dir () & (1 << 1));}
brset2_btb () {branch_expr (getbyte_dir () & (1 << 2));}
brset3_btb () {branch_expr (getbyte_dir () & (1 << 3));}
brset4_btb () {branch_expr (getbyte_dir () & (1 << 4));}
brset5_btb () {branch_expr (getbyte_dir () & (1 << 5));}
brset6_btb () {branch_expr (getbyte_dir () & (1 << 6));}
brset7_btb () {branch_expr (getbyte_dir () & (1 << 7));}

/* brclr n dir,offs - Branch if bit 'n' of location 'dir' is zero */
brclr0_btb () {branch_expr (!(getbyte_dir () & (1 << 0)));}
brclr1_btb () {branch_expr (!(getbyte_dir () & (1 << 1)));}
brclr2_btb () {branch_expr (!(getbyte_dir () & (1 << 2)));}
brclr3_btb () {branch_expr (!(getbyte_dir () & (1 << 3)));}
brclr4_btb () {branch_expr (!(getbyte_dir () & (1 << 4)));}
brclr5_btb () {branch_expr (!(getbyte_dir () & (1 << 5)));}
brclr6_btb () {branch_expr (!(getbyte_dir () & (1 << 6)));}
brclr7_btb () {branch_expr (!(getbyte_dir () & (1 << 7)));}

/* bset m,n - set bit m of location n */
bset0_bsc () {bset_bit (getaddr_dir (), 0);}
bset1_bsc () {bset_bit (getaddr_dir (), 1);}
bset2_bsc () {bset_bit (getaddr_dir (), 2);}
bset3_bsc () {bset_bit (getaddr_dir (), 3);}
bset4_bsc () {bset_bit (getaddr_dir (), 4);}
bset5_bsc () {bset_bit (getaddr_dir (), 5);}
bset6_bsc () {bset_bit (getaddr_dir (), 6);}
bset7_bsc () {bset_bit (getaddr_dir (), 7);}

/* bclr m,n - clr bit m of location n */
bclr0_bsc () {bclr_bit (getaddr_dir (), 0);}
bclr1_bsc () {bclr_bit (getaddr_dir (), 1);}
bclr2_bsc () {bclr_bit (getaddr_dir (), 2);}
bclr3_bsc () {bclr_bit (getaddr_dir (), 3);}
bclr4_bsc () {bclr_bit (getaddr_dir (), 4);}
bclr5_bsc () {bclr_bit (getaddr_dir (), 5);}
bclr6_bsc () {bclr_bit (getaddr_dir (), 6);}
bclr7_bsc () {bclr_bit (getaddr_dir (), 7);}

/* bhcc/bhcs - branch if half carry clear/set */
bhcc_rel () {branch_expr (!reg_gethflag ());}
bhcs_rel () {branch_expr ( reg_gethflag ());}

/* bmc/bms - branch if interrupt mask flag is cleared/set */
bmc_rel ()  {branch_expr (!reg_getiflag ());}
bms_rel ()  {branch_expr ( reg_getiflag ());}

/* bih/bil - branch if interrupt line is low/high */
bih_rel ()  {branch_expr ( getirq ());}
bil_rel ()  {branch_expr (!getirq ());}

/* brn_rel ()  {branch_expr (0);} */

/*
 * mul - X*A -> X:A
 */
mul_inh_6805 ()
{
  u_int res = reg_getacca() * reg_getix ();
  reg_setacca (LSB (res));
  reg_setix   (MSB (res));
}

/*
 * IXn - offset n = 0, 1 or 2 bytes
 */
neg_ix0 () { neg_addr (getaddr_ix0 ());}
neg_ix1 () { neg_addr (getaddr_ix1 ());}
neg_ix2 () { neg_addr (getaddr_ix2 ());}
neg_dir () { neg_addr (getaddr_dir ());}
negx_inh() { reg_setix (alu_negbyte (reg_getix ()));}

com_ix0 () { com_addr (getaddr_ix0 ());}
com_ix1 () { com_addr (getaddr_ix1 ());}
com_ix2 () { com_addr (getaddr_ix2 ());}
com_dir () { com_addr (getaddr_dir ());}
comx_inh() { reg_setix (alu_combyte (reg_getix ()));}

lsr_ix0 () { lsr_addr (getaddr_ix0 ());}
lsr_ix1 () { lsr_addr (getaddr_ix1 ());}
lsr_ix2 () { lsr_addr (getaddr_ix2 ());}
lsr_dir () { lsr_addr (getaddr_dir ());}
lsrx_inh() { reg_setix (alu_shrbyte (reg_getix (), 0));}

ror_ix0 () { ror_addr (getaddr_ix0 ());}
ror_ix1 () { ror_addr (getaddr_ix1 ());}
ror_ix2 () { ror_addr (getaddr_ix2 ());}
ror_dir () { ror_addr (getaddr_dir ());}
rorx_inh() {
  u_char msbit = reg_getcflag (); /* readability */
  reg_setix (alu_shrbyte (reg_getix (), msbit));
}

/* lsl - same as asl */
lsl_ix0 () { lsl_addr (getaddr_ix0 ());}
lsl_ix1 () { lsl_addr (getaddr_ix1 ());}
lsl_ix2 () { lsl_addr (getaddr_ix2 ());}
lsl_dir () { lsl_addr (getaddr_dir ());}
lslx_inh() { reg_setix (alu_shlbyte (reg_getix (), 0)); }

rol_ix0 () { rol_addr (getaddr_ix0 ());}
rol_ix1 () { rol_addr (getaddr_ix1 ());}
rol_ix2 () { rol_addr (getaddr_ix2 ());}
rol_dir () { rol_addr (getaddr_dir ());}
rolx_inh() {
  u_char lsbit = reg_getcflag (); /* readability */
  reg_setix (alu_shlbyte (reg_getix (), lsbit));
}
dec_ix0 () { dec_addr (getaddr_ix0 ());}
dec_ix1 () { dec_addr (getaddr_ix1 ());}
dec_ix2 () { dec_addr (getaddr_ix2 ());}
dec_dir () { dec_addr (getaddr_dir ());}
decx_inh() { reg_setix (alu_decbyte (reg_getix ()));}

inc_ix0 () { inc_addr (getaddr_ix0 ());}
inc_ix1 () { inc_addr (getaddr_ix1 ());}
inc_ix2 () { inc_addr (getaddr_ix2 ());}
inc_dir () { inc_addr (getaddr_dir ());}
incx_inh() { reg_setix (alu_incbyte (reg_getix ()));}

tst_ix0 () { tst_addr (getaddr_ix0 ());}
tst_ix1 () { tst_addr (getaddr_ix1 ());}
tst_ix2 () { tst_addr (getaddr_ix2 ());}
tst_dir () { tst_addr (getaddr_dir ());}
tstx_inh() { alu_testbyte (reg_getix ());}

clr_ix0 () { clr_addr (getaddr_ix0 ());}
clr_ix1 () { clr_addr (getaddr_ix1 ());}
clr_ix2 () { clr_addr (getaddr_ix2 ());}
clr_dir () { clr_addr (getaddr_dir ());}
clrx_inh() { reg_setix (alu_testbyte (0));}

stop_inh_6805 () {}
rsp_inh  () {reg_setsp (0xFF);}

/*
 * tax/txa shall not set CCR flags
 */
tax_inh  () { reg_setix   (reg_getacca ()); }
txa_inh  () { reg_setacca (reg_getix   ()); }
wait_inh () {}

/* Acc A, IX0, IX1, IX2 */
sub_ix0 () { reg_setacca (alu_subbyte (reg_getacca (), getbyte_ix0 (), 0)); }
sub_ix1 () { reg_setacca (alu_subbyte (reg_getacca (), getbyte_ix1 (), 0)); }
sub_ix2 () { reg_setacca (alu_subbyte (reg_getacca (), getbyte_ix2 (), 0)); }

cmp_ix0 () { alu_subbyte (reg_getacca (), getbyte_ix0 (), 0); }
cmp_ix1 () { alu_subbyte (reg_getacca (), getbyte_ix1 (), 0); }
cmp_ix2 () { alu_subbyte (reg_getacca (), getbyte_ix2 (), 0); }

sbc_ix0 () { alu_subbyte (reg_getacca (), getbyte_ix0 (), reg_getcflag ()); }
sbc_ix1 () { alu_subbyte (reg_getacca (), getbyte_ix1 (), reg_getcflag ()); }
sbc_ix2 () { alu_subbyte (reg_getacca (), getbyte_ix2 (), reg_getcflag ()); }

cpx_ix0 ()      { alu_subbyte (reg_getix (), getbyte_ix0 (), 0);}
cpx_ix1 ()      { alu_subbyte (reg_getix (), getbyte_ix1 (), 0);}
cpx_ix2 ()      { alu_subbyte (reg_getix (), getbyte_ix2 (), 0);}
cpx_imm_6805 () { alu_subbyte (reg_getix (), getbyte_imm (), 0);}
cpx_dir_6805 () { alu_subbyte (reg_getix (), getbyte_dir (), 0);}
cpx_ext_6805 () { alu_subbyte (reg_getix (), getbyte_ext (), 0);}


bit_ix0 () {alu_bittestbyte (reg_getacca () & getbyte_ix0 ());}
bit_ix1 () {alu_bittestbyte (reg_getacca () & getbyte_ix1 ());}
bit_ix2 () {alu_bittestbyte (reg_getacca () & getbyte_ix2 ());}

lda_ix0 () {reg_setacca (alu_bittestbyte (getbyte_ix0 ()));}
lda_ix1 () {reg_setacca (alu_bittestbyte (getbyte_ix1 ()));}
lda_ix2 () {reg_setacca (alu_bittestbyte (getbyte_ix2 ()));}

sta_ix0 () {mem_putb (getaddr_ix0 (), alu_bittestbyte (reg_getacca ()));}
sta_ix1 () {mem_putb (getaddr_ix1 (), alu_bittestbyte (reg_getacca ()));}
sta_ix2 () {mem_putb (getaddr_ix2 (), alu_bittestbyte (reg_getacca ()));}

eor_ix0 () {reg_setacca (alu_xorbyte (reg_getacca (), getbyte_ix0 ()));}
eor_ix1 () {reg_setacca (alu_xorbyte (reg_getacca (), getbyte_ix1 ()));}
eor_ix2 () {reg_setacca (alu_xorbyte (reg_getacca (), getbyte_ix2 ()));}

ora_ix0 () {reg_setacca (alu_orbyte (reg_getacca (), getbyte_ix0 ()));}
ora_ix1 () {reg_setacca (alu_orbyte (reg_getacca (), getbyte_ix1 ()));}
ora_ix2 () {reg_setacca (alu_orbyte (reg_getacca (), getbyte_ix2 ()));}

jmp_dir () {reg_setpc (getaddr_dir ());}
jmp_ix0 () {reg_setpc (getaddr_ix0 ());}
jmp_ix1 () {reg_setpc (getaddr_ix1 ());}
jmp_ix2 () {reg_setpc (getaddr_ix2 ());}

jsr_ix0 () {jsr_addr (getaddr_ix0 ());}
jsr_ix1 () {jsr_addr (getaddr_ix1 ());}
jsr_ix2 () {jsr_addr (getaddr_ix2 ());}

ldx_ix0 ()      {reg_setix (alu_bittestbyte (getbyte_ix0 ()));}
ldx_ix1 ()      {reg_setix (alu_bittestbyte (getbyte_ix1 ()));}
ldx_ix2 ()      {reg_setix (alu_bittestbyte (getbyte_ix2 ()));}
ldx_imm_6805 () {reg_setix (alu_bittestbyte (getbyte_imm ()));}
ldx_dir_6805 () {reg_setix (alu_bittestbyte (getbyte_dir ()));}
ldx_ext_6805 () {reg_setix (alu_bittestbyte (getbyte_ext ()));}

stx_ix0 ()      {mem_putb (getaddr_ix0 (), alu_bittestbyte (reg_getix ()));}
stx_ix1 ()      {mem_putb (getaddr_ix1 (), alu_bittestbyte (reg_getix ()));}
stx_ix2 ()      {mem_putb (getaddr_ix2 (), alu_bittestbyte (reg_getix ()));}
stx_dir_6805 () {mem_putb (getaddr_dir (), alu_bittestbyte (reg_getix ()));}
stx_ext_6805 () {mem_putb (getaddr_ext (), alu_bittestbyte (reg_getix ()));}

int_6805 (addr)
  u_int addr;
{
  pushword (reg_getpc ());
  pushbyte (reg_getix ());
  pushbyte (reg_getacca ());
  pushbyte (reg_getccr ());
  reg_setpc (mem_getw (addr));
#if !defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
  callstack_push (reg_getpc ());               /* new subroutine ref. */
#endif
  reg_setiflag (1);
}

rti_6805  ()
{
  reg_setccr  (popbyte ());
  reg_setacca (popbyte ());
  reg_setix   (popbyte ());
  reg_setpc   (popword ());
}

swi_6805  ()
{
  extern u_int swivector; /* chip specific */
  int_6805 (swivector); /* chip specific */
}

trap_6805 ()
{
#if defined(SSE_IKBD_6301_DISABLE_CALLSTACK)
  warning ("trap: pc:%04x\n",reg_getpc ());
#else
  warning ("trap: pc:%04x\nSubroutine: %04x\n",
     reg_getpc (), callstack_peek_addr ());
#endif
}

#endif /* M6805 */
