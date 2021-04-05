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

DOMAIN: Debug
FILE: debug.h
DESCRIPTION: Declarations for debug facilities, like ASSERT, TRACE...
The Debug object and some debug facilities are used in all builds (release
too).
Debugger: fake IO declarations
struct TDebug
This file is used by 6301.c.
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSEDEBUG_H
#define SSEDEBUG_H

#include "SSE.h"

#ifdef UNIX
#include "../pch.h"
#include <assert.h>
#endif

#if defined(__cplusplus)
#ifdef WIN32
#include <windows.h>
#endif
#include "conditions.h"
#include <easystr.h>
#endif

#include "parameters.h"
#include <stdio.h>


#ifdef MINGW_BUILD
#define NULL 0
#endif

#if defined(SSE_DEBUG) // Debugger build or ide debug build
// general use debug variables;
extern 
#ifdef __cplusplus
"C" 
#endif//c++
int debug0,debug1,debug2,debug3,debug4,debug5,debug6,debug7,debug8,debug9;
#endif

// a structure that may be used by C++ and C objects
#pragma pack(push, 8)

struct TDebug {
#ifdef __cplusplus 
  // ENUM 
  enum EDebug {INIT0,INIT,LOAD,RESET,START,STOP,EXIT};
#endif
  // FUNCTIONS
#ifdef __cplusplus 
  TDebug();
  ~TDebug();
#if defined(SSE_DEBUG_TRACE)
  void FlushTrace();
  void TraceInit();
  void Trace(char *fmt,...); // one function for both IDE & file
  // if logsection enabled, static for function pointer, used in '6301':
  static void TraceLog(char *fmt,...); 
#endif
  void TraceGeneralInfos(int when);
  void Vbl();
  void Reset(bool Cold);
#if defined(SSE_DEBUGGER_TRACE_EVENTS)
  void TraceEvent( void* pointer);
#endif
#endif//C++
  // DATA
  char trace_buffer[MAX_TRACE_CHARS];
#if defined(SSE_DEBUG_TRACE_LOCK)
  HANDLE trace_file_pointer;
#else
  FILE *trace_file_pointer;
#endif
  int ShifterTricks; // used by Stats too
#if defined(SSE_OSD_FPS_INFO)
  DWORD frame_checksum;
  DWORD vbase_at_vbi,vcount_at_vsync;
  BYTE frame_no_change;
#endif
#if defined(SSE_DEBUG)
  int nTrace;
  int LogSection;
  WORD nHbis; // counter for each frame
  BYTE logsection_enabled[100];
#endif
#if defined(SSE_DEBUGGER)
  HWND dbg_timer_hwnd[4]; //to record WIN handles
  WORD FrameMfpIrqs;
  BYTE FrameInterrupts; //bit0 VBI 1 HBI 2 MFP
  BYTE IgnoreErrors;
  BYTE DialogOnStopEvent;
  BYTE log_in_trace,trace_in_log,tracing_suspended;
  WORD MonitorValue;
  BYTE MonitorValueSpecified; // Debugger SSE option
  BYTE MonitorComparison; // as is, none found = 0 means no value to look for
  BYTE MonitorRange; //check from ad1 to ad2
#endif
#if defined(SSE_DEBUGGER_FAKE_IO)
/*  Hack. A free zone in IO is mapped to an array of masks to control 
    a lot of debug options using the Debugger's built-in features.
    Memory browsers display words so we use words even if bytes are
    handier (less GUI clutter).
*/
  WORD ControlMask[FAKE_IO_LENGTH];
#endif
#if defined(SSE_HD6301_LL)
  BYTE HD6301RamBuffer[256+8];
#endif
};

extern 
#ifdef __cplusplus
"C" 
#endif
struct TDebug Debug;

#pragma pack(pop)

#ifdef __cplusplus
extern EasyStr sTraceFile;
#endif


#ifdef __cplusplus
extern MEM_ADDRESS aes_intout,vdi_intout;
#if defined(DEBUG_BUILD)
// to place Steem Debugger breakpoints in the code
void debug_set_bk(unsigned long ad,bool set); //bool set
#define BREAK(ad) debug_set_bk(ad,true)
#else
#define BREAK(ad)
#endif
#endif//#ifdef __cplusplus

