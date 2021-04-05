/*---------------------------------------------------------------------------
PROJECT: Steem SSE
Atari ST emulator
Copyright (C) 2021 by Anthony Hayward and Russel Hayward + SSE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see https://www.gnu.org/licenses/.

DOMAIN: Emu
FILE: cpu.h
DESCRIPTION: Declarations for CPU emulation.
struct TMC68000, m68k_exception, TIplTiming
---------------------------------------------------------------------------*/

#pragma once
#ifndef CPU_DECLA_H
#define CPU_DECLA_H

#include "binary.h"
#include "setjmp.h"
#include "conditions.h"
#include "acc.h"
#include "emulator.h"
#include "device_map.h"
#include "steemh.h"
#include "debug.h"
#include "display.h"
#include "glue.h"
#include "mmu.h"
#include "run.h"
#include "blitter.h"
#include "mfp.h"
#include "shifter.h"
#include <data_union.h>

#ifdef DEBUG_BUILD
#include "d2.h"
#include "debug_emu.h"
#endif

#pragma pack(push, 8)

struct TMC68000 {
  // ENUM
/* MC68000UM p99 (6-1)
"The processor is always in one of three processing states: normal, exception, or halted.
The normal processing state is associated with instruction execution; the memory
references are to fetch instructions and operands and to store results. A special case of
the normal state is the stopped state, resulting from execution of a STOP instruction. In
this state, no further memory references are made."
*/
  enum EMC68000 {NORMAL,EXCEPTION,STOPPED,HALTED}; 
  // FUNCTIONS
  TMC68000();
  void Reset(bool Cold);
  int SyncEClock();
  void UpdateCyclesForEClock();
  inline void m68kUnstop();
  void StemdosCheckFlags();
  bool IsPriv(WORD op);
  // DATA
  COUNTER_VAR dbi_timing;
  COUNTER_VAR cycles_for_eclock; // because of integer overflow problems
  COUNTER_VAR cycles0; // to record A_S_T
  MEM_ADDRESS Pc; // Pc is official register
  DU32 upc; // upc can be temp register (reference pc)
  int BusIdleCycles;
  signed int r[17]; // data and address registers
  WORD sr; // status register
  WORD ird; // current opcode
  WORD irc,ir; //2 words prefetch queue
  BYTE eclock_sync_cycle; //0-9 (0 in practice)
  BYTE ProcessingState;
  bool tpend; // actual internal latch set when CPU should trace current instruction
#if defined(SSE_DEBUG)
  COUNTER_VAR nExceptions;
#endif
};

#pragma pack(pop)


void m68kProcess();


///////////////
// Registers //
///////////////

#define areg (Cpu.r+8) //TODO: will eventually disappear? (still used by Debugger)
#define LITTLE_PC (pc&0xFFFFFE)
#define TRUE_PC Cpu.Pc
extern BYTE d8; // for single byte R/W
extern DUS32 uresult;
extern long &result;
extern short &resulth,&resultl;
extern signed char &resultb;
extern DUS32 *cpureg;
extern DU32 *cpuareg;
extern DU16 uird;
extern WORD &IRC,&IR,&IRD; // there's no real reason, we use those in code
#define REGB(n) cpureg[n].d8[B0] // low order
#define REGW(n) cpureg[n].d16[LO] // low order
#define REGL(n) cpureg[n].d32
#define AREG(n) cpureg[8+n].d32
extern signed int compare_buffer;
extern DU32 uiabus;
extern MEM_ADDRESS &iabus;
extern WORD &iabush;
extern WORD &iabusl;
extern DU32 *ureg;
extern DU32 *cpuareg;
extern MEM_ADDRESS &sp,&other_sp;
extern MEM_ADDRESS &pc;
extern WORD &pch,&pcl;
extern WORD &SR;


///////////////////
// ST Exceptions //
///////////////////

