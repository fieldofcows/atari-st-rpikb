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
FILE: shortcutbox.h
DESCRIPTION: Declarations for Steem's flexible shortcuts system that
maps all sorts of user input to all sorts of emulator functions.
struct TShortcutInfo
class TShortcutBox
---------------------------------------------------------------------------*/
#pragma once
#ifndef SHORTCUTBOX_DECLA_H
#define SHORTCUTBOX_DECLA_H

#include "stemdialogs.h"
#ifdef WIN32
#include "scrollingcontrolswin.h"
#endif
#if defined(UNIX)
#include <x/x_controls.h>
#include <x/hxc_dir_lv.h>
#endif

#define CUT_PRESSKEY 0
#define CUT_PRESSCHAR 39
#define CUT_PLAYMACRO 44
#define CUT_TOGGLESTARTSTOP 3
#define CUT_TOGGLEFULLSCREEN 17
#define CUT_TAKESCREENSHOT 29


void ShortcutsCheck();
extern bool CutDisableKey[256];
extern int shortcut_vbl_count,cut_slow_motion_speed;
extern DWORD CutPauseUntilSysEx_Time;
extern int CutModDown;

UNIX_ONLY( extern "C" BYTE *Get_st_charset_bmp(); )

#pragma pack(push, 8)

struct TShortcutInfo {
  TShortcutInfo* DisableIfCutDownList[5];
  EasyStringList *pESL;
  DWORD PressChar;
  WORD Id[3],PressKey;
  int MacroFileIdx;
  BYTE OldDown,Down,Action;
};

#pragma pack(pop)

extern DynamicArray<TShortcutInfo> Cuts,CurrentCuts;
extern EasyStringList CutsStrings; //(eslNoSort);
extern EasyStringList CurrentCutsStrings; //(eslNoSort);

void ClearSHORTCUTINFO(TShortcutInfo *pSI);

extern EasyStringList CutFiles;

void DoShortcutDown(TShortcutInfo &),DoShortcutUp(TShortcutInfo &);

extern bool CutDisableAxis[MAX_PC_JOYS][20],CutDisablePOV[MAX_PC_JOYS][9];
extern DWORD CutButtonMask[MAX_PC_JOYS];
extern int MouseWheelMove;
extern bool CutButtonDown[2];

#define BPS_NOJOY 1
#define BPS_INSHORTCUT 2

#ifdef UNIX
typedef struct{
	hxc_button *p_sign[3],*p_del,*p_macro;
	hxc_buttonpicker *p_id[3],*p_stkey;
	hxc_dropdown *p_action,*p_stchar;
}PICKERLINE;
#endif


#pragma pack(push, 8)

class TShortcutBox : public TStemDialog {
public: //TODO
  void AddPickerLine(INT_PTR p);
  void UpdateAddButsPosition();
  void TranslateCutNames();
  void ChangeCutFile(Str NewSel,int Type,bool SaveOld);

  EasyStringList TranslatedCutNamesSL;
  Str CutDir,CurrentCutSel;

#ifdef WIN32
  static LRESULT CALLBACK WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
  static LRESULT CALLBACK ChooseMacroWndProc(HWND Win,UINT Mess,WPARAM wPar,
                                            LPARAM lPar);
  static int DTreeNotifyProc(DirectoryTree *pTree,void *t,int Mess,
                                  INT_PTR i1,INT_PTR i2);
  static int ChooseMacroTreeNotifyProc(DirectoryTree *pTree,void *t,
                                            int Mess,INT_PTR i1,INT_PTR);
  void ManageWindowClasses(bool Unreg);
  void RefreshCutFileView();

  Str ShowChooseMacroBox(Str CurrentMacro);
  void SetMacroFileButtonText(HWND But,int p);

  ScrollControlWin Scroller;
  HWND PopupFocus;
  Str ChooseMacroSel;
  int MenuItem;
  int CurrentCutSelType;
  int ScrollPos;
  static bool Picking;
  bool PopupOpen;
#endif
#ifdef UNIX
  static int WinProc(TShortcutBox*,Window,XEvent*);
	static int picker_notify_proc(hxc_buttonpicker*,int,int);
	static int button_notify_proc(hxc_button*,int,int*);
	static int dd_notify_proc(hxc_dropdown*,int,int);
	static int sa_notify_proc(hxc_scrollarea*,int,int);
	static int dir_lv_notify_proc(hxc_dir_lv*,int,int);
	void ShowHidePressSTKeyPicker(int);
	void LoadCutsAndCreateCutControls();
	void DeleteCut(int);
	PICKERLINE GetLine(int);
  void SetMacroFileButtonText(hxc_button*,int);

  Str ChooseMacro(Str);

  hxc_button sa_border,add_but[2],new_cut_but,change_fol_but;
  hxc_scrollarea sa;
  hxc_dir_lv dir_lv;

  IconGroup st_chars_ig;

	hxc_textdisplay help_td;
#endif
public:
  TShortcutBox();
  ~TShortcutBox() {
    Hide();
  }
  void Show(),Hide();
  void ToggleVisible() {
    IsVisible() ? Hide() : Show();
  }
  void UpdateDisableIfDownLists();
  void LoadAllCuts(bool LoadCurrent=true);
  int LoadShortcutInfo(DynamicArray<TShortcutInfo> &LoadCuts,
    EasyStringList &StringsESL,char *File,char *Sect="Shortcuts");
  void SaveShortcutInfo(DynamicArray<TShortcutInfo> &SaveCuts,char *File);
  void LoadData(bool FirstLoad,TConfigStoreFile *pCSF,bool *SecDisabled=NULL);
  void SaveData(bool FinalSave,TConfigStoreFile *pCSF);

#ifdef WIN32
  bool HasHandledMessage(MSG *mess);
  static HWND InfoWin;
  static DirectoryTree *pChooseMacroTree;
  static DirectoryTree DTree;
#endif
#ifdef UNIX
  int CurrentCutSelType;
  int ScrollPos;
  static bool Picking;
#endif
  bool Dirty;
};

extern TShortcutBox ShortcutBox;

#pragma pack(pop)


#endif//#ifndef SHORTCUTBOX_DECLA_H