#if defined(SSE_DEBUG)
/* We replace defines (that were in acc.h but also other places!) with an enum
   and we change some of the sections to better suit our needs.
   We use the same sections to control our traces in the Debugger, menu log.
   So, Steem has a double log system!
   It came to be because:
   - unfamiliar with the log system
   - familiar with a TRACE system (like in MFC) that can output in the IDE
   - the log system would choke the computer (each Shifter trick written down!)
*/

enum logsection_enum_tag {
 LOGSECTION_ALWAYS,
 LOGSECTION_FDC,
 LOGSECTION_DMA,
 LOGSECTION_PASTI,
 LOGSECTION_IMAGE_INFO,
 LOGSECTION_IO ,
 LOGSECTION_INTERRUPTS ,
 LOGSECTION_TRAP ,
 LOGSECTION_MMU,
 LOGSECTION_MFP_TIMERS ,
 LOGSECTION_MFP=LOGSECTION_MFP_TIMERS,
 
 LOGSECTION_CRASH ,
 LOGSECTION_STEMDOS ,
 LOGSECTION_ACIA,
 LOGSECTION_IKBD ,
 LOGSECTION_MIDI ,
 LOGSECTION_VIDEO ,
 LOGSECTION_BLITTER ,
 LOGSECTION_TRACE ,
 LOGSECTION_CPU ,
 LOGSECTION_CARTRIDGE,
 LOGSECTION_INIT ,
 LOGSECTION_INIFILE ,
 LOGSECTION_SHUTDOWN ,
 LOGSECTION_SPEEDLIMIT ,
 LOGSECTION_GUI ,
 LOGSECTION_AGENDA ,
 LOGSECTION_OPTIONS, 
 LOGSECTION_VIDEO_RENDERING,
 LOGSECTION_SOUND ,
 NUM_LOGSECTIONS,
 };
#endif

#if defined(SSE_DEBUGGER_FAKE_IO)

#define OSD_MASK1 (Debug.ControlMask[2])
#define OSD_CONTROL_INTERRUPT               (1<<15)
#define OSD_CONTROL_IKBD                  (1<<14)
#define OSD_CONTROL_FDC              (1<<13)

#define OSD_MASK_CPU (Debug.ControlMask[3])
#define OSD_CONTROL_CPUTRACE           (1<<15)
#define OSD_CONTROL_CPUBOMBS  (1<<14)

#define OSD_MASK2 (Debug.ControlMask[4])
#define OSD_CONTROL_SHIFTERTRICKS           (1<<15)
#define OSD_CONTROL_MODES (1<<14)

#define TRACE_MASK0 (Debug.ControlMask[5]) // trace level
#define TRACE_LEVEL2 (1<<15)
#define TRACE_LEVEL3 (1<<14)

#define TRACE_MASK1 (Debug.ControlMask[6]) //Glue
#define TRACE_CONTROL_VERTOVSC (1<<15)

#define TRACE_MASK2 (Debug.ControlMask[7])
#define TRACE_CONTROL_IRQ_TA (1<<15) //timer A
#define TRACE_CONTROL_IRQ_TB (1<<14) //timer B
#define TRACE_CONTROL_IRQ_TC (1<<13) //timer C
#define TRACE_CONTROL_IRQ_TD (1<<12) //timer D
#define TRACE_CONTROL_ECLOCK (1<<11)
#define TRACE_CONTROL_IRQ_SYNC (1<<10) //vbi, hbi
#define TRACE_CONTROL_RTE (1<<9)
#define TRACE_CONTROL_EVENT (1<<8)

#define TRACE_MASK3 (Debug.ControlMask[8])
#define TRACE_CONTROL_FDCSTR (1<<15)
#define TRACE_CONTROL_FDCBYTES (1<<14)//no logsection needed
#define TRACE_CONTROL_FDCPSG (1<<13)//drive/side
#define TRACE_CONTROL_FDCREGS (1<<12)// writes to registers CR,TR,SR,DR
#define TRACE_CONTROL_FDCWD (1<<11) // for Steem's 2nd wd1772 emu, more details
#define TRACE_CONTROL_FDCMFM (1<<10)

