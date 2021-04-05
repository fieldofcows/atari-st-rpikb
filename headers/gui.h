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

DOMAIN: GUI
FILE: gui.h
DESCRIPTION: Declarations for Steem's Graphic User Interface.
struct TStatusInfo, TThreadFlag, [TToolbar], TSystemMetrics
Linux: TModifierState
---------------------------------------------------------------------------*/

#pragma once
#ifndef GUI_DECLA_H
#define GUI_DECLA_H

#include <easystr.h>
#include <configstorefile.h>
#include "conditions.h"
#include "resnum.h"
#include "stemdialogs.h"

#if !defined(ONEGAME)
#ifdef WIN32
#if defined(SSE_GUI_TOOLBAR)
#define MENUHEIGHT GuiSM.m_toolbar_height
#else
#define MENUHEIGHT (BIG_ICONS?(36+1)/*40*/:(20+1))
#endif
#else
#define MENUHEIGHT 21 // TODO
#endif
#else
#define MENUHEIGHT 0
#endif

#define PEEKED_MESSAGE 0
#define PEEKED_QUIT 1
#define PEEKED_NOTHING 2
#define IGNORE_EXTEND 2
#define NO_SHIFT_SWITCH 8
#define STEM_MOUSEMODE_DISABLED 0
#define STEM_MOUSEMODE_WINDOW 1
#define STEM_MOUSEMODE_BREAKPOINT 3141
#define BOOT_MODE_DEFAULT 0
#define BOOT_MODE_FULLSCREEN 1
#define BOOT_MODE_WINDOW 2
#define BOOT_MODE_FLAGS_MASK 0xff
#define BOOT_MODE_RUN 0x100
#define STPASTE_TOGGLE 0
#define STPASTE_START 1
#define STPASTE_STOP 2
#define LVI_SI_CHECKED (1 << 13)
#define LVI_SI_UNCHECKED (1 << 12)
#define PROFILESECT_ON LVI_SI_CHECKED
#define PROFILESECT_OFF LVI_SI_UNCHECKED
#define RC_FLAG_WIDTH 16
#define RC_FLAG_HEIGHT 12
#define SHORTCUTS_TIMER_ID 2000
#define DISPLAYCHANGE_TIMER_ID 2100
#define MIDISYSEX_TIMER_ID 2200
#if defined(SSE_DISK_SWAPPER)
#define STATUSBAR_TIMER_ID 2300
#endif
#define HL_STATIC 1
#define HL_UNDERLINE 2
#define HL_WINDOWBK 4
#define ARG_UNKNOWN 0
#define ARG_GDI 1
#define ARG_NODS 2
#define ARG_WINDOW 3
#define ARG_NOLPT 4
#define ARG_NOCOM 5
#define ARG_NOSHM 6
#define ARG_QUITQUICKLY 7
#define ARG_SOUNDCLICK 8
#define ARG_HELP 9
#define ARG_FULLSCREEN 10
#define ARG_DOUBLECHECKSHORTCUTS 11
#define ARG_DONTLIMITSPEED 12
#define ARG_EXACTSPEEDLIMITNONE 13
#define ARG_EXACTSPEEDLIMITTICK 14
#define ARG_EXACTSPEEDLIMITHP 15
#define ARG_EXACTSPEEDLIMITSLEEPTIME 16
#define ARG_EXACTSPEEDLIMITSLEEPHP 17
#define ARG_ACCURATEFDC 18
#define ARG_NOPCJOYSTICKS 19
#define ARG_OLDPORTIO 20
#define ARG_ALLOWREADOPEN 21
#define ARG_NOINTS 22
#define ARG_STFMBORDER 23
#define ARG_SCREENSHOTUSEFULLNAME 24
#define ARG_ALLOWLPTINPUT 25
#define ARG_NONOTIFYINIT 26
#define ARG_SCREENSHOTALWAYSADDNUM 27
#define ARG_PSGCAPTURE 28
#define ARG_CROSSMOUSE 29
#define ARG_RUN 30
#define ARG_GDIFSBORDER 31
#define ARG_PASTI 32
#define ARG_NOAUTOSNAPSHOT 33
#define ARG_NOPASTI 34
#define ARG_SETSOF 100
#define ARG_SETINIFILE 101
#define ARG_SETTRANSFILE 102
#define ARG_SETFONT 103
#define ARG_SETCUTSFILE 104
#define ARG_TAKESHOT 107
#define ARG_SETPABUFSIZE 108
#define ARG_RTBUFSIZE 109
#define ARG_RTBUFNUM 110
#define ARG_DISKIMAGEFILE 201
#define ARG_SNAPSHOTFILE 202
#define ARG_CARTFILE 203
#define ARG_STPROGRAMFILE 204
#define ARG_STPROGRAMTPFILE 205
#define ARG_LINKFILE 206
#define ARG_TOSIMAGEFILE 207
#define ARG_PASTIDISKIMAGEFILE 208
#define ARG_NONEWINSTANCE 250
#define ARG_ALWAYSNEWINSTANCE 251
#if defined(SSE_UNIX_TRACE)
#define ARG_LOGSECTION 252
#define ARG_TRACEFILE 253
#endif
#define ARG_NOTRACE 254
#define MSW_NOCHANGE int(0x7fff)

