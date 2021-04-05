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
FILE: infobox.cpp
DESCRIPTION: Declarations for Steem's general information dialog.
class TGeneralInfo
---------------------------------------------------------------------------*/

#pragma once
#ifndef INFOBOX_H
#define INFOBOX_H

#include <scrollingcontrolswin.h>

#include "stemdialogs.h"


enum EInfobox {
  INFOPAGE_ABOUT,
  INFOPAGE_DRAWSPEED,
  INFOPAGE_LINKS,
  INFOPAGE_README,
  INFOPAGE_UNIXREADME,
  INFOPAGE_HOWTO_DISK,
  INFOPAGE_HOWTO_CART,
  INFOPAGE_FAQ,
  INFOPAGE_FAQ_SSE,
  INFOPAGE_HINTS,
  INFOPAGE_README_SSE,
  INFOPAGE_LICENCE,
  INFOPAGE_TRACE,
  INFOPAGE_BUGS,
  INFOPAGE_STATS, // run.cpp
  NUM_INFOPAGE,
  ID_INFOBOXTEXT=500,
  ID_INFOBOXRICHTEXT=501,
  IDC_INFOBOXTEXTCONTROL=505
};


#pragma pack(push, 8)

class TGeneralInfo : public TStemDialog {
public: //TODO
#if !defined(SSE_GUI_RICHEDIT2)
  void GetHyperlinkLists(EasyStringList &,EasyStringList &);
#endif
#ifdef WIN32
  static WNDPROC OldEditWndProc;
  HBRUSH BkBrush;
  HIMAGELIST il;
#if !defined(SSE_GUI_RICHEDIT)
  HFONT hFontCourier;
#endif
  EasyStr SearchText;
  int page_l,page_w,page_h;
  int MaxLinkID;
  int Page;
  ScrollControlWin Scroller;
  static LRESULT CALLBACK WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
  void DestroyCurrentPage();
  void ManageWindowClasses(bool Unreg);
#endif
#ifdef UNIX
  int page_l,page_w;
  int Page;
  static int WinProc(TGeneralInfo*,Window,XEvent*);
  static int button_notifyproc(hxc_button*,int,int*);
  static int listview_notify_proc(hxc_listview*,int,int);
  static int edit_notify_proc(hxc_edit*,int,int);
	void ShowTheReadme(char*,bool=false);
	hxc_button gb,thanks_label;
	hxc_textdisplay about,thanks;
	hxc_textdisplay readme;
	hxc_button steem_link,email_link;
	hxc_scrollarea sa;
	hxc_listview page_lv;
  int last_find_idx;
#endif
public:
  TGeneralInfo();
  ~TGeneralInfo() { 
    Hide();
    WIN_ONLY( DeleteObject(BkBrush); ) 
  }
  void Show(),Hide();
  void ToggleVisible() {
    IsVisible() ? Hide() : Show();
  }
  void LoadData(bool FirstLoad,TConfigStoreFile *pCSF,bool*);
  void SaveData(bool FinalSave,TConfigStoreFile *pCSF);
  void CreatePage(int pg);
  void CreateSpeedPage(),CreateAboutPage(),CreateLinksPage();
  void CreateReadmePage(int pg);
  void UpdatePositions();
#ifdef WIN32
  INT_PTR DrawColumn(int x,int y,INT_PTR id,char *t1,...);
  EasyStr dp4_disp(int val);
  void LoadIcons();
  bool HasHandledMessage(MSG *mess);
#endif
};

extern TGeneralInfo InfoBox;



#pragma pack(pop)


#endif//#ifndef INFOBOX_H