#if defined(SSE_M68K_EXCEPTION_TRY_CATCH)
/*  Using C++ try/catch works but it's dramatically slower in the debug build.
    Check for example with the BIG Demo loader.
    There's certainly a performance hit in the release builds too, as try/catch
    does more than setjmp, but we don't need this work, because setjmp is only
    used to go through the calling stack, in other words it's appropriate. 

    No reason to define, but heed warning C4611.
*/
#define TRY_M68K_EXCEPTION try {
#define CATCH_M68K_EXCEPTION } catch (m68k_exception* exception_object) { 
#define END_M68K_EXCEPTION }
#else
// This is exactly the same as the C++ exceptions except you mustn't return or break
// out of the exception block without executing the END_M68K_EXCEPTION. If you have
// to you can just add pJmpBuf=oldpJmpBuf before the return/break and it will work.
// This could be fixed by making a wrapper class for jmp_buf so it will call the
// destructor when it goes out of scope, but GCC seems flakey on that sort of thing.
#define TRY_M68K_EXCEPTION jmp_buf temp_excep_jump;jmp_buf *oldpJmpBuf=pJmpBuf;pJmpBuf=&temp_excep_jump;if (setjmp(temp_excep_jump)==0){
#define CATCH_M68K_EXCEPTION }else{
#define END_M68K_EXCEPTION }pJmpBuf=oldpJmpBuf;
#endif

enum E68000Exception {
  BOMBS_RESET=0,
  BOMBS_BUS_ERROR=2,
  BOMBS_ADDRESS_ERROR=3,
  BOMBS_ILLEGAL_INSTRUCTION=4,
  BOMBS_DIVISION_BY_ZERO=5,
  BOMBS_CHK=6,
  BOMBS_TRAPV=7,
  BOMBS_PRIVILEGE_VIOLATION=8,
  BOMBS_TRACE_EXCEPTION=9, 
  BOMBS_LINE_A=10,
  BOMBS_LINE_F=11, // 11, not $F!
  BOMBS_SPURIOUS_EXCEPTION=24,
  EA_READ=0,EA_WRITE,EA_FETCH,EA_INST
};

#pragma pack(push, 8)

struct m68k_exception {
  m68k_exception() {
    ZeroMemory(this,sizeof(m68k_exception));
  }
  ~m68k_exception() {}
  void init(int en,int ea,MEM_ADDRESS ad);
  void crash();
  DU32 u_pc;
  DU32 ucrash_address;
  DU32 uaddress;
  int bombs;
  int action;
  WORD crash_sr,crash_ird;
};



struct TIplTiming {
  COUNTER_VAR time;
  COUNTER_VAR ipl; // 3 bits enough but it's padded anyway
};



#pragma pack(pop)

COUNTER_VAR check_ipl(); // return ipl level IF above mask in SR, 0 otherwise
extern COUNTER_VAR tvn_latch_time;
extern TIplTiming ipl_timing[256];
extern BYTE ipl_timing_index;

#define CHECK_IPL tvn_latch_time=A_S_T // macro placed in each opcode function

#define SR_IPL   (BIT_a+BIT_9+BIT_8 )
#define SR_IPL_7 (BIT_a+BIT_9+BIT_8 )
#define SR_IPL_6 (BIT_a+BIT_9       )
#define SR_IPL_5 (BIT_a+      BIT_8 )
#define SR_IPL_4 (BIT_a             )
#define SR_IPL_3 (      BIT_9+BIT_8 )
#define SR_IPL_2 (      BIT_9       )
#define SR_IPL_1 (            BIT_8 )
#define SR_IPL_0                    0


void exception(int en,int ea,MEM_ADDRESS a);
void m68k_trap1(); // illegal opcode
#ifdef SSE_BETA
void m68k_trap1();
void m68k_trap1();
#else
#define m68k_trap1 m68k_trap1
#define m68k_trap1 m68k_trap1
#endif
void m68k_halt();
void m68k_finish_exception(int vector);
void m68k_interrupt(MEM_ADDRESS ad); // only used by utilities
extern m68k_exception ExceptionObject;
extern jmp_buf *pJmpBuf;

#define STOP_INTS_BECAUSE_INTERCEPT_OS ((ioaccess&IOACCESS_INTERCEPT_OS)!=0)


///////////////////////
// Effective Address //
///////////////////////