extern char stem_version_text[SSE_VERSION_TXT_LEN];
extern char stem_window_title[WINDOW_TITLE_MAX_CHARS+1];
extern char gAppName[];


#pragma pack(push, 8)

struct TStatusInfo {
  enum EStatusInfo {MESSAGE_NONE,MOTOROLA_CRASH,INTEL_CRASH,BLIT_ERROR,
    BOILER_MESSAGE,HD6301_CRASH,MESSAGE_DISK};
  char text[256];
  int FlagX; // to record width of text before TOS flag
  BYTE MessageIndex;
};

#pragma pack(pop)

extern "C" int PeekEvent();
void InitTranslations();
bool CleanupTempFiles();
void CleanupGUI();
void GUIRunStart(),GUIRunEnd();
int Alert(char *Mess,char *Title,UINT Flags);
void QuitSteem();
void fast_forward_change(bool Down,bool Searchlight);
BOOL GUIPauseWhenInactive();
void PasteVBL();
void StemWinResize(int xo=0,int yo=0);
void GUIColdResetChangeSettings();
void GUISaveResetBackup();
#if defined(SSE_VID_2SCREENS)
void CentreWindow(HWND Win,bool Redraw);
#endif
void CheckResetIcon();
void CheckResetDisplay(bool AlwaysHide=false);
void ParseCommandLine(int NumArgs,char *Arg[],int Level=0);
BOOL MakeGUI();
void SetStemWinSize(int w,int h,int xo=0,int yo=0);
void SetStemMouseMode(int NewMM,BOOL SuppressMessage=FALSE);
void flashlight(bool on);
void PasteIntoSTAction(int Action);
void HandleKeyPress(UINT VKCode,DWORD Up,int Extended=IGNORE_EXTEND);
void InitRealVKCodeForKeypad();
char *FSTypes(int Type,...);
void LoadAllIcons(TConfigStoreFile *pCSF,bool FirstCall=0);
int GetComLineArgType(char *,EasyStr &);
void MoveStemWin(int x,int y,int w,int h);
int GetScreenWidth(),GetScreenHeight();
void ShowAllDialogs(bool Show);
void slow_motion_change(bool Down);
void ShiftSwitchChangeModifiers(bool ShiftShouldBePressed,bool 
                                AltShouldBePressed,int ModifierRestoreArray[3]);
void ShiftSwitchRestoreModifiers(int ModifierRestoreArray[3]);
void HandleShiftSwitching(UINT VKCode,DWORD Up,BYTE &STCode,
                          int ModifierRestoreArray[3]);
Str SnapShotGetLastBackupPath();
void SnapShotGetOptions(EasyStringList* p_sl);

enum {SB_PART_FREQ,SB_PART_MAIN,SB_PART_ICONS,SB_PART_CAPS};
#if defined(SSE_GUI_STATUS_BAR)
extern HWND hStatusBar;
extern char status_bar_text[4][64];
extern RECT status_bar_rc[4];
extern BYTE status_bar_mask; // Invalidated
HWND DoCreateStatusBar(HWND hwndParent, HMENU idStatus, HINSTANCE
                   hinst, int cParts);
