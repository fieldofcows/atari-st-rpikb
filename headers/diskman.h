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

DOMAIN: Disk image, GUI
FILE: diskman.h
DESCRIPTION: Declarations for Steem's Disk Manager.
struct TDiskManager, TDiskManFileInfo
---------------------------------------------------------------------------*/

#pragma once
#ifndef DISKMAN_DECLA_H
#define DISKMAN_DECLA_H

#include <stemdialogs.h>
#include <floppy_disk.h>
#ifdef SSE_UNIX
#include <x/hxc_dir_lv.h>
#endif


#define FileIsDisk(s) ExtensionIsDisk(strrchr(s,'.'))

int ExtensionIsDisk(char *TestedExt);
bool ExtensionIsPastiDisk(char *Ext);

enum EDiskmqn {
  IDC_DISKPATH=97,
  IDC_DISKVIEW=102,
};

#pragma pack(push, 8)

struct TDiskManFileInfo {
  EasyStr Name,Path,LinkPath;
#ifdef WIN32
  FILETIME Date;
#endif
  int Image;
  bool UpFolder,Folder,ReadOnly,BrokenLink,Zip;

// Could implement the next few, in a million years!
//
//  EasyStr IconPath;int IconIdx;
//  EasyStr Description
};

#define DISKVIEWSCROLL_TIMER_ID 1
#define MSACONV_TIMER_ID 2

struct TDiskManager : public TStemDialog {
  // FUNCTION
  void PerformInsertAction(int Action,EasyStr Name,EasyStr Path,
                           EasyStr DiskInZip);
  void ExtractArchiveToSTHardDrive(Str Path);
  static void GCGetCRC(char *Path,DWORD *lpCRC,int nCRCs);
#ifdef DEAD_C0DE
  static BYTE* GCConvertToST(char *Path,int Num,int *pLen);
#endif
  void GetContentsSL(Str Path);
  bool GetContentsCheckExist();
  //Str GetContentsGetAppendName(Str TOSECName);
#ifdef WIN32
  static LRESULT CALLBACK WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar);
  static LRESULT CALLBACK Drive_Icon_WndProc(HWND Win,UINT Mess,WPARAM wPar,
                                             LPARAM lPar);
  static LRESULT CALLBACK DiskView_WndProc(HWND Win,UINT Mess,WPARAM wPar,
                                           LPARAM lPar);
  static LRESULT CALLBACK DriveView_WndProc(HWND Win,UINT Mess,WPARAM wPar,
                                            LPARAM lPar);
  static LRESULT CALLBACK Dialog_WndProc(HWND Win,UINT Mess,WPARAM wPar,
                                         LPARAM lPar);
  static int CALLBACK CompareFunc(LPARAM lPar1,LPARAM lPar2,LPARAM);
  void BeginDrag(int Item,HWND From);
  void MoveDrag();
  void EndDrag(int x,int y,bool RightDrag);
  bool DoCreateMultiLinks();
  void AddFoldersToMenu(HMENU Pop,int StartID,EasyStr NoAddFol,bool Setting);
  bool MoveOrCopyFile(bool Moving,char *From,char *To,char *DiskPath,
                      bool SameFol);
  void PropShowFileInfo(int i);
  void AddFileOrFolderContextMenu(HMENU Pop,TDiskManFileInfo *Inf);
  void UpdateBPBFiles(Str CurDisk,Str NewDisk,bool Moving);
  void ManageWindowClasses(bool Unreg);
  Str GetMSAConverterPath();
  void GoToDisk(Str Path,bool bRefresh,bool bFocusView=true);
  void AdaptBackground();
#ifndef SSE_NO_WINSTON_IMPORT
  HRESULT CreateLinkCheckForOverwrite(char *,char *,IShellLink *,IPersistFile *);
  bool ImportDiskExists(char *,EasyStr &),DoImport();
  void ShowImportDiag(); //public
#endif
#endif 
#ifdef UNIX
  static int WinProc(TDiskManager*,Window,XEvent*);
  void set_path(EasyStr,bool=true,bool=true);
  void UpdateDiskNames(int);
  void ToggleReadOnly(int);
  Str GetCustomDiskImage(int*,int*,int*);
  void set_home(Str);
  static int dir_lv_notify_handler(hxc_dir_lv*,int,int);
  static int button_notify_handler(hxc_button*,int,int*);
	static int menu_popup_notifyproc(hxc_popup*,int,int);
  static int diag_lv_np(hxc_listview *,int,int);
  static int diag_but_np(hxc_button *,int,int*);
  static int diag_ed_np(hxc_edit *,int,int);
  void RefreshDiskView(Str=""); //public
#endif
  TDiskManager();
  ~TDiskManager();
  void Show(),Hide(),ToggleVisible();
  void LoadData(bool FirstLoad,TConfigStoreFile *pCSF,bool *SecDisabled=NULL);
  void SaveData(bool FinalSave,TConfigStoreFile *pCSF);
  void SwapDisks(int FocusDrive);
#if defined(SSE_DISK_SWAPPER)
  BOOL ChangeDisk(int floppyno,int direction,BOOL bStatusbar);
