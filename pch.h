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

FILE: pch.h
MODULES: ALL
DESCRIPTION: System specific includes, declarations and macros. PCH stands
for pre-compiled headers (to reduce build time).
---------------------------------------------------------------------------*/
#pragma once
// pch.h must include SSE.h because WINVER and DirectX depend on some switches

#include <SSE.h>
#ifndef __PCH_H
#define __PCH_H







#if 0

#if defined(LINUX) || defined(CYGWIN) || defined(UNIX)

#undef WIN32
#ifndef UNIX
#define UNIX
#endif

#elif !defined(WIN32)

#ifndef UNIX // we insist but codelite still sees the #define :(
#define WIN32
#endif

#endif

#ifdef WIN32
/*  INFO
//
// _WIN32_WINNT version constants
//
#define _WIN32_WINNT_NT4                    0x0400 // Windows NT 4.0
#define _WIN32_WINNT_WIN2K                  0x0500 // Windows 2000
#define _WIN32_WINNT_WINXP                  0x0501 // Windows XP
#define _WIN32_WINNT_WS03                   0x0502 // Windows Server 2003
#define _WIN32_WINNT_WIN6                   0x0600 // Windows Vista
#define _WIN32_WINNT_VISTA                  0x0600 // Windows Vista
#define _WIN32_WINNT_WS08                   0x0600 // Windows Server 2008
#define _WIN32_WINNT_LONGHORN               0x0600 // Windows Vista
#define _WIN32_WINNT_WIN7                   0x0601 // Windows 7
#define _WIN32_WINNT_WIN8                   0x0602 // Windows 8
#define _WIN32_WINNT_WINBLUE                0x0603 // Windows 8.1
#define _WIN32_WINNT_WINTHRESHOLD           0x0A00 // Windows 10
#define _WIN32_WINNT_WIN10                  0x0A00 // Windows 10
*/
#if defined(SSE_WINDOWS_XP___) // for the moment the requirement is 2000
#define WINVER 0x0501          // should we do that?
#define _WIN32_WINNT 0x0501 
#elif defined(SSE_WINDOWS_2000)
#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#else
#define WINVER 0x0400
#define _WIN32_WINNT 0x0300
#endif
#if defined(SSE_VID_DD7)
#if defined(BCC_BUILD) || _MSC_VER == 1200 
#define DIRECT3D_VERSION 0x0900
#define DIRECTDRAW_VERSION 0x0700
#else
#define DIRECTDRAW_VERSION 0x0700
#endif
#else
#define DIRECTDRAW_VERSION 0x0200
#endif
#define DIRECTINPUT_VERSION 0x0500
#define DIRECTSOUND_VERSION 0x0200
#define OEMRESOURCE 1
#ifdef BCC_BUILD
#pragma anon_structs on
#endif
//---------------------------------------------------------------------------
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <io.h>
#include <math.h>
#include <mmsystem.h>
#include <commctrl.h>
#include <binary.h>
#include <time.h>
#include <setjmp.h>
#include <clarity.h>
#if defined(SSE_VID_D3D)
#if defined(BCC_BUILD)
#include "d3d/ddraw.h"
#include "d3d/dinput.h"
#include "d3d/dsound.h"
#elif _MSC_VER == 1200 && defined(SSE_VID_DD7) //VC6
#include "d3d/ddraw.h"
#include "d3d/dinput.h"
#include "d3d/dsound.h" 
#else//VS2008+VS2015
#include <d3d/ddraw.h>
#include <d3d/dsound.h>
#include <d3d/dinput.h>
#endif
#else//no d3d
#pragma pack(push, 8)
#include <d3d/ddraw.h>
#pragma pack(pop)
#include <d3d/dsound.h>
#include <d3d/dinput.h>
#endif

#if defined(BCB_BUILD) || defined(BCC_BUILD)

#ifdef __cplusplus
#include <except.h>
#endif
#include <dos.h>

#ifndef SPI_SETSCREENSAVERRUNNING
#define SPI_SETSCREENSAVERRUNNING 97
#endif

#elif defined(MINGW_BUILD)

#include <winnls.h>
#include <ctype.h>

#define _INC_TIME
#define _INC_COMMCTRL
#define _SHLOBJ_H_
#define EnumDateFormats EnumDateFormatsA
#define SUBLANG_SWEDISH 0x01    // Swedish
#define LVS_EX_CHECKBOXES 0x00000004
#define LVM_SETICONSPACING (LVM_FIRST + 53)
#define ListView_SetIconSpacing(hwndLV,cx,cy) (DWORD)SendMessage((hwndLV), LVM_SETICONSPACING, 0, MAKELONG(cx,cy))
#define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 54)   // optional wParam == mask
#define ListView_SetExtendedListViewStyle(hwndLV,dw) (DWORD)SendMessage((hwndLV), LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dw)
#define TTM_SETMAXTIPWIDTH (WM_USER + 24)
//#define SPI_SETSCREENSAVERRUNNING 97
#define LANG_BASQUE 0x2d
#define LANG_CATALAN 0x03

extern char *ultoa(unsigned long,char*,int);
extern char *strupr(char*);
extern char *strlwr(char*);
extern int random(int);

#elif defined(VC_BUILD)

extern int random(int);

#endif

#ifndef CSIDL_PROGRAM_FILES
#define CSIDL_PROGRAM_FILES                             0x0026
#endif
//---------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <binary.h>
#include <limits.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#define _INC_TIME
#include <dirent.h>
#include <setjmp.h>
#include <sys/resource.h>

#ifndef CYGWIN
// XImage shared memory extension
#include <sys/ipc.h>
#include <sys/shm.h>
//#ifdef __cplusplus
#include <X11/extensions/XShm.h>
#include <X11/extensions/xf86vmode.h>
//#endif
#else
#define NO_SHM
#endif

#include <sys/ioctl.h>
#include <fcntl.h>

#include <clarity.h>
#include <pthread.h>

#ifdef LINUX
#include <linux/joystick.h>
#include <sys/vfs.h>   // for statfs
#endif

#include <unistd.h>

#include <string.h>
#include <ctype.h>

extern char *ultoa(unsigned long,char*,int);
extern char *strupr(char*);
extern char *strlwr(char*);
//extern int random(int);
extern char *itoa(int,char*,int);

#endif

#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif

#endif//#ifndef __PCH_H


#endif