void GUIRefreshStatusBar(BYTE part=0xFF);
#if defined(SSE_EMU_THREAD)
void GUIRefreshStatusBar2(BYTE part=0xFF);
#endif
#define UPDATE_STATUS_BAR_PART(n) GUIRefreshStatusBar(1<<n)
#define REFRESH_STATUS_BAR        GUIRefreshStatusBar()
#define REFRESH_STATUS_BAR_GX     InvalidateRect(hStatusBar,NULL,FALSE)
#else
#define REFRESH_STATUS_BAR {}
#define REFRESH_STATUS_BAR_GX {}
#define UPDATE_STATUS_BAR_PART(n) {}
#endif

#if defined(SSE_GUI_TOOLBAR)
void GuiShowToolbar(BOOL show);
#endif

extern TStatusInfo StatusInfo;
extern int DoSaveScreenShot;
extern bool ResChangeResize,CanUse_400;
extern bool bAppActive,bAppMinimized;
extern DWORD DisableDiskLightAfter;
extern bool StepByStepInit;
extern EasyStr RunDir,WriteDir,globalINIFile,ScreenShotFol,DocDir;
extern EasyStr LastSnapShot,BootStateFile,StateHist[10],AutoSnapShotName;
extern EasyStr DefaultSnapshotFile;
extern Str BootDisk[2];
extern bool PauseWhenInactive,BootTOSImage;
extern BYTE MuteWhenInactive;
extern bool bAOT,bAppMaximized;
#ifndef ONEGAME
extern bool AutoLoadSnapShot,ShowTips;
#else
extern bool AutoLoadSnapShot,ShowTips;
#endif
extern bool AllowLPT,AllowCOM;
extern bool HighPriority;
extern int BootInMode;
extern POINT WinSize[4][5];
extern int WinSizeForRes[4];
extern RECT rcPreFS;
extern BYTE PCCharToSTChar[128];
extern BYTE STCharToPCChar[128];
extern BYTE KeyDownModifierState[256];
extern int PasteVBLCount,PasteSpeed;
extern Str PasteText;
extern bool StartEmuOnClick;
extern int stem_mousemode;
extern int window_mouse_centre_x,window_mouse_centre_y;
extern bool TaskSwitchDisabled;
extern Str ROMFile,CartFile;
extern bool FSQuitAskFirst,Quitting;
extern bool FSDoVsync;
extern int ExternalModDown;
extern bool comline_allow_LPT_input;
extern bool AllowTaskSwitch;

#if defined(SSE_GUI_ALT_MENU)
extern HMENU StemWin_AltMenu,StemWin_AltMenuFile,StemWin_AltMenuEmu,
  StemWin_AltMenuTools;
extern bool AltMenuOn;
BOOL SteemSetMenu(bool on);
#endif

#if defined(SSE_EMU_THREAD)

#pragma pack(push, 8)

struct TThreadFlag {
  volatile bool blocked;
  volatile bool acknowledged;
  volatile BYTE disabled;
  TThreadFlag() {
    blocked=acknowledged=disabled=false;
  }
  BOOL Lock(); // TRUE: ok; FALSE: was already locked or is disabled
  BOOL Unlock(); // TRUE: ok ok FALSE: wasn't locked or is disabled
  BOOL Acknowledge(); // TRUE: ok; FALSE: wasn't locked or is disabled
};

#pragma pack(pop)

extern TThreadFlag SoundLock,VideoLock,DiskLock;
extern HANDLE hEmuThread;
extern DWORD EmuThreadId;
extern bool SuspendRendering;

#endif


#ifdef WIN32

#define PBS_RIGHTCLICK 1
#define PBS_DBLCLK 2
#define BM_GETCLICKBUTTON 0x00F8
#define BM_SETCLICKBUTTON 0x00FA
#define BM_RELOADICON 0x00F9
#define PDS_VCENTRESTATIC 1

#define CLICK_PLAY_BUTTON() \
  if (!RunMessagePosted){ \
    SendDlgItemMessage(StemWin,IDC_PLAY,BM_SETCLICKBUTTON,1,0); \
    PostMessage(StemWin,WM_COMMAND,MAKEWPARAM(IDC_PLAY,BN_CLICKED),\
      (LPARAM)GetDlgItem(StemWin,IDC_PLAY)); \
    RunMessagePosted=true; \
  }

