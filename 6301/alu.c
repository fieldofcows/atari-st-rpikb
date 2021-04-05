/* <<<                                  */
/* Copyright (c) 1994-1996 Arne Riiber. */
/* All rights reserved.                 */
/* >>>                                  */
/*
 * alu.c - operations in arithmetic/logic unit
 *
 * $Id: alu.c,v 1.1 1995/07/11 11:40:15 arne Exp arne $
 *
 * $Log: alu.c,v $
 * Revision 1.1  1995/07/11  11:40:15  arne
 * Initial revision
 *
 */

#include "defs.h"   /* unsigned/signed definitions */
#include "reg.h"

#ifdef USE_PROTOTYPES
#include "alu.h"
#include "reg.h"
#endif
/*
 *  addbyte - Return val1 + val2 + carry, set CCR flags
 *
 */
int
alu_addbyte (val1, val2, carry)
  u_char val1;
  u_char val2;
  u_char carry;   /* 0 or 1 */
{
  u_int  result = val1 + val2 + carry;
  u_char cflag  = (result >> 1) & 0x80;

  reg_setcflag (cflag);
  reg_setnflag (result & 0x80);
  reg_setvflag (((val1 ^ val2 ^ result) ^ cflag) & 0x80);
  reg_setzflag ((result & 0xFF) == 0);
  reg_sethflag ((val1 ^ val2 ^ result) & 0x10); /* addbyte only */
  return result;
}


/*
 *  addword - Return val1 + val2 + carry, set CCR flags
 */
alu_addword (val1, val2, carry)
  u_int val1;
  u_int val2;
  u_char  carry;
{
  u_long result = (long) val1 + val2 + carry;
  u_int  cflag  = (result >> 1) & 0x8000;

  reg_setcflag (cflag);
  reg_setnflag (result & 0x8000);
  reg_setvflag (((val1 ^ val2 ^ result) ^ cflag) & 0x8000);
  reg_setzflag ((result & 0xFFFF) == 0);
  return result;
}

/*
 * andbyte - return the bitwise and of val1 and val2
 */
alu_andbyte (val1, val2)
  u_char val1;
  u_char val2;
{
  u_int result = (val1 & val2) & 0xFF;
  alu_bittestbyte (result);
  return result;
}


/*
 *  bittest* - set NZV flags and return value,
 *    called by the bit test instructions and other
 */

alu_bittestbyte (value)
  u_char value;
{
  reg_setnflag (value & 0x80);
  reg_setzflag (value == 0);
  reg_setvflag (0);
  return value;
}

alu_bittestword (value)
  u_int value;
{
  reg_setnflag (value & 0x8000);
  reg_setzflag (value == 0);
  reg_setvflag (0);
  return value;
}


/*
 *  clrbyte - called by clr*() to set flags and return zero
 */
alu_clrbyte (value)
  u_char value;
{
  reg_setnflag (0);
  reg_setzflag (1);
  reg_setvflag (0);
  reg_setcflag (0);
  return 0;
}

/*
 * combyte - called by com*() to 1'complement a byte and set flags
 */
alu_combyte (value)
  u_char value;
{
  u_char result = ~value;
  reg_setnflag (result & 0x80);
  reg_setzflag (result == 0);
  reg_setvflag (0);
  reg_setcflag (1);
  return result;
}

/*
 * decbyte - called by dec*() to decrement a byte and set flags
 */
alu_decbyte (value)
  u_char value;
{
  reg_setvflag (value == 0x80);
  value = (--value & 0xFF);
  reg_setnflag (value & 0x80);
  reg_setzflag (value == 0);
  return value;
}

/*
 * decword - called by dex*(), dey*() to decrement a word and set flags
 */
alu_decword (value)
  u_int value;
{
  value = --value & 0xFFFF;
  reg_setzflag (value == 0);
  return value;
}


/*
 * incbyte - called by inc*() to increment a byte and set flags
 */
alu_incbyte (value)
  u_char value;
{
  reg_setvflag (value == 0x7F);
  value = ++value & 0xFF;
  reg_setnflag (value & 0x80);
  reg_setzflag (value == 0);
  return value;
}

/*
 * incword - called by inx*(), iny*() to increment a word and set flags
 */
