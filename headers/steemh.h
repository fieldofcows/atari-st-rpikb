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

DOMAIN: All
FILE: steemh.h
DESCRIPTION: miscellanous declarations
class SE_Exception
---------------------------------------------------------------------------*/

#pragma once
#ifndef STEEMH_DECLA_H
#define STEEMH_DECLA_H

#include "conditions.h"

////////////////
// EXCEPTIONS //
////////////////

#if defined(SSE_MAIN_LOOP2)
#ifdef WIN32
#ifdef __cplusplus
#include <eh.h>
/*  The idea is to report a SEH exception in a normal try/catch block.
    https://msdn.microsoft.com/en-us/library/5z4bw5h5(VS.80).aspx
*/
class SE_Exception //as adapted
{
public:
//private:
    unsigned int nSE;
public:
    EXCEPTION_POINTERS* m_pExp; 
    //SE_Exception() {}
    //SE_Exception( unsigned int n ) : nSE( n ) {}
    SE_Exception(unsigned int u, EXCEPTION_POINTERS* pExp) {
      nSE=u;
      m_pExp=pExp;
    }
    ~SE_Exception() {}
    //unsigned int getSeNumber() { return nSE; }
    void handle_exception();
};

void __cdecl trans_func(unsigned int u,EXCEPTION_POINTERS* pExp);

#endif
#endif
#endif


//////////
// DISK //
//////////

#define ADAT (FloppyDrive[DRIVE].adat)


//////////
// UNIX //
//////////

#ifdef SSE_UNIX

#if defined(SSE_DISK_SCP) && !defined(SSE_DISK_CAPS)
#define CAPSFDC_AI_AMDETENABLE BIT_0
#define CAPSFDC_AI_CRCENABLE   BIT_1
#define CAPSFDC_AI_CRCACTIVE   BIT_2
#define CAPSFDC_AI_AMACTIVE    BIT_3
#define CAPSFDC_AI_MA1ACTIVE   BIT_4
#define CAPSFDC_AI_AMFOUND     BIT_5
#define CAPSFDC_AI_MARKA1      BIT_6
#define CAPSFDC_AI_MARKC2      BIT_7
#define CAPSFDC_AI_DSRREADY    BIT_8
#define CAPSFDC_AI_DSRAM       BIT_9
#define CAPSFDC_AI_DSRMA1      BIT_10
#endif

#endif


/////////////
// VARIOUS //
/////////////

extern char ansi_string[MAX_PATH];
extern const char *stem_version_date_text;
#define TIMING_INFO FRAME,scan_y,(short)LINECYCLES


// was WIN_ONLY but is reentrant?
#ifdef WIN32
#if defined(SSE_EMU_THREAD)
#define AGENDA_CS(x) if(!OPTION_EMUTHREAD) x 
#else
#define AGENDA_CS(x) x
#endif
#endif

#ifdef UNIX
#define AGENDA_CS(x)
#endif

///////////
// VIDEO //
///////////

#define LINECYCLE0 TimeOfHSyncOff
#define LINECYCLES (ABSOLUTE_SYS_TIME-LINECYCLE0) 
#define FRAMECYCLES (ABSOLUTE_SYS_TIME-sys_time_of_last_vbl)

#define FRAME (Shifter.nVbl) 

#if defined(SSE_VID_SIZE4)
#define SCANLINES_OK (SSEOptions.Scanlines && WinSizeForRes[screen_res])
#else
#define SCANLINES_OK (SSEOptions.Scanlines && screen_res<HIRES \
  && WinSizeForRes[screen_res])
#endif

#if defined(SSE_VID_SCANLINES_INTERPOLATED)
#define SCANLINES_INTERPOLATED (SCANLINES_OK \
  && draw_win_mode[screen_res]==DWM_STRETCH && !FullScreen)
#else
#define SCANLINES_INTERPOLATED (FALSE)
#endif

#ifndef FRAME
#define FRAME (-1)
#endif


#endif//STEEMH_DECLA_H