#pragma pack(push, 8)

struct TSystemMetrics {
  int m_cx_screen,m_cy_screen,m_cx_frame,m_cy_frame,m_cy_caption,m_cx_vscroll;
  short mCharHeight,mHorizontalSeparation,mCbUnits;
  short m_menubar_height,m_toolbar_height,m_statusbar_height;
  // function and test because we use that in constructors, quick fix TODO
  TSystemMetrics() {
    ZeroMemory(this,sizeof(TSystemMetrics));
  }
  int cx_screen() {
    if(!m_cx_screen)
      Update();
    return m_cx_screen;
  }
  int cy_screen() {
    if(!m_cy_screen)
      Update();
    return m_cy_screen;
  }
  int cx_frame() {
    if(!m_cx_frame)
      Update();
    return m_cx_frame;
  }
  int cy_frame() {
    if(!m_cy_frame)
      Update();
    return m_cy_frame;
  }
  int cy_caption() {
    if(!m_cy_caption)
      Update();
    return m_cy_caption;
  }
  int cx_vscroll() {
    if(!m_cx_vscroll)
      Update();
    return m_cx_vscroll;
  }
#if defined(SSE_VID_2SCREENS)
  void Update(BOOL NoMonitorCheck=FALSE);
#else
  void Update();
#endif
};

#pragma pack(pop)


bool HandleMessage(MSG *mess);
void MakeEditNoCaret(HWND Edit);
HWND CreateTextDisplay(HWND daddy,int x,int y,int w,int h,INT_PTR id);
LRESULT CALLBACK WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
#ifdef DEADC0DE
LRESULT CALLBACK FSClipWndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
#endif
LRESULT CALLBACK FSQuitWndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
void EnableAllWindows(bool Enable,HWND NoDisable);
void DisableTaskSwitch();
void EnableTaskSwitch();
bool IsSteemAssociated(EasyStr Exts);
void AssociateSteem(EasyStr Exts,EasyStr FileClass);
void UpdatePasteButton();

extern TSystemMetrics GuiSM;
extern bool RunMessagePosted;
extern HWND StemWin,ParentWin,ToolTip,DisableFocusWin,UpdateWin;
extern HMENU StemWin_SysMenu;
extern HFONT fnt,hSteemGuiFont;
extern HCURSOR PCArrow;
extern COLORREF MidGUIRGB,DkMidGUIRGB;
extern bool WinNT;
extern HWND NextClipboardViewerWin;
extern HICON hGUIIcon[RC_NUM_ICONS],hGUIIconSmall[RC_NUM_ICONS];
void GUIToolbarArrangeIcons(int cw);

#endif//WIN32


#ifdef UNIX

#define PEEKED_RUN 3
extern void PostRunMessage();
#define CLICK_PLAY_BUTTON() PostRunMessage()


extern void XGUIUpdatePortDisplay();
extern short GetKeyState(int);
#define GetAsyncKeyState GetKeyState
extern void GUIUpdateInternalSpeakerBut();
extern void GetCursorPos(POINT *);
extern void SetCursorPos(int,int);
extern TModifierState GetLRModifierStates();

#define MB_OK 0x00000000L
#define MB_OKCANCEL 0x00000001L
#define MB_ABORTRETRYIGNORE 0x00000002L
#define MB_YESNOCANCEL 0x00000003L
#define MB_YESNO 0x00000004L
#define MB_RETRYCANCEL 0x00000005L

#define MB_ICONHAND 0x00000010L
#define MB_ICONQUESTION 0x00000020L
#define MB_ICONEXCLAMATION 0x00000030L
#define MB_ICONASTERISK 0x00000040L
#define MB_USERICON 0x00000080L
#define MB_ICONWARNING MB_ICONEXCLAMATION
#define MB_ICONERROR MB_ICONHAND
#define MB_ICONINFORMATION MB_ICONASTERISK
#define MB_ICONSTOP MB_ICONHAND

#define MB_DEFBUTTON1 0x00000000L
#define MB_DEFBUTTON2 0x00000100L
#define MB_DEFBUTTON3 0x00000200L
#define MB_DEFBUTTON4 0x00000300L

