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

FILE: conditions.h
MODULES: All
DESCRIPTION: Sets up all conditions that affect the Steem binary. 
See also pch.h, steemh.h, SSE.h and parameters.h
This should be better sorted.
---------------------------------------------------------------------------*/

//                         Conditional Defines for Steem


#pragma once
#ifndef CONDITIONS_H
#define CONDITIONS_H

#ifdef WIN32
#include <Windows.h>
#if !defined(NO_DEBUG_BUILD) && !defined(DEBUG_BUILD) && !defined(ONEGAME)
#define DEBUG_BUILD
#endif
#else
#include "notwindows.h"
#endif

// These should all be commented out for release!
#ifdef UNIX
//#define ENABLE_LOGFILE
#endif
//#define SHOW_WAVEFORM 8 // no D3D, still working
//#define DISABLE_STEMDOS // still working
//#define DISABLE_BLITTER // still working
//#define SHOW_DRAW_SPEED // still working
//#define WRITE_ONLY_SINE_WAVE // strange, Steem PSG still works with it TODO
//#define DRAW_ALL_ICONS_TO_SCREEN // Linux
//#define DRAW_TIMER_TO_SCREEN // Linux
//#define TRANSLATION_TEST
//#define NO_CRAZY_MONITOR // still working
//#define X_NO_PC_JOYSTICKS // Linux
//#define NO_XVIDMODE

// This was always on in the Steem v3.2 release but it is not compatible with modern X.
//#define ALLOW_XALLOCID

#if defined(DEBUG_BUILD) && 0 //&& defined(BCB_BUILD)
#define PEEK_RANGE_TEST
#endif

// These should be left in for release
#ifdef WIN32
#ifndef NO_PASTI    // would be a compilation directive
#define USE_PASTI 1 // it is enabled in 64bit builds just in case
#else
#define USE_PASTI 0
#endif
#endif

// These should always be left like this
#define SCREENS_PER_SOUND_VBL 1
#ifdef CYGWIN
#define NO_XVIDMODE
#endif

#ifdef DEBUG_BUILD
#define ENABLE_LOGFILE
#endif


//     Set up some standard functions/defines that some compilers don't


//                                Visual C++

#ifdef VC_BUILD
#define HMONITOR_DECLARED
#ifndef OBM_COMBO
#define OBM_COMBO 32738
#endif
#ifndef DIDEVTYPE_JOYSTICK
#define DIDEVTYPE_JOYSTICK 4
#endif
#define _argc __argc  // this is M$'s name
#define _argv __argv
// VC++ 6.0 (and below) scoping bugfix
#if defined(_MSC_VER) && _MSC_VER <= 1200
#define for if(0); else for
#endif
#if defined(_DEBUG) && defined(_MSC_VER) && (_MSC_VER == 1500) &&!defined(SSE_X64)
#if 0
#include <vld.h> // think it can sound false alerts
#else
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
    #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
    // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
    // allocations to be of _CLIENT_BLOCK type
#else
    #define DBG_NEW new
#endif
#endif
#endif
#endif


//                                GCC/MinGW

#if defined(MINGW_BUILD) || defined(UNIX)
#define ASMCALL /*this should be C calling convention*/
#ifdef UNIX
#define max(a,b) ((a)>(b) ? (a):(b))
#define min(a,b) ((a)>(b) ? (b):(a))
#define strcmpi strcasecmp

#if !defined(RPI)
extern Display *XD;
extern XContext cWinThis,cWinProc;
#endif

extern char **_argv;
extern int _argc;
extern void UnixOutput(char *Str);
#endif
#ifdef WIN32
#define _MINGW_INTS
#ifdef __cplusplus
extern "C" 
#endif
int ASMCALL int_16_2();
#define UDM_SETPOS32 UDM_SETPOS
#endif
#ifdef UNIX
char *itoa(int i,char *s,int radix);
#endif
char *ultoa(unsigned long l,char *s,int radix);
extern char strupr_convert_buf[256],strlwr_convert_buf[256];
char *strupr(char *s);
char *strlwr(char *s);
#endif


//                            Add more compilers here



#ifdef __cplusplus
//         Some nice macros to make porting easier (less #ifdefs)

#ifdef WIN32
#ifndef WIN_ONLY
#define WIN_ONLY(a) a
#endif
#define UNIX_ONLY(a)
#define SLASH "\\"
#define SLASHCHAR '\\'
typedef HWND WINDOWTYPE;
#define UnixOutput(a)
#endif

#ifdef UNIX
#ifndef WIN_ONLY
#define WIN_ONLY(a)
#endif
#define UNIX_ONLY(a) a
#define SLASH "/"
#define SLASHCHAR '/'
#ifndef RPI
typedef Window WINDOWTYPE;
#endif
//#define NO_PORTAUDIO
#endif

#ifdef DEBUG_BUILD
#define DEBUG_ONLY(s) s
#define NOT_DEBUG(s)
#else
#define DEBUG_ONLY(s)
#define NOT_DEBUG(s) s
#endif

