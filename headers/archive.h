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
FILE: archive.h
DESCRIPTION: Declarations for archive handling.
class zipclass
---------------------------------------------------------------------------*/

#pragma once
#ifndef ARCHIVE_DECLA_H
#define ARCHIVE_DECLA_H

#include <easystr.h>
#include <easystringlist.h>

#ifdef UNIX
#include <unzip.h>
#endif

#ifdef WIN32
#include <various/unzip_win32.h>
#include <ArchiveAccess/ArchiveAccess/ArchiveAccessSSE.h>
#if !defined(SSE_NO_UNZIPD32)
extern HINSTANCE hUnzip;
void LoadUnzipDLL();
#endif
#endif

extern bool enable_zip;

#ifndef NO_RARLIB
#define RARLIB_SUPPORT
#endif

#ifdef RARLIB_SUPPORT
#include <unrarlib/unrarlib/unrarlib.h>	
#endif

#if defined(SSE_UNRAR_SUPPORT_WIN)
#include <UnRARDLL/unrar.h>
void LoadUnrarDLL();
extern HINSTANCE hUnrar;
extern HANDLE (PASCAL *rarOpenArchive)(struct RAROpenArchiveData *ArchiveData);
extern int (PASCAL *rarCloseArchive)(HANDLE hArcData);
extern int (PASCAL *rarReadHeader)(HANDLE hArcData,struct RARHeaderData 
                                   *HeaderData);
extern int (PASCAL *rarProcessFile)(HANDLE hArcData,int Operation,
                                    char *DestPath,char *DestName);
#endif

#define ZIPPY_FAIL TRUE
#define ZIPPY_SUCCEED FALSE

#pragma pack(push, 8)

class zipclass {
private:
public:
#ifdef UNIX
  //use zlib
  unzFile uf;
  unz_global_info gi;
  char filename_inzip[256];
  unz_file_info fi;
#endif
#ifdef WIN32
  TPackRec PackInfo;
#endif
#ifdef RARLIB_SUPPORT
  ArchiveList_struct *rar_list,*rar_current;
#endif
#if defined(SSE_UNRAR_SUPPORT_WIN)
  // persistent, good idea?
  RAROpenArchiveData ArchiveData;
  RARHeaderData HeaderData; // no Ex?
  HANDLE hArcData;
#endif
  DWORD crc;
  int current_file_n,current_file_offset,err;
  char type[12]; // extension
//  EasyStr last_error;
  WORD m_attrib;
  bool is_open;
  bool bCRC; // flag extract archive so we get CRC32 (archiveaccess)
  zipclass();
  //~zipclass(){ };
#if defined(SSE_ARCHIVEACCESS_SUPPORT)  
  bool for_archiveaccess();
#endif
#if defined(SSE_7Z_SUPPORT_UNIX)  
  bool for_7za();
#endif  
  bool first(char *name);
  bool next();
  bool close();
  char* filename_in_zip();
  void list_contents(char *name,EasyStringList *eslp,bool st_disks_only=false);
  bool extract_file(char *fn,int offset,char *dest_dir,bool hide=false,
                    DWORD attrib=0,BOOL DontClose=FALSE);
};

#pragma pack(pop)

extern zipclass zippy; 

#endif//ARCHIVE_DECLA_H