#define MB_APPLMODAL 0x00000000L
#define MB_SYSTEMMODAL 0x00001000L
#define MB_TASKMODAL 0x00002000L

#define MB_HELP 0x00004000L
#define MB_NOFOCUS 0x00008000L
#define MB_SETFOREGROUND 0x00010000L
#define MB_DEFAULT_DESKTOP_ONLY 0x00020000L
#define MB_TOPMOST 0x00040000L
#define MB_RIGHT 0x00080000L

#define MB_TYPEMASK 0x0000000FL
#define MB_ICONMASK 0x000000F0L
#define MB_DEFMASK 0x00000F00L
#define MB_MODEMASK 0x00003000L
#define MB_MISCMASK 0xFFFFC000L

#define IDOK                1
#define IDCANCEL            2
#define IDABORT             3
#define IDRETRY             4
#define IDIGNORE            5
#define IDYES               6
#define IDNO                7

extern XErrorEvent XError;
int HandleXError(Display*,XErrorEvent*);

int StemWinProc(void*,Window,XEvent*);
int timerproc(void*,Window,int);

#include "../../include/x/hxc.h"
#include "../../include/x/hxc_popup.h"
#include "../../include/x/hxc_popuphints.h"
#include "../../include/x/hxc_alert.h"
#include "../../include/x/hxc_fileselect.h"


int hyperlink_np(hxc_button*,int,int*);

void steem_hxc_modal_notify(bool);

extern hxc_popup pop;
extern hxc_popuphints hints;

int ProcessEvent(XEvent *);
void InitColoursAndIcons();

void steem_hxc_alloc_colours(Display*);
void steem_hxc_free_colours(Display*);

extern Window StemWin;
extern GC DispGC;
extern Cursor EmptyCursor;
extern Atom RunSteemAtom,LoadSnapShotAtom;
extern XID SteemWindowGroup;
extern DWORD BlackCol,WhiteCol,BkCol,BorderLightCol,BorderDarkCol;
extern hxc_alert alert;
//XFontStruct *GUIFont=NULL,*SmallFont=NULL;

void PrintHelpToStdout();
#define GetLongPathName(from,to,len) if (to!=from){strncpy(to,from,len);to[len-1]=0;}
bool SetForegroundWindow(Window,Time=CurrentTime);
Window GetForegroundWindow();
void CentreWindow(Window,bool);
bool GetWindowPositionData(Window,TWinPositionData *);

extern short KeyState[256];
void SetKeyState(int,bool,bool=false);
short GetKeyStateSym(KeySym);

int MessageBox(WINDOWTYPE,char *,char *,UINT);

typedef int (BUTNOTIFYPROC)(Window,int,int,int*);
typedef BUTNOTIFYPROC* LPBUTNOTIFYPROC;
int StemWinButtonNotifyProc(Window,int,int,int *);

