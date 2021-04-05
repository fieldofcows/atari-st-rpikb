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

FILE: midi.h
DOMAIN: I/O
DESCRIPTION: Declarations for MIDI input/output.
struct TOutSysEx
class TMIDIOut, TMIDIIn
---------------------------------------------------------------------------*/

#pragma once
#ifndef MIDI_DECLA_H
#define MIDI_DECLA_H

#include "conditions.h"

#include <circularbuffer.h>
#include <easystr.h>
#include "parameters.h"

#ifdef MINGW_BUILD
#define NULL 0
#endif

//int MidiGetStatusNumParams(BYTE StatByte);

#define MIDI_NO_RUNNING_STATUS 1
#define MIDI_ALLOW_RUNNING_STATUS 0
extern int MIDI_out_running_status_flag;
extern int MIDI_in_running_status_flag;
extern int MIDI_in_n_sysex,MIDI_out_n_sysex,MIDI_in_speed;
extern WORD MIDI_out_volume;
extern DWORD MIDI_in_sysex_max,MIDI_out_sysex_max;

#ifdef WIN32
struct TOutSysEx {
  BYTE *pData;
  DWORD Len;
  MIDIHDR *pHdr;
};
#endif

class TMIDIOut {
//private:
public:
#ifdef WIN32
  HMIDIOUT Handle;
private:
  BYTE MessBuf[8];
  int MessBufLen,ParamCount,nStatusParams;
  TOutSysEx SysEx[MAX_SYSEX_BUFS+1];
  TOutSysEx *pCurSysEx;
  int nSysExBufs;
  DWORD MaxSysExLen;
  MIDIHDR SysExHeader[MAX_SYSEX_BUFS];
  DWORD OldVolume;
#endif
public:
  TMIDIOut(int,int);
  bool AllocSysEx();
  void ReInitSysEx();
  ~TMIDIOut();
  void SendByte(BYTE Val);
#ifdef WIN32
  int GetDeviceID();
  bool IsOpen() {
    return (Handle!=NULL);
  }
  bool FreeHeader(MIDIHDR *pHdr);
#endif
  bool SetVolume(int Volume);
  bool Mute();
  void Reset();
  Str ErrorText;
  DWORD TimeLastSent;
};

typedef void MIDIINNOTEMPTYPROC();
typedef MIDIINNOTEMPTYPROC* LPMIDIINNOTEMPTYPROC;

class TMIDIIn {
private:
DEBUG_ONLY(public:)
  CircularBuffer Buf;
#ifdef WIN32
  HMIDIIN Handle;
  MIDIHDR SysExHeader[MAX_SYSEX_BUFS];
  BYTE *SysExBuf[MAX_SYSEX_BUFS];  //[MIDI_SYSEX_BUFFER_SIZE+2]; // +2=Space for SOX and EOX
  volatile bool Killing;
  bool Started;
  DWORD MaxSysExLen;
  int RunningStatus,nSysExBufs;
  static void CALLBACK InProc(HMIDIIN Handle,UINT Msg,DWORD_PTR dwThis,
    DWORD_PTR MidiMess,DWORD_PTR timestamp);
#endif
public:
  TMIDIIn(int Device,bool StartNow,LPMIDIINNOTEMPTYPROC NEP=NULL);
  void AddSysExBufs(),RemoveSysExBufs();
  void ReInitSysEx();
  ~TMIDIIn();
#ifdef WIN32
  int GetDeviceID();
  bool IsOpen() { 
    return (Handle!=NULL);
  }
#endif
  void Reset();
  bool Start();
  void Stop();
  bool AreBytesToCome() {
    return Buf.AreBytesInBuffer();
  }
  BYTE ReadByte() {
    return Buf.ReadByte();
  }
  void NextByte() {
    Buf.NextByte();
  }
  LPMIDIINNOTEMPTYPROC NotEmptyProc;
  Str ErrorText;
  DWORD_PTR LastTimestamp;
  WORD TimeSinceLastNote;
  COUNTER_VAR CycleOfLastNote;
};

#endif//MIDI_DECLA_H