#define DEBUGGER_CONTROL_MASK1 (Debug.ControlMask[9])
#define DEBUGGER_CONTROL_LARGE_HISTORY (1<<15)
#define DEBUGGER_CONTROL_HISTORY_TMG (1<<14)

#define SOUND_CONTROL_MASK (Debug.ControlMask[10])
#define SOUND_CONTROL_OSD (1<<9)//first entries other variables

#define DEBUGGER_CONTROL_MASK2 (Debug.ControlMask[11])
#define DEBUGGER_CONTROL_NEXT_PRG_RUN (1<<15)
#define DEBUGGER_CONTROL_TOPOFF (1<<14)
#define DEBUGGER_CONTROL_BOTTOMOFF (1<<13)
#define DEBUGGER_CONTROL_6301 (1<<12) // custom prg run

#define TRACE_MASK_IO (Debug.ControlMask[12])
#define TRACE_CONTROL_IO_W (1<<15)
#define TRACE_CONTROL_IO_R (1<<14)

#define TRACE_MASK4 (Debug.ControlMask[13]) //cpu
#define TRACE_CONTROL_CPU_REGISTERS (1<<15) 
#define TRACE_CONTROL_CPU_CYCLES (1<<14) 
#define TRACE_CONTROL_CPU_LIMIT (1<<13)
#define TRACE_CONTROL_CPU_VALUES (1<<12)

#define TRACE_MASK_14 (Debug.ControlMask[14]) //Glue 2

#endif//#if defined(SSE_DEBUGGER_FAKE_IO)

// debug macros
extern BYTE FullScreen; // to avoid asserts in fullscreen

// ASSERT
#if defined(SSE_DEBUG)
#if defined(_DEBUG) && defined(VC_BUILD)
// Our ASSERT facility has no MFC dependency.
#if defined(SSE_X64_DEBUG)
#define ASSERT(x) {if(!(x) && !FullScreen) DebugBreak();}
#else
#define ASSERT(x) {if(!((x)) /*&& !FullScreen*/) _asm{int 0x03}}
#endif
#elif defined(SSE_UNIX_TRACE)
#define ASSERT(x) assert(x)
#elif defined(DEBUG_BUILD) // for Debugger
#ifdef __cplusplus
#define ASSERT(x) {if (!((x))) {TRACE("Assert failed: %s\n",#x); \
  if(!Debug.IgnoreErrors) { \
  debug9=MessageBox(0,#x,"ASSERT",MB_ICONWARNING|MB_ABORTRETRYIGNORE);   \
  if(debug9==IDABORT) exit(EXIT_FAILURE);\
  Debug.IgnoreErrors=(debug9==IDIGNORE);}}}
#endif//c++
#endif//vc
#else //!SSE_DEBUG
#define ASSERT(x)
#endif

// BREAKPOINT 
#if defined(SSE_DEBUG)
#if defined(_DEBUG) && defined(VC_BUILD)
#if defined(SSE_X64_DEBUG)
#define BREAKPOINT {DebugBreak();}
#else
#define BREAKPOINT _asm { int 3 }
#endif
#elif defined(SSE_UNIX_TRACE)
#define BREAKPOINT {assert(0)}
#elif defined(DEBUG_BUILD) // for Debugger
#ifdef __cplusplus
#define BREAKPOINT {if(!Debug.IgnoreErrors) { \
  TRACE("Breakpoint\n"); \
  Debug.IgnoreErrors=!(MessageBox(0,"no message","Breakpoint",MB_ICONWARNING|MB_OKCANCEL)==IDOK);}}
#endif//c++
#endif
#else //!SSE_DEBUG
#define BREAKPOINT {}
#endif

// BRK(x) 
#if defined(SSE_DEBUG)

#if defined(DEBUG_BUILD)