void SnapShotProcess(int);
//SS check icon16.bmp, there they are!
#define ICO16_FOLDER 0
#define ICO16_PARENTDIRECTORY 1
#define ICO16_EXCLAMATION 2
#define ICO16_QUESTION 3
#define ICO16_STOP 4
#define ICO16_INFO 5
#define ICO16_FF 6
#define ICO16_FORWARD 7
#define ICO16_HARDDRIVE 8
#define ICO16_HOMEFOLDER 9
#define ICO16_GENERALINFO 10
#define ICO16_JOY 11
#define ICO16_PORTS 12
#define ICO16_OSD 13
#define ICO16_STEEM 14
#define ICO16_INSERTDISK 15
#define ICO16_RESET 16
#define ICO16_RUN 17
#define ICO16_SETHOMEFOLDER 18
#define ICO16_CUT 19
#define ICO16_SNAPSHOTS 20
#define ICO16_SNAPSHOT 20
#define ICO16_EJECTDISK 21
#define ICO16_OPTIONS 22
#define ICO16_BACK 23
#define ICO16_STCONFIG 24
#define ICO16_CHIP 24
#define ICO16_CART 24
#define ICO16_SOUND 25
#define ICO16_DISKMAN 26
#define ICO16_DISK 26
#define ICO16_JOYDIR 27
#define ICO16_ZIP_RO 28
#define ICO16_PATCHES 29
#define ICO16_TOOLS ICO16_OPTIONS
#define ICO16_DISPLAY 32
#define ICO16_BRIGHTCON 31
#define ICO16_LINKS 33
#define ICO16_README 34
#define ICO16_FAQ 35
#define ICO16_DRAWSPEED 36
#define ICO16_PASTE 37
#define ICO16_FUJI16 38
#define ICO16_ACCURATEFDC 39
#define ICO16_MIDI 40
#define ICO16_CUTON 41
#define ICO16_CUTOFF 42
#define ICO16_PATCHESNEW 43
#define ICO16_RESETGLOW 44
#define ICO16_PROFILE 45
#define ICO16_ST 46
#define ICO16_MACROS 47
#define ICO16_TICKED 48
#define ICO16_RADIOMARK 49
#define ICO16_DISKMANMENU 50
#define ICO16_FOLDERLINK 51
#define ICO16_FOLDERLINKBROKEN 52
#define ICO16_DISKLINK 53
#define ICO16_DISKLINKBROKEN 54
#define ICO16_CUTONLINK 55
#define ICO16_CUTOFFLINK 56
#define ICO16_ZIP_RW 57
#define ICO16_DISK_RO 58
#define ICO16_PROFILELINK 59
#define ICO16_MACROLINK 60
#define ICO16_UNTICKED 61
#define ICO16_FULLSCREEN 62
#define ICO16_TAKESCREENSHOTBUT 63
#define ICO16_UNRADIOMARKED 64

#if defined(SSE_UNIX_OPTIONS_SSE_ICON)
#define ICO16_SSE_OPTION 65 // must be in icon16.bmp
#endif

#define ICO32_JOYDIR 0
#define ICO32_RECORD 1
#define ICO32_DRIVE_A 2
#define ICO32_DRIVE_B 3
#define ICO32_LINKCUR 4
#define ICO32_PLAY 5
#define ICO32_DRIVE_B_OFF 6
#define ICO32_EXCLAM 7
#define ICO32_QUESTION 8
#define ICO32_STOP 9
#define ICO32_INFO 10

#define ICO64_STEEM 0
#define ICO64_HARDDRIVES 1
#define ICO64_HARDDRIVES_FR 2

extern "C" LPBYTE Get_icon16_bmp(),Get_icon32_bmp(),Get_icon64_bmp(),Get_tos_flags_bmp();
extern IconGroup Ico16,Ico32,Ico64,IcoTOSFlags;
extern Pixmap StemWinIconPixmap,StemWinIconMaskPixmap;

extern hxc_button RunBut,FastBut,ResetBut,SnapShotBut,ScreenShotBut,PasteBut,FullScreenBut;
extern hxc_button InfBut,PatBut,CutBut,OptBut,JoyBut,DiskBut;

#define FF_DOUBLECLICK_MS 200

extern DWORD ff_doubleclick_time;

int StemWinButtonNotifyProc(hxc_button*,int,int*);


inline bool SetProp(Window Win,XContext Prop,DWORD Val)
{
  return SetProp(XD,Win,Prop,Val);
}


inline DWORD GetProp(Window Win,XContext Prop)
{
  return GetProp(XD,Win,Prop);
}


inline DWORD RemoveProp(Window Win,XContext Prop)
{
  return RemoveProp(XD,Win,Prop);
}

int romfile_parse_routine(char*fn,struct stat*s);

int diskfile_parse_routine(char *fn,struct stat *s);

int wavfile_parse_routine(char *fn,struct stat *s);

int folder_parse_routine(char *fn,struct stat *s);

int cartfile_parse_routine(char *fn,struct stat*s);

#if defined(SSE_IKBD_MAPPINGFILE)
int inifile_parse_routine(char *fn,struct stat*s);
#endif

extern hxc_fileselect fileselect;

#define COMLINE_HTTP 0
#define COMLINE_FTP 1
#define COMLINE_MAILTO 2
#define COMLINE_FM 3
#define COMLINE_FIND 4

#define NUM_COMLINES 5

extern char* Comlines_Default[NUM_COMLINES][8];

extern Str Comlines[NUM_COMLINES];

#endif //UNIX


#endif//GUI_DECLA_H