extern DU32 ueffective_address;
extern DWORD &effective_address;
extern WORD &effective_address_h;
extern WORD &effective_address_l;
extern DU32 um68k_old_dest;
extern DWORD &m68k_old_dest;
extern WORD &m68k_old_dest_h;
extern WORD &m68k_old_dest_l;
extern DUS32 sm68k_src_l;
extern long &m68k_src_l;
extern short &m68k_src_lh;
extern short &m68k_src_ll;
extern char m68k_dst_b;
extern short m68k_dst_w;
extern DUS32 sm68k_dst_l;
extern long &m68k_dst_l;
extern short &m68k_dst_lh;
extern short &m68k_dst_ll;
extern WORD m68k_ap,m68k_iriwo;
extern short m68k_src_w;
extern char m68k_src_b;
extern WORD ry,rx;
// pointers
extern void (*m68k_jsr_get_source_b[8])();
extern void (*m68k_jsr_get_source_w[8])();
extern void (*m68k_jsr_get_source_l[8])();
extern void (*m68k_jsr_get_source_b_not_a[8])();
extern void (*m68k_jsr_get_source_w_not_a[8])();
extern void (*m68k_jsr_get_source_l_not_a[8])();
extern void (*m68k_jsr_get_dest_b[8])();
extern void (*m68k_jsr_get_dest_w[8])();
extern void (*m68k_jsr_get_dest_l[8])();
extern void (*m68k_jsr_get_dest_b_not_a[8])();
extern void (*m68k_jsr_get_dest_w_not_a[8])();
extern void (*m68k_jsr_get_dest_l_not_a[8])();
extern void (*m68k_jsr_get_dest_b_not_a_or_d[8])();
extern void (*m68k_jsr_get_dest_w_not_a_or_d[8])();
extern void (*m68k_jsr_get_dest_l_not_a_or_d[8])();
// functions
void m68k_get_source_000_b();
void m68k_get_source_000_w();
void m68k_get_source_000_l();
void m68k_get_source_001_b();
void m68k_get_source_001_w();
void m68k_get_source_001_l();
void m68k_get_source_010_b();
void m68k_get_source_010_w();
void m68k_get_source_010_l();
void m68k_get_source_011_b();
void m68k_get_source_011_w();
void m68k_get_source_011_l();
void m68k_get_source_100_b();
void m68k_get_source_100_w();
void m68k_get_source_100_l();
void m68k_get_source_101_b();
void m68k_get_source_101_w();
void m68k_get_source_101_l();
void m68k_get_source_110_b();
void m68k_get_source_110_w();
void m68k_get_source_110_l();
void m68k_get_source_111_b();
void m68k_get_source_111_w();
void m68k_get_source_111_l();
void m68k_get_dest_000_b();
void m68k_get_dest_000_w();
void m68k_get_dest_000_l();
void m68k_get_dest_001_b();
void m68k_get_dest_001_w();
void m68k_get_dest_001_l();
void m68k_get_dest_010_b();
void m68k_get_dest_010_w();
void m68k_get_dest_010_l();
void m68k_get_dest_011_b();
void m68k_get_dest_011_w();
void m68k_get_dest_011_l();
void m68k_get_dest_100_b();
void m68k_get_dest_100_w();
void m68k_get_dest_100_l();
void m68k_get_dest_101_b();
void m68k_get_dest_101_w();
void m68k_get_dest_101_l();
void m68k_get_dest_110_b();
void m68k_get_dest_110_w();
void m68k_get_dest_110_l();
void m68k_get_dest_111_b();
void m68k_get_dest_111_w();
void m68k_get_dest_111_l();


inline void m68kGetDestByte() {
  m68k_jsr_get_dest_b[(IRD&BITS_543)>>3]();
}


inline void m68kGetDestWord() {
  m68k_jsr_get_dest_w[(IRD&BITS_543)>>3]();
}


inline void m68kGetDestLong() {
  m68k_jsr_get_dest_l[(IRD&BITS_543)>>3]();
}


inline void m68kGetDestByteNotA() {
  m68k_jsr_get_dest_b_not_a[(IRD&BITS_543)>>3]();
}


inline void m68kGetDestWordNotA() {
  m68k_jsr_get_dest_w_not_a[(IRD&BITS_543)>>3]();
}


inline void m68kGetDestLongNotA() {
  m68k_jsr_get_dest_l_not_a[(IRD&BITS_543)>>3]();
}


inline void m68kGetDestByteNotAOrD() {
  m68k_jsr_get_dest_b_not_a_or_d[(IRD&BITS_543)>>3]();
}


inline void m68kGetDestWordNotAOrD() {
  m68k_jsr_get_dest_w_not_a_or_d[(IRD&BITS_543)>>3]();
}


inline void m68kGetDestLongNotAOrD() {
  m68k_jsr_get_dest_l_not_a_or_d[(IRD&BITS_543)>>3]();
}


inline void m68kGetSourceByte() {
  m68k_jsr_get_source_b[(IRD&BITS_543)>>3]();
}


inline void m68kGetSourceWord() {
  m68k_jsr_get_source_w[(IRD&BITS_543)>>3]();
}


inline void m68kGetSourceLong() {
  m68k_jsr_get_source_l[(IRD&BITS_543)>>3]();
}


