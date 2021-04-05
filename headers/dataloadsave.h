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
FILE: dataloadsave.h
DESCRIPTION: Declarations for option load/save to and from the ini file.
struct TProfileSectionData
---------------------------------------------------------------------------*/

#pragma once
#ifndef DATALOADSAVE_DECLA_H
#define DATALOADSAVE_DECLA_H

#include <easystr.h>
#include <configstorefile.h>


#pragma pack(push, 8)

struct TProfileSectionData {
  char *Name;
  int ID;
};

#pragma pack(pop)

extern TProfileSectionData ProfileSection[21];

void LoadAllDialogData(bool FirstLoad,Str INIFile,bool *SecDisabled=NULL,
                       TConfigStoreFile *pCSF=NULL);
void SaveAllDialogData(bool FinalSave,Str INIFile,TConfigStoreFile *pCSF=NULL);

#endif//DATALOADSAVE_DECLA_H
