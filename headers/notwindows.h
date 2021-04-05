#pragma once
#ifndef NOTWINDOWS_DECLA_H
#define NOTWINDOWS_DECLA_H

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned int UINT;
typedef char* LPSTR;
typedef const char* LPCSTR;

typedef long long LONGLONG;
typedef unsigned long long DWORDLONG;

typedef unsigned char* LPBYTE;
typedef unsigned short* LPWORD;
typedef unsigned long* LPDWORD;
typedef long* LPLONG;
typedef void* LPVOID;

typedef unsigned long HRESULT;
#ifdef SSE_X64
#else
typedef long LONG_PTR;
typedef int INT_PTR;
typedef UINT UINT_PTR;
typedef unsigned long DWORD_PTR;
#endif

//#define MAX_PATH          260

#define DD_OK 0
#define DDERR_GENERIC 1
#define DDERR_SURFACELOST 2

#define DS_OK 0
#define DSERR_GENERIC 1
#define DSERR_BUFFERLOST 2

typedef struct{
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
}RECT;

typedef struct{
  LONG x;
  LONG y;
}POINT;


typedef struct{
  DWORD dwSize; //Not read
  DWORD dwFlags; //Not read
  DWORD dwXpos;
  DWORD dwYpos;
  DWORD dwZpos;
  DWORD dwRpos;
  DWORD dwUpos;
  DWORD dwVpos;
  DWORD dwButtons;
  DWORD dwButtonNumber; //Not read
  DWORD dwPOV;
}JOYINFOEX;


#define LOBYTE(a) BYTE(a)
#define HIBYTE(a) BYTE(((WORD)(a)) >> 8)

#define LOWORD(a) WORD(a)
#define HIWORD(a) WORD(((DWORD)(a)) >> 16)

typedef unsigned long LANGID;
#define LANG_ENGLISH 0
#define SUBLANG_ENGLISH_UK 0
#define SUBLANG_ENGLISH_US 1
#define SUBLANG_ENGLISH_AUS 2

#define LANG_FRENCH 1
#define SUBLANG_FRENCH 1
#define SUBLANG_FRENCH_BELGIAN 2

#define LANG_GERMAN 2
#define SUBLANG_GERMAN 2

#define LANG_SWEDISH 3
#define SUBLANG_SWEDISH 3

#define LANG_SPANISH 4
#define SUBLANG_SPANISH 5

#define LANG_CATALAN 5
#define SUBLANG_CATALAN 5

#define LANG_BASQUE 6
#define SUBLANG_BASQUE 6

#define LANG_ITALIAN 7
#define SUBLANG_ITALIAN 7

#define LANG_NORWEGIAN 8
#define SUBLANG_NEUTRAL 0 //2 //SSE

#define LANG_DANISH 9
#define SUBLANG_DANISH 9

#define MAKELANGID MAKELONG

#ifndef RPI
#define MAX_PATH ((PATH_MAX>2000) ? 2000:PATH_MAX)
#else
#define MAX_PATH 2000
#endif

#define NO_DEBUG_BUILD

/*
#define FA_NORMAL   0x00
#define FA_RDONLY   0x01
#define FA_HIDDEN   0x02
#define FA_SYSTEM   0x04
#define FA_LABEL    0x08
#define FA_DIREC    0x10
#define FA_ARCH     0x20
*/

#define kbhit() (feof(stdin)==0)

#if !defined(UNIX) && defined(__cplusplus) //TODO
typedef unsigned int KeyCode;
#endif
#if defined(RPI)
typedef unsigned int KeyCode;
#endif
#if defined(UNIX)
extern KeyCode VK_LBUTTON,VK_RBUTTON,VK_MBUTTON;
extern KeyCode VK_F1,VK_F11,VK_F12,VK_END;
extern KeyCode VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN,VK_TAB;
extern KeyCode VK_SHIFT,VK_LSHIFT,VK_RSHIFT;
extern KeyCode VK_MENU,VK_LMENU,VK_RMENU;
extern KeyCode VK_CONTROL,VK_LCONTROL,VK_RCONTROL;
extern KeyCode VK_NUMLOCK,VK_SCROLL;
#endif
#include <sys/timeb.h>


#endif//#ifndef NOTWINDOWS_DECLA_H