inline void m68kGetSourceByteNotA() {
  m68k_jsr_get_source_b_not_a[(IRD&BITS_543)>>3]();
}


inline void m68kGetSourceWordNotA() {
  m68k_jsr_get_source_w_not_a[(IRD&BITS_543)>>3]();
}


inline void m68kGetSourceLongNotA() {
  m68k_jsr_get_source_l_not_a[(IRD&BITS_543)>>3]();
}


BYTE m68k_read_dest_b();
WORD m68k_read_dest_w();
LONG m68k_read_dest_l();

// could have tables for these, but they can be worse for performance
#define DEST_IS_REGISTER ((IRD&BITS_543)<=BITS_543_001)
#define DEST_IS_MEMORY ((IRD&BITS_543)>BITS_543_001)
#define SOURCE_IS_REGISTER_OR_IMMEDIATE ((IRD&BITS_543)<=BITS_543_001 || ((IRD&b00111111)==b00111100) )
#define DEST_IS_DATA_REGISTER ((IRD&BITS_543)==BITS_543_000)
#define DEST_IS_ADDRESS_REGISTER ((IRD&BITS_543)==BITS_543_001)

#define m68k_GET_DEST_B m68kGetDestByte();
#define m68k_GET_DEST_W m68kGetDestWord();
#define m68k_GET_DEST_L m68kGetDestLong();
#define m68k_GET_DEST_B_NOT_A m68kGetDestByteNotA();
#define m68k_GET_DEST_W_NOT_A m68kGetDestWordNotA();
#define m68k_GET_DEST_L_NOT_A m68kGetDestLongNotA();
#define m68k_GET_DEST_B_NOT_A_OR_D m68kGetDestByteNotAOrD();
#define m68k_GET_DEST_W_NOT_A_OR_D m68kGetDestWordNotAOrD();
#define m68k_GET_DEST_L_NOT_A_OR_D m68kGetDestLongNotAOrD();
#define m68k_GET_SOURCE_B m68kGetSourceByte();
#define m68k_GET_SOURCE_W m68kGetSourceWord();
#define m68k_GET_SOURCE_L m68kGetSourceLong();
#define m68k_GET_SOURCE_B_NOT_A m68kGetSourceByteNotA();
#define m68k_GET_SOURCE_W_NOT_A m68kGetSourceWordNotA();
#define m68k_GET_SOURCE_L_NOT_A m68kGetSourceLongNotA();


///////////
// Flags //
///////////

#define SR_C BIT_0
#define SR_V BIT_1
#define SR_Z BIT_2
#define SR_N BIT_3
#define SR_X BIT_4
#define SR_S BIT_d
#define SR_T BIT_f


// we define flags as booleans for quicker use
// flags or SR are updated as needed, on R/W SR
// the union allows changing more than one flag in one go

extern BYTE &pswT,&pswS,&pswI,&pswX,&pswN,&pswZ,&pswV,&pswC;

void update_sr_from_flags();
#define UPDATE_SR update_sr_from_flags()
void update_flags_from_sr();
#define UPDATE_FLAGS update_flags_from_sr()

#define SUPERFLAG (pswS)
#define SSP ((MEM_ADDRESS)((SUPERFLAG) ? sp:other_sp))
#define USP ((MEM_ADDRESS)((SUPERFLAG) ? other_sp:sp))
#define lpSSP ((MEM_ADDRESS*)((SUPERFLAG) ? &sp:&other_sp))
#define lpUSP ((MEM_ADDRESS*)((SUPERFLAG) ? &other_sp:&sp))
#define IOACCESS_FLAGS_MASK  0xFFFFFFC0
#define IOACCESS_FLAG_FOR_CHECK_INTRS BIT_6
#define IOACCESS_FLAG_FOR_CHECK_INTRS_MFP_CHANGE BIT_10
#define IOACCESS_FLAG_DELAY_MFP BIT_11
#define IOACCESS_INTERCEPT_OS BIT_12

#define CCR WORD_B_0(&SR)
#ifndef SSE_LEAN_AND_MEAN
#define SR_VALID_BITMASK 0xa71f
#endif
extern DU64 uflags;
#define PSWC pswC
#define PSWV pswV
#define PSWZ pswZ
#define PSWN pswN
#define PSWX pswX
#define PSWI pswI
#define PSWS pswS
#define PSWT pswT