#endif
  bool InsertDisk(int drive,EasyStr Name,EasyStr Path,bool DontChangeDisk=false,
                  bool MakeFocus=true,EasyStr DiskInZip="",
                  bool SuppressErr=false,bool AllowInsert2=false);
  void EjectDisk(int drive,bool losechanges=false);
  bool AreNewDisksInHistory(int d);
  void InsertHistoryAdd(int d,char *Name,char *Path,char *DiskInZip="");
  void InsertHistoryDelete(int d,char *Name,char *Path,char *DiskInZip="");
  bool CreateDiskImage(char *STName,WORD Sectors,WORD SecsPerTrack,WORD Sides);
  EasyStr CreateDiskName(char *Name,char *DiskInZip);
  void SetNumFloppies(int NewNum);
  void ExtractDisks(Str Path);
  void InitGetContents();
  void ShowDatabaseDiag(),ShowContentDiag();

#ifdef WIN32
  bool HasHandledMessage(MSG *mess); // public
  void SetDir(EasyStr NewFol,bool AddToHistory,EasyStr SelPath="",
              bool EditLabel=false,EasyStr SelLinkPath="",int iSelItem=0);
  bool SelectItemWithPath(char *Path,bool EditLabel=false,char *LinkPath=NULL);
  bool SelectItemWithLinkPath(char *LinkPath,bool EditLabel=false) {
    return SelectItemWithPath(NULL,EditLabel,LinkPath);
  }
  void RefreshDiskView(EasyStr SelPath="",bool EditLabel=false,
                       EasyStr SelLinkPath="",int iItem=0);
  int GetSelectedItem();
  TDiskManFileInfo *GetItemInf(int iItem,HWND LV=NULL); //v402 not inline
  void ShowLinksDiag(),ShowPropDiag(),ShowDiskDiag();
  int GetDiskSelectionSize();
  void SetDiskViewMode(int Mode);
  void LoadIcons();
  void SetDriveViewEnable(int drive,bool EnableIt);
  HWND VisibleDiag() { // used by HasHandledMessage()
    return (HWND)((DWORD_PTR)DiskDiag^(DWORD_PTR)LinksDiag^(DWORD_PTR)PropDiag
#if !defined(SSE_NO_WINSTON_IMPORT)
      ^(DWORD_PTR)ImportDiag
#endif
      ^(DWORD_PTR)ContentDiag^(DWORD_PTR)DatabaseDiag);
  }
#endif
  //DATA
  EasyStr HistBack[10],HistForward[10];
  EasyStr SaveSelPath;
  EasyStr ContentsLinksPath;
  EasyStr DisksFol,HomeFol,ContentListsFol;
  EasyStr QuickFol[10];
  struct {
    EasyStr Name,Path,DiskInZip;
  }InsertHist[2][10];
  TDiskManFileInfo PropInf;
  bool HideBroken,CloseAfterIRR,HideExtension,ShowHiddenFiles;
  bool Maximized,FSMaximized,SmallIcons;
  BYTE AutoInsert2,nFloppyDrives;
#if defined(SSE_DISK_SWAPPER)
  bool bSwapperPattern; // look for filename with just different digit or letter
  bool ArchiveNoMore;
  int IsDiskImage(char *name); // also checks inside archive
#endif
  bool EjectDisksWhenQuit;
  bool bArchiveRW,bTurboDrive,floppy_access_ff;
  BYTE mBackground;
  EasyStringList contents_sl;
  TBpbInfo bpbi,file_bpbi,final_bpbi;
  int IconSpacing, DoubleClickAction,ContentConflictAction,SaveScroll;
  int SortBy;
  int Width,Height,FSWidth,FSHeight;
  WORD SecsPerTrackIdx,TracksIdx,SidesIdx; // Idx = actual numbers now
  EasyStringList MenuESL;
#ifdef WIN32
  EasyStr MultipleLinksPath,LinksTargetPath;
  WNDPROC Old_ListView_WndProc;
  HIMAGELIST il[2];
  HWND DragLV;
  HIMAGELIST DragIL;
  HWND DiskView;
  HICON DriveIcon[2],AccurateFDCIcon,DisableDiskIcon;
  HWND DatabaseDiag,ContentDiag,DiskDiag,LinksDiag,PropDiag,DiagFocus;
  HANDLE MSAConvProcess;
  int Dragging,DragWidth,DragHeight,DropTarget;
  int LastOverID;
  int MenuTarget;  
  Str MSAConvPath,MSAConvSel;
  Str DatabaseFind;
  
  bool DragEntered,EndingDrag;
  bool AtHome;
  bool ExplorerFolders;
  bool DoExtraShortcutCheck;
#if !defined(SSE_NO_WINSTON_IMPORT)
  HWND ImportDiag;
  bool Importing;
  EasyStr WinSTonPath,WinSTonDiskPath,ImportPath;
  bool ImportOnlyIfExist;
  int ImportConflictAction;
#endif
#endif//WIN32
#ifdef UNIX
  int HistBackLength,HistForwardLength;
  int ArchiveTypeIdx;
  bool TempEject_InDrive[2];
  Str TempEject_Name,TempEject_DiskInZip[2];
  hxc_dir_lv dir_lv;
  hxc_button UpBut,BackBut,ForwardBut,eject_but[2];
  hxc_button DirOutput,disk_name[2],drive_icon[2];
  hxc_button HomeBut,SetHomeBut,MenuBut;
  hxc_button HardBut;
#endif//UNIX
};

extern TDiskManager DiskMan; // singleton

#pragma pack(pop)

#endif//DISKMAN_DECLA_H