#ifdef ENABLE_LOGFILE
#define LOG_ONLY(s) s
#else
#define LOG_ONLY(s)
#endif

#ifdef ONEGAME
#define ONEGAME_ONLY(s) s
#define NOT_ONEGAME(s)
#define DISABLE_STEMDOS
#define NO_RARLIB
#else
#define ONEGAME_ONLY(s)
#define NOT_ONEGAME(s) s
#endif

#ifndef ASMCALL
#ifdef WIN32
#define ASMCALL __cdecl
#else
#define ASMCALL
#endif

#endif

#ifndef VC_BUILD
#define SET_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
                       const GUID name={l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}
#else
#define SET_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)
#endif

#if defined(LO) || defined(HI) || defined(B0) || defined(B1)|| defined(B2)|| defined(B3)
#error ERROR
#endif

#ifndef BIG_ENDIAN_PROCESSOR

// Little endian: least significant byte , low mid byte, hi mid byte , most significant byte
#define LO 0
#define HI 1
#define B0 0
#define B1 1
#define B2 2
#define B3 3

#define MEM_DIR -1 // only used by debug_plugin_read_mem(),debug_plugin_write_mem()
#define MORE_SIGNIFICANT_BYTE_OFFSET 1
#define LPLOWORD(a) ((WORD*)(&a))
#define LPHIWORD(a) (((WORD*)(&a))+1)
// B_n - n=significance (0 is LSB, 3 is MSB)
#define DWORD_B_0(s) *( ((BYTE*)(s))   )
#define DWORD_B_1(s) *( ((BYTE*)(s)) +1)
#define DWORD_B_2(s) *( ((BYTE*)(s)) +2)
#define DWORD_B_3(s) *( ((BYTE*)(s)) +3)
#define DWORD_B(s,n) *( ((BYTE*)(s)) +(n))
#define DWORD_W_0(s) *( ((WORD*)(s))   )
#define DWORD_W_1(s) *( ((WORD*)(s)) +1)
#define lpDWORD_B_0(s)  ( ((BYTE*)(s)) )
#define lpDWORD_B_1(s)  ( ((BYTE*)(s)) +1)
#define lpDWORD_B_2(s)  ( ((BYTE*)(s)) +2)
#define lpDWORD_B_3(s)  ( ((BYTE*)(s)) +3)

#define WORD_B_0(s) *( ((BYTE*)(s))   )
#define WORD_B_1(s) *( ((BYTE*)(s)) +1)
#define WORD_B(s,n) *( ((BYTE*)(s)) +(n))
#define lpWORD_B_0(s)  ( ((BYTE*)(s))  )
#define lpWORD_B_1(s)  ( ((BYTE*)(s)) +1)

#else

// Big endian: most significant byte , hi mid byte, low mid byte , least significant byte
// never been tried
#define LO 1
#define HI 0
#define B0 3
#define B1 2
#define B2 1
#define B3 0


#define MEM_DIR 1
#define MORE_SIGNIFICANT_BYTE_OFFSET -1
#define LPHIWORD(a) ((WORD*)(&a))
#define LPLOWORD(a) (((WORD*)(&a))+1)
// B_n - n=significance (0 is LSB, 3 is MSB)
#define DWORD_B_3(s) *( ((BYTE*)(s))   )
#define DWORD_B_2(s) *( ((BYTE*)(s)) +1)
#define DWORD_B_1(s) *( ((BYTE*)(s)) +2)
#define DWORD_B_0(s) *( ((BYTE*)(s)) +3)
#define DWORD_B(s,n) *( ((BYTE*)(s)) +(3-(n)))
#define DWORD_W_1(s) *( ((WORD*)(s))   )
#define DWORD_W_0(s) *( ((WORD*)(s)) +1)
#define lpDWORD_B_3(s)  ( ((BYTE*)(s))   )
#define lpDWORD_B_2(s)  ( ((BYTE*)(s)) +1)
#define lpDWORD_B_1(s)  ( ((BYTE*)(s)) +2)
#define lpDWORD_B_0(s)  ( ((BYTE*)(s)) +3)

#define WORD_B_1(s) *( ((BYTE*)(s))   )
#define WORD_B_0(s) *( ((BYTE*)(s)) +1)
#define WORD_B(s,n) *( ((BYTE*)(s)) +(1-(n)))
#define lpWORD_B_1(s)  ( ((BYTE*)(s))   )
#define lpWORD_B_0(s)  ( ((BYTE*)(s)) +1)

#endif

#define MAKECHARCONST(a,b,c,d) (BYTE(a) | (BYTE(b) << 8) | (BYTE(c) << 16) | (BYTE(d) << 24))

#undef MAKEWORD
#undef MAKELONG // this version gives no warning in BCC contrary to the one in windef.h
#define MAKEWORD(l,h) ((WORD)(((WORD)(((BYTE)(l))) | (((WORD)((BYTE)(h))) << 8))))
#define MAKELONG(l,h) ((LONG)(((LONG)(((WORD)(l))) | (((DWORD)((WORD)(h))) << 16))))