#define CC_T (true)
#define CC_F (false)
#define CC_HI (!pswC&&!pswZ)
#define CC_LS (pswC||pswZ)
#define CC_CC (!pswC)
#define CC_CS (pswC)
#define CC_NE (!pswZ)
#define CC_EQ (pswZ)
#define CC_VC (!pswV)
#define CC_VS (pswV)
#define CC_PL (!pswN)
#define CC_MI (pswN)
#define CC_GE (pswV^!pswN)
#define CC_LT (pswV^pswN)
#define CC_GT (!pswZ&&(pswV^!pswN))
#define CC_LE (pswZ||(pswV^pswN))

#define SET_C pswC=1
#define CLEAR_C pswC=0
#define SET_N pswN=1
#define CLEAR_N pswN=0
#define SET_V pswV=1
#define CLEAR_V pswV=0
#define SET_X pswX=1
#define CLEAR_X pswX=0
#define SET_Z pswZ=1
#define CLEAR_Z pswZ=0
#define SET_S pswS=1
#define CLEAR_S pswS=0
#define SET_T pswT=1
#define CLEAR_T pswT=0

#if defined(BIG_ENDIAN_PROCESSOR)
#define CLEAR_VC CLEAR_V,CLEAR_C
#define CLEAR_NVC_SET_Z CLEAR_N,CLEAR_V,CLEAR_C,SET_Z
//#define CLEAR_C_SET_NV CLEAR_C,SET_N,SET_V
#define CLEAR_XNC CLEAR_X,CLEAR_N,CLEAR_C
#define SET_XNC SET_X,SET_N,SET_C
#define SET_XC SET_X,SET_C
#define CLEAR_XC CLEAR_X,CLEAR_C
#define SET_NV SET_N,SET_V
//#define CLEAR_XNZVC CLEAR_X,CLEAR_N,CLEAR_Z,CLEAR_V,CLEAR_C
//#define CLEAR_NZVC CLEAR_N,CLEAR_Z,CLEAR_V,CLEAR_C
//#define CLEAR_NZV CLEAR_N,CLEAR_Z,CLEAR_V
#else
/*
tough to get right, but when it's done, it's an optimisation
XNZVC
BYTE &pswT=uflags.d8[7];  d16[3]L   d32[1]LL
BYTE &pswS=uflags.d8[6];  d16[3]H   d32[1]LH
BYTE &pswI=uflags.d8[5];  d16[2]L   d32[1]HL
BYTE &pswX=uflags.d8[4];  d16[2]H   d32[1]HH
BYTE &pswN=uflags.d8[3];  d16[1]L   d32[0]LL
BYTE &pswZ=uflags.d8[2];  d16[1]H   d32[0]LH
BYTE &pswV=uflags.d8[1];  d16[0]L   d32[0]HL
BYTE &pswC=uflags.d8[0];  d16[0]H   d32[0]HH
*/
#define CLEAR_VC uflags.d16[LO]=0
#define CLEAR_NVC_SET_Z uflags.d32[0]=0x00010000
//#define CLEAR_C_SET_NV  uflags.d32[0]&=0x01010100,uflags.d32[0]|=0x01000100
#define CLEAR_XNC       uflags.d64&=0x0101070000010100
#define SET_XNC         uflags.d64|=0x0000000101000001
#define SET_XC          uflags.d64|=0x0000000100000001
#define CLEAR_XC        uflags.d64&=0x0101070001010100
#define SET_NV          uflags.d32[0]|=0x01000100
//#define CLEAR_XNZVC     uflags.d64&=0x0101070000000000
//#define CLEAR_NZVC      uflags.d32[0]&=0
//#define CLEAR_NZV       uflags.d32[0]&=0x00000001
#endif

inline void change_to_user_mode() {
  compare_buffer=sp;sp=other_sp;other_sp=compare_buffer;
#ifndef SSE_LEAN_AND_MEAN
  CLEAR_S; // already done anyway
#endif
}


inline void change_to_supervisor_mode() {
  compare_buffer=sp;sp=other_sp;other_sp=compare_buffer;
  SET_S;
}

#define DETECT_CHANGE_TO_USER_MODE if (!SUPERFLAG) change_to_user_mode();


///////////////
// Peek/Poke //
///////////////

// pointers
extern BYTE (*m68k_peek)(MEM_ADDRESS ad);
extern WORD (*m68k_dpeek)(MEM_ADDRESS ad);
extern WORD (*m68k_fetch)(MEM_ADDRESS ad);
extern void (*m68k_poke_abus)(BYTE x);
extern void (*m68k_dpoke_abus)(WORD x);