#ifdef __cplusplus
#define BRK(x){if(!Debug.IgnoreErrors) { \
  TRACE("Breakpoint: %s\n",#x); \
  Debug.IgnoreErrors=!(MessageBox(0,#x,"Breakpoint",MB_ICONWARNING|MB_OKCANCEL)==IDOK);}}
#endif//c++

#elif defined(SSE_UNIX_TRACE)

#define BRK(x) {TRACE("BRK %s\n",#x);assert(0);}

#elif defined(VC_BUILD)

#if defined(SSE_X64_DEBUG)
#define BRK(x) {DebugBreak();}
#else
#define BRK(x) {TRACE("BRK %s\n",#x); _asm { int 3 } }
#endif

#endif

#else //!SSE_DEBUG

#define BRK(x)

#endif

// TRACE
#if defined(SSE_DEBUG_TRACE) && defined(SSE_DEBUG) //395 no TRACE for release mode
#ifdef __cplusplus
#define TRACE Debug.Trace
#endif//c++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE(x,...)
#else
#define TRACE
#endif
#endif//#if defined(SSE_DEBUG_TRACE) 

// TRACE_ENABLED
#if defined(DEBUG_BUILD) // Debugger
#if defined(_DEBUG) // IDE
//#define TRACE_ENABLED (Debug.logsection_enabled[LOGSECTION] || LOGSECTION<NUM_LOGSECTIONS && logsection_enabled[LOGSECTION])
#define TRACE_ENABLED(section) (Debug.logsection_enabled[section] || section<NUM_LOGSECTIONS && logsection_enabled[section])
#else // no IDE but Debugger
//#define TRACE_ENABLED (Debug.logsection_enabled[LOGSECTION] || LOGSECTION<NUM_LOGSECTIONS && logsection_enabled[LOGSECTION])
#define TRACE_ENABLED(section) (Debug.logsection_enabled[section] || section<NUM_LOGSECTIONS && logsection_enabled[section])
#endif
#else // no Debugger 
#if defined(_DEBUG) // IDE
//#define TRACE_ENABLED (Debug.logsection_enabled[LOGSECTION])
#define TRACE_ENABLED(section) (Debug.logsection_enabled[section])
#else // 3rd party objects
#define TRACE_ENABLED (0)
#endif
#endif

// TRACE_LOG
#if defined(SSE_DEBUG)
#ifdef __cplusplus
#define TRACE_LOG Debug.LogSection=LOGSECTION, Debug.TraceLog //!
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_LOG(x,...)
#else
#define TRACE_LOG
#endif
#endif

#if defined(DEBUG_BUILD)
#ifdef __cplusplus
#define TRACE_LOG2 if((TRACE_MASK0&TRACE_LEVEL2)) Debug.LogSection=LOGSECTION,Debug.TraceLog
#define TRACE_LOG3 if((TRACE_MASK0&TRACE_LEVEL3)) Debug.LogSection=LOGSECTION,Debug.TraceLog
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_LOG2(x,...)
#define TRACE_LOG3(x,...)
#else
#define TRACE_LOG2
#define TRACE_LOG3
#endif
#endif



// v3.6.3 introducing more traces,  verbose here, short in code
// TRACE_FDC
#if defined(SSE_DEBUG)
#ifdef __cplusplus
#define TRACE_FDC Debug.LogSection=LOGSECTION_FDC, Debug.TraceLog //!
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_FDC(x,...)
#else
#define TRACE_FDC
#endif
#endif


//#define TRACE_HDC TRACE_FDC //3.7.2
#define TRACE_HDC TRACE_LOG //4.1.0
//#define TRACE_HDC TRACE


// so we get extended trace only if 'wd' checked
#if defined(DEBUG_BUILD)
#ifdef __cplusplus
//#define TRACE_WD if((TRACE_MASK3 & TRACE_CONTROL_FDCWD) && (LOGSECTION==LOGSECTION_FDC)) Debug.LogSection=LOGSECTION,Debug.TraceLog
#define TRACE_WD if((TRACE_MASK3 & TRACE_CONTROL_FDCWD)) Debug.LogSection=LOGSECTION,Debug.TraceLog
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_WD(x,...)
#else
#define TRACE_WD
#endif
#endif

// TRACE_INIT 3.7.0
#if defined(SSE_DEBUG)
#ifdef __cplusplus
#define TRACE_INIT Debug.LogSection=LOGSECTION_INIT, Debug.TraceLog //!
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_INIT(x,...)
#else
#define TRACE_INIT
#endif
#endif

// TRACE_INT 3.7.0
#if defined(SSE_DEBUG)
#ifdef __cplusplus
#define TRACE_INT Debug.LogSection=LOGSECTION_INTERRUPTS, Debug.TraceLog //!
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_INT(x,...)
#else
#define TRACE_INT
#endif
#endif


// TRACE_MFP 3.7.0
/*#if defined(SSE_DEBUG)
#ifdef __cplusplus
#define TRACE_MFP Debug.LogSection=LOGSECTION_MFP, Debug.TraceLog //!
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_MFP(x,...)
#else
#define TRACE_MFP
#endif
#endif*/

// TRACE_MFM 3.7.1
#if defined(SSE_DEBUGGER_FAKE_IO) 
#ifdef __cplusplus
#define TRACE_MFM if(TRACE_MASK3&TRACE_CONTROL_FDCMFM) Debug.LogSection=LOGSECTION_FDC, Debug.TraceLog //!
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_MFM(x,...)
#else
#define TRACE_MFM
#endif
#endif

// TRACE_TOS 3.7.1
#if defined(SSE_DEBUG)
#ifdef __cplusplus
#define TRACE_TOS Debug.LogSection=LOGSECTION_STEMDOS, Debug.TraceLog //!
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_TOS(x,...)
#else
#define TRACE_TOS
#endif
#endif

// TRACE_VID 3.7.3
#if defined(SSE_DEBUG)
#ifdef __cplusplus
#define TRACE_VID Debug.LogSection=LOGSECTION_VIDEO, Debug.TraceLog //!
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_VID(x,...)
#else
#define TRACE_VID
#endif
#endif

// TRACE_VID_R 3.9.3
#if defined(SSE_DEBUG)
#ifdef __cplusplus
#define TRACE_VID_R Debug.LogSection=LOGSECTION_VIDEO_RENDERING, Debug.TraceLog //!
#define TRACE_VID_RECT(rect) Debug.LogSection=LOGSECTION_VIDEO_RENDERING,Debug.TraceLog("%d %d %d %d\n",rect.left,rect.top,rect.right,rect.bottom)
#endif//C++
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_VID_R(x,...)
#define TRACE_VID_RECT(rect)
#else
#define TRACE_VID_R
#define TRACE_VID_RECT
#endif
#endif

#if defined(SSE_DEBUGGER_TRACE_EVENTS) //3.8.0
#define TRACE_EVENT(x) Debug.TraceEvent(x)
#else
#define TRACE_EVENT(x) 
#endif

// OSD
#if defined(SSE_DEBUG)
#define TRACE_OSD OsdControl.Trace // (...)
#elif defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE_OSD(x,...)
#else
#define TRACE_OSD //?
#endif

#define TRACE_OSD2 OsdControl.Trace

// TRACE_RECT 3.9.2
#define TRACE_RECT(rect) TRACE("%d %d %d %d\n",rect.left,rect.top,rect.right,rect.bottom)
#define TRACE_OSD_RECT(rect) TRACE_OSD("%d %d %d %d",rect.left,rect.top,rect.right,rect.bottom)

// VERIFY
#if defined(SSE_DEBUG)
#if defined(_DEBUG) && defined(VC_BUILD)
// Our VERIFY facility has no MFC dependency.
#if defined(SSE_X64_DEBUG)
#define VERIFY(x) {if(!(x) && !FullScreen) DebugBreak();}
#else
#define VERIFY(x) {if(!((x)) && !FullScreen) _asm{int 0x03}}
#endif
#elif defined(SSE_UNIX_TRACE)
#define VERIFY(x) if (!(x)) {TRACE("Verify failed: %s\n",#x); assert(0);} 
#elif defined(DEBUG_BUILD) // for Debugger
#ifdef __cplusplus
#define VERIFY(x) {if (!(x)) {TRACE("Verify failed: %s\n",#x); \
  if(!Debug.IgnoreErrors) { \
  debug9=MessageBox(0,#x,"VERIFY",MB_ICONWARNING|MB_ABORTRETRYIGNORE);   \
  if(debug9==IDABORT) exit(EXIT_FAILURE);\
  Debug.IgnoreErrors=(debug9==IDIGNORE);}}}
#endif//C++
#endif
#else //!SSE_DEBUG
#define VERIFY(x) ((void)(x))
#endif


#if !defined(SSE_DEBUG) 
enum { // to pass compilation
 LOGSECTION_FDC_BYTES, // was DIV
 LOGSECTION_IMAGE_INFO, //was Pasti
 LOGSECTION_OPTIONS,
 };
#endif

#if defined(SSE_DEBUGGER_FRAME_REPORT)
#define REPORT_LINE FrameEvents.ReportLine()
#else
#define REPORT_LINE
#endif

#if defined(SSE_DEBUG_TRACE)
#define TRACE2 Debug.Trace
#define FLUSH_TRACE Debug.FlushTrace()
#else
#if defined(VC_BUILD) || defined(SSE_UNIX)
#define TRACE2(x,...)
#else
#define TRACE2
#endif
#define FLUSH_TRACE
#endif

#ifdef SSE_BETA
#define TRACE3 Debug.Trace
#else
#define TRACE3(x,...)
#endif

#if defined(__cplusplus)
#ifdef ENABLE_LOGFILE

#define DBG_LOG(s)  \
   {if(logsection_enabled[LOGSECTION]){ \
      if(!logging_suspended){            \
        log_write(s);                \
      }                               \
   }}

#define log_stack  \
   {if(logsection_enabled[LOGSECTION]){ \
      if(!logging_suspended){            \
        log_write_stack();                 \
      }                               \
   }}

void log_write(EasyStr);
void log_os_call(int trap);

#define log_to_section(section,s) if (logsection_enabled[section] && logging_suspended==0) log_write(s);
#define LOG_TO(section,s)  if (logsection_enabled[section] && logging_suspended==0) log_write(s);
void log_write_stack();

extern bool logging_suspended;
extern bool logsection_enabled[100];
void log_io_write(MEM_ADDRESS,BYTE);

#define CPU_INSTRUCTIONS_TO_LOG 10000
extern int log_cpu_count;
void stop_cpu_log();
Str scanline_cycle_log();

#if defined(SSE_DEBUGGER_FAKE_IO)
#define LOG_CPU \
    if(log_cpu_count) { \
      log_to_section(LOGSECTION_CPU,HEXSl(pc,6)+": "+disa_d2(pc)); \
      if(TRACE_MASK4&TRACE_CONTROL_CPU_LIMIT) \
        if((--log_cpu_count)==0) stop_cpu_log(); \
    }
#elif defined(DEBUG_BUILD)
#define LOG_CPU \
    if (log_cpu_count){ \
      log_to_section(LOGSECTION_CPU,HEXSl(pc,6)+": "+disa_d2(pc)); \
      if ((--log_cpu_count)==0) stop_cpu_log(); \
    }
#else
#define LOG_CPU
#endif

struct struct_logsection {
  char *Name;
  int Index;
};

extern struct_logsection logsections[NUM_LOGSECTIONS+8];
extern const char *name_of_mfp_interrupt[22];
extern const char* gemdos_calls[0x58];
extern const char* bios_calls[12];
extern const char* xbios_calls[40];
extern FILE *logfile;
extern EasyStr LogFileName;

#else

#define DBG_LOG(s)
#define log_stack ;
// s can be char* or EasyStr
//#ifdef UNIX
//#define log_write(s) printf(s);printf("\n");
//#else
#define log_write(s)
//#endif
#define log_io_write(a,b)
#define log_to_section(section,s)
#define LOG_TO(section,s)
#define log_write_stack() ;
#define LOG_CPU

#endif
#endif//#if defined(__cplusplus)
#define log_DELETE_SOON(s) log_write(s);


#endif//#ifndef SSEDEBUG_H