//#define SWAPBYTES(Var) (Var=MAKEWORD(HIBYTE((Var)),LOBYTE((Var))))
//#define SWAPWORDS(Var) (Var=MAKELONG(HIWORD((Var)),LOWORD((Var))))

// These are so you can do MAKEBINW(b00000011,b11100000);
#define MAKEBINW(high,low) ((BYTE(high) << 8) | BYTE(low))
#define MAKEBINL(highest,high,low,lowest) \
         ( (BYTE(highest) << 24) | (BYTE(high) << 16) | \
           (BYTE(low) << 8) | BYTE(lowest) )

typedef unsigned long MEM_ADDRESS;


#ifdef WIN32
#ifdef __cplusplus
extern HINSTANCE Inst,&HInstance;
#endif
#endif

#endif

#if 1
// macros to avoid indenting all code and facilitate search (diskman, options_create)
#define ADVANCED_BEGIN if(OPTION_ADVANCED) {
#define ADVANCED_HACK_BEGIN if(OPTION_ADVANCED&&OPTION_HACKS) {
#define HACK_BEGIN if(OPTION_HACKS) {
#define NOT_ADVANCED_BEGIN if(!OPTION_ADVANCED) {
#else
#define ADVANCED_BEGIN if(1){
#define ADVANCED_HACK_BEGIN if(1){
#define HACK_BEGIN if(1){
#define NOT_ADVANCED_BEGIN if(0){
#endif

#define ADVANCED_ELSE  }else{
#define HACK_ELSE  }else{
#define ADVANCED_END   }
#define HACK_END   }



///////////////
// COMPILERS //
///////////////

#if defined(BCC_BUILD) 
// after x warnings, BCC stops compiling! It's less important now that more little
// cpp files are compiled
#pragma warn- 8004 
#pragma warn- 8010 // continuation character
#pragma warn- 8012
#pragma warn- 8019
#pragma warn- 8027
#pragma warn- 8057
#pragma warn- 8071
#endif


#if defined(SSE_UNIX) || defined(MINGW_BUILD)
#include <stdint.h>
#ifdef UNIX
#define FALSE 0
#define TRUE 1

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

#endif
#else // for CAPS, HFE
      // those should be in stdint.h, but it's C and not in every VC version
typedef signed __int8		int8_t;
typedef unsigned __int8		uint8_t;
typedef signed __int16		int16_t;
typedef unsigned __int16	uint16_t;
typedef signed __int32		int32_t;
typedef unsigned __int32	uint32_t;
typedef signed __int64		int64_t;
typedef unsigned __int64	uint64_t;

#endif



#if defined(VC_BUILD)

//#define _NO_CRT_STDIO_INLINE // for stdio.h

#if (_MSC_VER <= 1200) // <=VC6
#define GetWindowLongPtr GetWindowLong
#define GWLP_INSTANCE GWL_INSTANCE
#define GWLP_HINSTANCE GWL_HINSTANCE
#define LONG_PTR LONG
#define UDM_SETPOS32 UDM_SETPOS
#define M_PI 3.14159265358979323846
#endif

#pragma warning (disable : 4100) //unreferenced formal parameter
#pragma warning (disable : 4189) //local variable is initialized but not referenced
#pragma warning (disable : 4127) //conditional expression is constant
#if defined(SSE_DEBUGGER)
//#pragma warning (disable : 4125) //decimal digit terminates octal escape sequence
#else
//#pragma warning (disable : 4002) //too many actual parameters for macro 'TRACE_LOG'
//#pragma warning(disable : 4552)
#endif
#pragma warning (disable : 4611) //interaction between '_setjmp' and C++ object destruction is non-portable
//#pragma warning (disable : 4244) //conversion from 'int' to 'short', possible loss of data
// The POSIX name for this item is deprecated or This function or variable may be unsafe
#pragma warning (disable : 4996)
#endif

#if defined(SSE_VC_INTRINSICS)
#include <intrin.h>
#define BITTEST(var,bit) (_bittest((long*)&var,bit)/*!=0*/)
#define BITRESET(var,bit) (_bittestandreset((long*)&var,bit))
//#define BITSET(var,bit) (_bittestandset((long*)&var,bit))
#endif

// use PRICV for COUNTER_VAR type in TRACE
// for example:
// TRACE_LOG(PRICV " timeout, next " PRICV "\n",timeout1,timeout2);
#if defined(SSE_X64)
typedef __int64 COUNTER_VAR;
#define PRICV "%lld" // for TRACE
#else
typedef int COUNTER_VAR;
#define PRICV "%d"
#endif

#define MAX(a,b) ((a)>(b) ? (a):(b))
#define MIN(a,b) ((a)>(b) ? (b):(a))

#define ERR_OK 0

#endif//#ifndef CONDITIONS_H