// functions
BYTE m68k_peek_st(MEM_ADDRESS ad);
WORD m68k_dpeek_st(MEM_ADDRESS ad);
WORD m68k_fetch_st(MEM_ADDRESS ad);
DWORD m68k_lpeek(MEM_ADDRESS ad);

void m68k_poke_abus0(BYTE x);
void m68k_dpoke_abus0(WORD x);
void m68k_lpoke_abus(LONG x);

#if defined(SSE_OPTION_FASTLINEA)
BYTE m68k_peek_st_lineA(MEM_ADDRESS ad);
WORD m68k_dpeek_st_lineA(MEM_ADDRESS ad);
WORD m68k_fetch_st_lineA(MEM_ADDRESS ad);
void m68k_poke_abus_lineA(BYTE x);
void m68k_dpoke_abus_lineA(WORD x);
#endif


inline void m68k_poke(MEM_ADDRESS ad,BYTE x) {
  iabus=ad;
  m68k_poke_abus(x);
}


inline void m68k_dpoke(MEM_ADDRESS ad,WORD x) {
  iabus=ad;
  m68k_dpoke_abus(x);
}


void m68k_lpoke(MEM_ADDRESS ad,LONG x); // lpoke now used only by utility functions, not CPU emulation proper


inline void m68k_push_w(WORD x) {
  sp-=2;
  iabus=sp;
  m68k_dpoke_abus(x);
}

#define m68k_PUSH_W(x) m68k_push_w(x)


void m68k_push_l_without_timing(DWORD x);

#define m68k_PUSH_L(x) m68k_push_l_without_timing(x)


/////////////////////
// Counting cycles //
/////////////////////

/*
Note on rounding:
You need "round cycles up to 4" only for RAM and Shifter accesses.
Funny but this was already in the Engineering Hardware Specification of 1986:

     ---------------
    | MC68000 MPU   |<--
    |               |   |
     ---------------    |
                        |                           ----------
                        |<------------------------>|192 Kbyte |<--->EXPAN
               ---------|------------------------->| ROM      |
              |         |                           ----------
              |         |                           ----------
              |         |                          |512K or 1M|  
              |         |                       -->| byte RAM |<--
      ----------        |        ----------    |    ----------    |
     | Control  |<----->|<----->| Memory   |<--                   |
     | Logic    |-------|------>|Controller|<--                   |
      ----------        |        ----------    |    ----------    |
       |||||            |        ----------     -->| Video    |<--  RF MOD
       |||||            |<----->| Buffers  |<----->| Shifter  |---->RGB
       |||||            |       |          |        ----------      MONO
       |||||            |        ----------
       |||||            |        ----------         ----------
       |||||            |<----->| MC6850   |<----->| Keyboard |<--->IKBD
       |||| ------------|------>| ACIA     |       | Port     |
       ||||             |        ----------         ----------
       ||||             |        ----------         ----------
       ||||             |<----->| MC6850   |<----->| MIDI     |---->OUT/THRU
       ||| -------------|------>| ACIA     |       | Ports    |<----IN
       |||              |        ----------         ----------
       |||              |        ----------         ----------
       |||              |<----->| MK68901  |<----->| RS232    |<--->MODEM
       || --------------|------>| MFP      |<--    | Port     |
       ||               |        ----------    |    ----------
       ||               |                      |    ----------
       ||               |                       ---| Parallel |<--->PRINTER
       ||               |                       -->| Port     |
       ||               |        ----------    |    ----------
       ||               |<----->| YM-2149  |<--     ----------
       | ---------------|------>| PSG      |------>| Sound    |---->AUDIO
       |                |       |          |---    | Channels |
       |                |        ----------    |    ----------
       |                |                      |    ----------
       |                |        ----------     -->| Floppy   |<--->FLOPPY
       |                |<------| WD1772   |<----->|Disk Port |     DRIVE
       |                |    -->| FDC      |        ----------
       |                |   |    ----------
       |                |   |    ----------         ----------
       |                |    -->| DMA      |<----->|Hard Disk |<--->HARD
       |                |<----->|Controller|       | Port     |     DRIVE
        ----------------------->|          |        ----------
                                 ----------
*/

extern DWORD CpuNormalHz;
extern COUNTER_VAR a_s_t; // to be updated with ABSOLUTE_SYS_TIME and used as appropriate // was act
void ChangeTimingFunctions(); // to be called on config change

