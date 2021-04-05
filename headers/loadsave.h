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

DOMAIN: File
FILE: loadsave.h
DESCRIPTION: Declarations for loading and saving memory snapshots.
---------------------------------------------------------------------------*/
#pragma once
#ifndef LOADSAVE_DECLA_H
#define LOADSAVE_DECLA_H


#if defined(SSE_VERSION)
// rather silly but so we leave the define here and we know which version goes with sse version
#if SSE_VERSION>=410
#define SNAPSHOT_VERSION 63
#elif SSE_VERSION>=402 && SSE_VERSION_R>=3 // change snapshot version, keep Steem version
#define SNAPSHOT_VERSION 62
#elif SSE_VERSION>=402
#define SNAPSHOT_VERSION 61
#elif SSE_VERSION>=400
#define SNAPSHOT_VERSION 60
#elif SSE_VERSION>=394
#define SNAPSHOT_VERSION 58
#elif SSE_VERSION>=393
#define SNAPSHOT_VERSION 57
#elif SSE_VERSION>=392
#define SNAPSHOT_VERSION 56
#elif SSE_VERSION>=391
#define SNAPSHOT_VERSION 55
#elif SSE_VERSION>=390
#define SNAPSHOT_VERSION 54
#elif SSE_VERSION>=382
#define SNAPSHOT_VERSION 53
#elif SSE_VERSION>=380
#define SNAPSHOT_VERSION 52
#elif SSE_VERSION>=372
#define SNAPSHOT_VERSION 51
#elif SSE_VERSION>=371
#define SNAPSHOT_VERSION 50
#elif SSE_VERSION>=370
#define SNAPSHOT_VERSION 49
#elif SSE_VERSION>=361
#define SNAPSHOT_VERSION 48
#elif SSE_VERSION>=360
#define SNAPSHOT_VERSION 47 //unused
#elif SSE_VERSION>=354//SSE_VERSION>=353
#define SNAPSHOT_VERSION 46
#elif SSE_VERSION>=352
#define SNAPSHOT_VERSION 45
#elif SSE_VERSION>=351
#define SNAPSHOT_VERSION 44
#elif SSE_VERSION>=350
#define SNAPSHOT_VERSION 43
#elif SSE_VERSION>=340
#define SNAPSHOT_VERSION 42
#elif SSE_VERSION==330
#define SNAPSHOT_VERSION 41
#endif

#else
#define SNAPSHOT_VERSION 40
#endif

#define LS_LOAD 0
#define LS_SAVE 1

extern BYTE snapshot_loaded;

#ifdef ONEGAME
extern int LoadSaveAllStuff(BYTE* &,bool,int =(-1),bool =(true),int* =(NULL));
#else
extern int LoadSaveAllStuff(FILE *f,bool LoadOrSave,int Version=-1,
                            bool ChangeDisksAndCart=true,int *pVerRet=NULL);
#endif
extern void LoadSnapShotUpdateVars(int Version);

void AddSnapShotToHistory(char *FilNam);
void SaveSnapShot(char *FilNam,int Version=-1,bool AddToHistory=true);

void LoadState(TConfigStoreFile *pCSF);
void SaveState(TConfigStoreFile *pCSF);

#ifdef ENABLE_LOGFILE
void load_logsections();
#endif

bool LoadSnapShot(char *FilNam,bool AddToHistory=true,bool ShowErrorMess=true,
                  bool ChangeDisks=true);
void LoadSnapShotChangeTOS(Str NewROM,int NewROMVer,int NewROMCountry);
void LoadSnapShotChangeCart(Str NewCart);
void LoadSnapShotChangeDisks(Str NewDisk[2],Str NewDiskInZip[2],
                             Str NewDiskName[2]);
void LoadSavePastiActiveChange();

#endif//#ifndef LOADSAVE_DECLA_H