alu_incword (value)
  u_int value;
{
  value = ++value & 0xFFFF;
  reg_setzflag (value == 0);
  return value;
}

alu_negbyte (value)
  u_char value;
{
  return alu_subbyte (0, value, 0);
}

/*
 * orbyte - set flags and return the inclusive or of val1 and val2
 */
alu_orbyte (val1, val2)
  u_char val1;
  u_char val2;
{
  u_char result = (val1 | val2) & 0xFF;
  alu_bittestbyte (result);
  return result;
}


/*
 * xorbyte - return the exclusive or of val1 and val2
 */
alu_xorbyte (val1, val2)
  u_char val1;
  u_char val2;
{
  u_char result = (val1 ^ val2) & 0xFF;
  alu_bittestbyte (result);
  return result;
}



/*
 * shlbyte - called by asl()/lsl()/rol() to shift left and set flags
 */
alu_shlbyte (operand, lsbit)
  u_int operand;
  u_char lsbit;
{
  u_int  result = ((operand << 1) & 0xFF) | (lsbit != 0);
  u_char cflag  = operand & 0x80;

  reg_setnflag (result & 0x80);
  reg_setzflag (result == 0);
  reg_setvflag ((result ^ cflag) & 0x80);
  reg_setcflag (cflag);
  return result;
}


/*
 * shrbyte - called by asr()/lsr()/ror() to shift right and set flags
 */
alu_shrbyte (operand, msbit)
  u_int operand;
  u_char  msbit;
{
  u_int result = ((operand >> 1) & 0xFF) | (msbit ? 0x80 : 0);
  u_char cflag = operand & 0x01 ? 0x80 : 0;

  reg_setnflag (result & 0x80);
  reg_setzflag (result == 0);
  reg_setvflag ((result ^ cflag) & 0x80);
  reg_setcflag (cflag);
  return result;
}


/*
 * shlword - called by asld()/lsld() to shift left and set flags
 */
alu_shlword (operand, lsbit)
  u_int operand;
  u_char lsbit;
{
  u_int result = ((operand << 1) & 0xFFFF) | (lsbit != 0);
  u_int cflag  = operand & 0x8000;

  reg_setnflag (result & 0x8000);
  reg_setzflag (result == 0);
  reg_setvflag ((result ^ cflag) & 0x8000);
  reg_setcflag (cflag);
  return result;
}


/*
 * shrword - called by asrd()/lsrd() to shift right and set flags
 */
alu_shrword (operand, msbit)
  u_int operand;
  u_char  msbit;
{
  u_int result = ((operand >> 1) & 0xFFFF) | (msbit ? 0x8000 : 0);
  u_int cflag  = operand & 0x0001 ? 0x8000 : 0;

  reg_setnflag (result & 0x8000);
  reg_setzflag (result == 0);
  reg_setvflag ((result ^ cflag) & 0x8000);
  reg_setcflag (cflag);
  return result;
}


/*
 * subbyte - subtract val2 and carry from val1, set flags
 */
alu_subbyte (val1, val2, carry)
  u_char val1;
  u_char val2;
  u_char  carry;    /* 0 or 1 */
{
  u_int  result = val1 - val2 - carry;
  u_char cflag  = (result >> 1) & 0x80;

  reg_setcflag (cflag);
  reg_setnflag (result & 0x80);
  reg_setvflag (((val1 ^ val2 ^ result) ^ cflag) & 0x80);
  reg_setzflag ((result & 0xFF) == 0);
  return result;

}


/*
 * subword - subtract val2 and carry from val1, set flags
 */
alu_subword (val1, val2, carry)
  u_int val1;
  u_int val2;
  u_char carry;   /* 0 or 1 */
{
  u_long result = (long) val1 - val2 - carry;
  u_int  cflag  = (result >> 1) & 0x8000;

  reg_setcflag (cflag);
  reg_setnflag (result & 0x8000);
  reg_setvflag (((val1 ^ val2 ^ result) ^ cflag) & 0x8000);
  reg_setzflag ((result & 0xFFFF) == 0);
  return result;
}

/*
 *  testbyte - called by tst*() to subtract zero and set flags
 *
 */
alu_testbyte (operand)
  u_char operand;
{
  int result = alu_subbyte (operand, 0, 0);
  reg_setvflag (0);
  reg_setcflag (0);
  return result;
}