// pointers to functions
extern void (*pInstructionTimeIdle)(int t);   // n*t
extern void (*pInstructionTimeWS)(int t);     // for waitstates (bus busy)
extern void (*pInstructionTimePrefetchOnly)();   // PREFETCH_ONLY (fetch IRC at pc)
extern void (*pInstructionTimePrefetchFinal)();  // PREFETCH_FINAL (IRC->IR + fetch IRC at au=pc+2)
extern void (*pInstructionTimePrefetchTotal)();  // PREFETCH (do pc+2, fetch IRC at pc)
extern void (*pInstructionTimeReadB)();       // CPU read byte
extern void (*pInstructionTimeRead)();        // CPU read word
extern void (*pInstructionTimeWrite)();       // CPU write word
extern void (*pInstructionTimeWriteB)();      // CPU write byte
extern void (*pInstructionTimeBltRead)();     // Blitter read word
extern void (*pInstructionTimeBltWrite)();    // Blitter write word

// macros actually used
#define CPU_BUS_IDLE(t)               pInstructionTimeIdle((t)*TICKS8) // max is 8
#define PREFETCH                      pInstructionTimePrefetchTotal();
#define PREFETCH_FINAL                pInstructionTimePrefetchFinal();
#define PREFETCH_ONLY                 pInstructionTimePrefetchOnly();
#define CPU_BUS_ACCESS_READ           pInstructionTimeRead()
#define CPU_BUS_ACCESS_READ_B         pInstructionTimeReadB()
#define CPU_BUS_ACCESS_WRITE          pInstructionTimeWrite()
#define CPU_BUS_ACCESS_WRITE_B        pInstructionTimeWriteB()
#define BLT_BUS_ACCESS_READ           pInstructionTimeBltRead()
#define BLT_BUS_ACCESS_WRITE          pInstructionTimeBltWrite()
#define DMA_BUS_ACCESS_READ           pInstructionTimeRead()
#define DMA_BUS_ACCESS_WRITE          pInstructionTimeWrite() //temp
#ifdef DEBUG_BUILD
#define BUS_JAM_TIME(t) if(stem_runmode==STEM_MODE_CPU){pInstructionTimeWS((t)*TICKS8);a_s_t=A_S_T;}
#else
#define BUS_JAM_TIME(t) {pInstructionTimeWS((t)*TICKS8);a_s_t=A_S_T;}
#endif

// specialised timing functions (also see interface_stvl)
// STF
void InstructionTimeStfIdle(int t);
void InstructionTimeStfWS(int t);
void InstructionTimeStfPrefetchOnly();
void InstructionTimeStfPrefetchFinal();
void InstructionTimeStfPrefetchTotal();
void InstructionTimeStfReadB();
void InstructionTimeStfRead();
void InstructionTimeStfWriteB();
void InstructionTimeStfWriteW();
//STE
void InstructionTimeSteIdle(int t);
void InstructionTimeSteWS(int t);
void InstructionTimeStePrefetchOnly();
void InstructionTimeStePrefetchFinal();
void InstructionTimeStePrefetchTotal();
void InstructionTimeSteReadB();
void InstructionTimeSteRead();
void InstructionTimeSteWriteB();
void InstructionTimeSteWriteW();
void InstructionTimeSteBltRead();
void InstructionTimeSteBltWrite();
//MEGASTE
#if defined(SSE_MEGASTE)
void InstructionTimeMegaSteIdle(int t);
void InstructionTimeMegaSteWS(int t);
void InstructionTimeMegaStePrefetchOnly();
void InstructionTimeMegaStePrefetchFinal();
void InstructionTimeMegaStePrefetchTotal();
void InstructionTimeMegaSteReadB();
void InstructionTimeMegaSteRead();
void InstructionTimeMegaSteWriteB();
void InstructionTimeMegaSteWriteW();
#endif


/////////////
// Opcodes //
/////////////

void m68kSetPC(MEM_ADDRESS ad,int count_timing=0);
extern void (*m68k_call_table[0xffff+1])(); // 65536 function pointers
void cpu_routines_init();
#define SET_PC(ad) m68kSetPC(ad,0); // use out of running emu

inline void  m68k_new_pc(MEM_ADDRESS ad) {
  pc_high_byte=ad&0xFF000000; // we keep this updated for snapshot compatibility?
  pc=ad;
}
#define NEW_PC(ad) m68k_new_pc(ad)


