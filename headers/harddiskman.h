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

DOMAIN: Hard drive, GUI
FILE: harddiskman.h
DESCRIPTION: Declarations for Steem's Hard Disk Manager.
struct THardDiskInfo
class THardDiskManager, TAcsiHardDiskManager
---------------------------------------------------------------------------*/

#pragma once
#ifndef HARDDISKMAN_DECLA_H
#define HARDDISKMAN_DECLA_H

#include <stemdialogs.h>

#pragma pack(push, 8)

struct THardDiskInfo {
  THardDiskInfo() {
    Letter='\0';
  }
  EasyStr Path;
  char Letter;
};


class THardDiskManager : public TStemDialog {
private:
#ifdef WIN32
  static LRESULT CALLBACK WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
  void ManageWindowClasses(bool Unreg);
  HWND Focus;
#endif
#ifdef UNIX
	static int WinProc(THardDiskManager*,Window,XEvent*);
	static int button_notify_proc(hxc_button*,int,int*);
	void RemoveLine(int);
	hxc_dropdown drive_dd[MAX_GEMDOS_HARDDRIVES];
	hxc_edit drive_ed[MAX_GEMDOS_HARDDRIVES];
	hxc_button drive_browse_but[MAX_GEMDOS_HARDDRIVES],drive_open_but[MAX_GEMDOS_HARDDRIVES],drive_remove_but[MAX_GEMDOS_HARDDRIVES];
	hxc_button all_off_but,new_but,boot_label,ok_but,cancel_but;
	hxc_dropdown boot_dd;
#endif
public:
  THardDiskManager();
  ~THardDiskManager() { 
    Hide(); 
  }
  void Show(),Hide();
  void LoadData(bool FirstLoad,TConfigStoreFile *pCSF,bool *SecDisabled=NULL);
  void SaveData(bool FinalSave,TConfigStoreFile *pCSF);
  bool IsMountedDrive(char d);
  EasyStr GetMountedDrivePath(char d);
  void update_mount();
  bool NewDrive(char *Path);
  void CreateDriveControls(INT_PTR Idx);
  void SetWindowHeight();
  void GetDriveInfo();

  THardDiskInfo *OldDriveInfo;
  int nOldDrives;
  int nDrives;
  THardDiskInfo HDrive[MAX_GEMDOS_HARDDRIVES];
  bool ApplyChanges;
  bool OldDisableHardDrives;
  bool DisableHardDrives;
#if defined(SSE_ACSI)
  bool acsi; // for RTTI, booh!
#endif
};


#if defined(SSE_ACSI)
class TAcsiHardDiskManager : public THardDiskManager {
public:
  TAcsiHardDiskManager() {
    acsi=true;
  }
  void CheckTos() {}
  void LoadData(bool FirstLoad,TConfigStoreFile *pCSF,bool *SecDisabled);
  void SaveData(bool FinalSave,TConfigStoreFile *pCSF);
};
#endif

extern THardDiskManager HardDiskMan;
#if defined(SSE_ACSI)
extern TAcsiHardDiskManager AcsiHardDiskMan;
#endif

#pragma pack(pop)

#endif//#ifndef HARDDISKMAN_DECLA_H
