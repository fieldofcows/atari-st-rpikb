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

DOMAIN: Disk image
FILE: floppy_disk.h
DESCRIPTION: Declarations for disk image support.
struct TImageType, TFloppyDisk, TImageMfm
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSEDISK_H
#define SSEDISK_H

#include <easystr.h>
#include "fdc.h"
#include "parameters.h"


enum EDiskImage { 
  // images
  FIMAGE_OK=0,
  FIMAGE_WRONGFORMAT,
  FIMAGE_CANTOPEN,
  FIMAGE_FILEDOESNTEXIST,
  FIMAGE_NODISKSINZIP,
  FIMAGE_CORRUPTZIP,
  FIMAGE_DIMNOMAGIC,
  FIMAGE_DIMTYPENOTSUPPORTED,
  FIMAGE_IS_CONFIG,
  FLOPPY_MAX_BYTESPERSECTOR=512,
  // extensions
  EXT_NONE=0,EXT_ST,EXT_MSA,EXT_DIM,EXT_STT,EXT_STX,EXT_IPF,EXT_CTR,EXT_STG,
  EXT_STW,EXT_PRG,EXT_TOS,EXT_SCP,EXT_HFE,NUM_EXT,
  // image types
  DISK_UNCOMPRESSED=1,DISK_COMPRESSED,DISK_PASTI,DISK_IS_CONFIG,
  /* image managers
  We start enums at 1 because 0 is used to clear status.
  MNGR_STEEM is Steem's original native emulation.
    MNGR_WD1772 is Steem's new emulation to handle STW disk images.*/
  MNGR_STEEM=1,MNGR_PASTI,MNGR_CAPS,MNGR_WD1772,MNGR_PRG,MNGR_ACSI
};


extern char *extension_list[NUM_EXT];
extern char *dot_ext(int i);

#define DISK_EXT_ST extension_list[EXT_ST]
#define DISK_EXT_MSA extension_list[EXT_MSA]
#define DISK_EXT_DIM extension_list[EXT_DIM]
#define DISK_EXT_STT extension_list[EXT_STT]
#define DISK_EXT_STX extension_list[EXT_STX]
#define DISK_EXT_IPF extension_list[EXT_IPF]
#define DISK_EXT_CTR extension_list[EXT_CTR] 
#define DISK_EXT_STG extension_list[EXT_STG]
#define DISK_EXT_STW extension_list[EXT_STW]
#define DISK_EXT_PRG extension_list[EXT_PRG]
#define DISK_EXT_TOS extension_list[EXT_TOS]
#define DISK_EXT_SCP extension_list[EXT_SCP]
#define DISK_EXT_HFE extension_list[EXT_HFE]


#pragma pack(push, 8)

/*  We separate disk image type in two parts:
    who's in charge (manager), and what kind of image we have (extension).
    This info is recorded for each drive (it is a variable in drive).
*/

struct TImageType {
  BYTE Manager;
  BYTE Extension;
  BYTE RealExtension; // ST, DIM, MSA converted to STW
};


struct TBpbInfo {
  int BytesPerSector,Sectors,SectorsPerTrack,Sides;
};


struct TFloppyDisk { // FloppyDisk[] objects inserted in FloppyDrive[] objects
  TFloppyDisk();
  void Init();
  // gaps
  int BytePositionOfFirstId();
  int BytesToID(BYTE &num); // if num=0, next ID
  int HblsPerSector();
  int nSectors();
  void NextID(BYTE &RecordIdx,int &nHbls);
  int PostIndexGap();
  int PreDataGap();
  int PostDataGap();
  int PreIndexGap();
  int RecordLength();
  int SectorGap();
  int TrackGap();
  int GetIDFields(int Side,int Track,TWD1772IDField *IDList);
  EasyStr GetImageFile() {
    return ImageFile;
  }
  bool IsMSA() {
    return MSATempFile.NotEmpty();
  }
  bool IsZip() {
    return ZipTempFile.NotEmpty();
  }
  bool BeenFormatted() {
    return (Format_f!=NULL);
  }
  bool NotBeenFormatted() {
    return (Format_f==NULL);
  }
  bool SeekSector(int Side,int Track,int Sector,bool Format,bool Freeboot=true);
  long GetLogicalSector(int Side,int Track,int Sector,bool FormatFile=false);
  
  int GetRawTrackData(int Side,int Track);
  bool OpenFormatFile();
  bool ReopenFormatFile();

  EasyStr ImageFile,MSATempFile,ZipTempFile,FormatTempFile;
  EasyStr DiskName,DiskInZip;
  EasyStr StwPath;
  FILE *f,*Format_f;
  BYTE *PastiBuf;
  
  DWORD DiskFileLen;
  DWORD STT_TrackStart[2][FLOPPY_MAX_TRACK_NUM+1];
  DWORD crc32;
  int FormatMostSectors,FormatLargestSector;
  int PastiBufLen;
  WORD STT_TrackLen[2][FLOPPY_MAX_TRACK_NUM+1]; 
  short BytesPerSector,Sides,SectorsPerTrack,TracksPerSide;

  WORD current_byte;
  WORD TrackBytes;
  BYTE current_side; //of the image
  BYTE current_track; //of the image
  BYTE Id; //0,1,2 same as drive

  bool TrackIsFormatted[2][FLOPPY_MAX_TRACK_NUM+1];
  bool STT_File,PastiDisk;
  bool ReadOnly;
  bool DIM_File,ValidBPB;
  bool WrittenTo;
  BYTE Density; // 1=DD 2=HD
};


struct  TImageMfm { // base class for TImageHFE, TImageSCP, TImageSTW
  TImageMfm() {
    Id=2;
    fCurrentImage=NULL;
    Position=0;
  }
  // interface
  virtual void Close()=0;
  virtual WORD GetMfmData(WORD position)=0; 
  virtual bool LoadTrack(BYTE side,BYTE track,bool reload=false)=0;
  virtual void SetMfmData(WORD position,WORD mfm_data)=0;
  void ComputePosition(WORD position);
  void IncPosition();
  // variables
  FILE *fCurrentImage;
  DWORD Position;
  BYTE Id;
};

#pragma pack(pop)

#endif//#ifndef SSEDISK_H