inline void m68k_get_immediate_l_with_timing() {
  m68k_src_lh=IRC;
  PREFETCH; //np
  m68k_src_ll=IRC;
  PREFETCH; //np
}
#define m68k_GET_IMMEDIATE_L_WITH_TIMING m68k_get_immediate_l_with_timing()


inline void m68k_write_long_result_backwards() {
  dbus=resultl;
  CPU_BUS_ACCESS_WRITE; //nw
  iabus-=2;
  dbus=resulth;
  CPU_BUS_ACCESS_WRITE; //nW
}
#define WRITE_LONG_BACKWARDS m68k_write_long_result_backwards()


inline void m68k_push_l_with_timing(DU32 &x) {
  AREG(7)-=4;
  iabus=AREG(7);
  dbus=x.d16[HI];
  CPU_BUS_ACCESS_WRITE; // nS
  iabus+=2;
  dbus=x.d16[LO];
  CPU_BUS_ACCESS_WRITE; // ns
}
#define m68k_PUSH_L_WITH_TIMING(x) m68k_push_l_with_timing(x)


#if defined(SSE_VC_INTRINSICS)
extern int (*count_bits_set_in_word)(unsigned short); // pointer to function
int count_bits_set_in_word2(unsigned short w);
#endif

//see 3rdparty/pasti, we made no h
extern "C" unsigned getDivu68kCycles( DWORD dividend, WORD divisor);
extern "C" unsigned getDivs68kCycles( LONG dividend, short divisor);

void m68kPerformRte(); // only used by utilities

inline void TMC68000::m68kUnstop() {
  if(ProcessingState==STOPPED)
  {
    ProcessingState=NORMAL; // will be exception soon
#ifdef DEBUG_BUILD
    m68kSetPC(pc+4,false); 
#endif
  }
}
#define M68K_UNSTOP Cpu.m68kUnstop();


///////////
// Debug //
///////////

#ifdef DEBUG_BUILD
#include "debug_emu.h"
void log_history(int bombs,MEM_ADDRESS crash_address);

/*
This is for the "stop on change to user mode" and "stop on next program run" options.
Whenever an instruction that can change SR is executed we check for SUPER change. If
a change to user mode has happened then stop_on_user_change is set to 2. Then when we
check for interrupts (after the current instruction) we see if an interrupt has happened
and if it hasn't we stop. If it has happened we return stop_on_user_change to 1 and wait
for the next change to user mode (when the interrupt has finished).
*/

#define CHECK_STOP_ON_USER_CHANGE  \
            if(stop_on_user_change && !PSWS && (debug_old_sr&SR_S)) \
              stop_on_user_change=2;

#define CHECK_STOP_USER_MODE_NO_INTR \
            if(stop_on_user_change==2) \
            { \
              if(!PSWS) \
              { \
                if(runstate==RUNSTATE_RUNNING) \
                { \
                  runstate=RUNSTATE_STOPPING; \
                  SET_WHY_STOP( HEXSl(old_pc,6)+": Switch to user mode" ) \
                } \
                if(stop_on_next_program_run==2) \
                  stop_new_program_exec(); \
              } \
              else \
                stop_on_user_change=1; \
            } \
            else if(stop_on_next_reset==2) \
            { \
              if(runstate==RUNSTATE_RUNNING) \
              { \
                runstate=RUNSTATE_STOPPING; \
                SET_WHY_STOP( HEXSl(old_pc,6)+": Reset" ) \
                stop_on_next_reset=1; /*sticky for the moment*/ \
              } \
            } 

#define PARAM_N ((IRD&BITS_ba9)>>9)
#define PARAM_M (IRD&0x7)

void DebugCheckIOAccess();
#define DEBUG_CHECK_IOACCESS DebugCheckIOAccess();

#else

#define CHECK_STOP_ON_USER_CHANGE
#define CHECK_STOP_USER_MODE_NO_INTR
#define debug_check_break_on_irq(irq)
#define PARAM_N rx // must also keep PARAM_N above for debugger!
#define PARAM_M ry // must also keep PARAM_M above for debugger!

#define DEBUG_CHECK_IOACCESS

#endif//DEBUG_BUILD?

#ifdef ENABLE_LOGFILE
#define IOACCESS_DEBUG_MEM_DMA BIT_13
#define IOACCESS_DEBUG_MEM_WRITE_LOG BIT_14
#define IOACCESS_DEBUG_MEM_READ_LOG BIT_15
extern MEM_ADDRESS debug_mem_write_log_address;
extern int debug_mem_write_log_bytes;
#endif

#endif//#ifndef CPU_DECLA_H
