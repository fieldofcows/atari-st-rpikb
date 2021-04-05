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

DOMAIN: Emu
FILE: fdc.h
DESCRIPTION: Declarations for floppy drive controller emulation.
struct TWD1772IDField, TWD1772MFM, TWD1772Crc, TWD1772Dpll,
TWD1772AmDetector, TWD1772 - guess the FDC model?
---------------------------------------------------------------------------*/

#pragma once
#ifndef WD1772_H
#define WD1772_H

#include "conditions.h"
#include "steemh.h"
#include "mymisc.h"
#include "parameters.h"


#if USE_PASTI
#include <pasti/pasti.h>
// callback functions (__cdecl)
void PASTI_CALLCONV pasti_handle_return(struct pastiIOINFO *pPIOI);
void PASTI_CALLCONV pasti_motor_proc(BOOL state);
void PASTI_CALLCONV pasti_log_proc(const char *text);
void PASTI_CALLCONV pasti_warn_proc(const char *text);
extern HINSTANCE hPasti;
extern COUNTER_VAR pasti_update_time;
extern const struct pastiFUNCS *pasti;
extern char pasti_file_exts[PASTI_FILE_EXTS_BUFFERSIZE];
extern WORD pasti_store_byte_access;
extern bool pasti_active; // when pasti handles ST, MSA, ACSI too
#else
extern const bool pasti_active;
#endif
#if defined(SSE_MEGASTE)
BOOL fdc_check_wrong_density();
#endif

#define FDC_STR_BSY                BIT_0
#define FDC_STR_IP                 BIT_1
#define FDC_STR_DRQ                BIT_1
#define FDC_STR_T0                 BIT_2
#define FDC_STR_LD                 BIT_2
#define FDC_STR_CRC                BIT_3
#define FDC_STR_SE                 BIT_4
#define FDC_STR_RNF                BIT_4
#define FDC_STR_SU                 BIT_5
#define FDC_STR_RT                 BIT_5
#define FDC_STR_WP                 BIT_6
#define FDC_STR_MO                 BIT_7


void fdc_command(BYTE cm);
void fdc_execute();
bool floppy_track_index_pulse_active();
int floppy_current_drive();
int floppy_current_side();
#ifdef SSE_WD1772_CRCTABLE
void fdc_make_crc16_table();
#endif
void fdc_add_to_crc(WORD &crc,BYTE data);
void agenda_fdc_spun_up(int do_exec);
void agenda_fdc_motor_flag_off(int);
void agenda_fdc_finished(int);
void agenda_fdc_seek(int);
void agenda_fdc_readwrite_sector(int Data);
void agenda_fdc_read_address(int idx);
void agenda_fdc_read_track(int part);
void agenda_fdc_write_track(int part);
void agenda_fdc_verify(int);

extern int floppy_mediach[2]; // should be BYTE but complicates snapshot compatibility
extern bool floppy_access_started_ff;
extern BYTE floppy_access_ff_counter;
extern BYTE floppy_irq_flag;
extern WORD floppy_write_track_bytes_done;
extern BYTE fdc_spinning_up; // can be 2
extern const BYTE fdc_step_time_to_hbls[4];

#define FLOPPY_ACCESS_FF(drive) (DiskMan.floppy_access_ff&&FloppyDrive[drive].adat)

#pragma pack(push, 8)


struct TWD1772IDField {
/*
Byte #     Meaning                |     Sector length code     Sector length
------     ------------------     |     ------------------     -------------
1          Track                  |     0                      128
2          Side                   |     1                      256
3          Sector                 |     2                      512
4          Sector length code     |     3                      1024
5          CRC byte 1             |
6          CRC byte 2             |
*/
  // FUCNTIONS
  inline TWD1772IDField() {
    ZeroMemory(this,sizeof(TWD1772IDField));
  }
  inline WORD nBytes() {
    return (1<<( (len&3) +7)); // other way: harder
  }
  void Trace(); // empty body if not debug
  // DATA
  BYTE track;
  BYTE side;
  BYTE num;
  BYTE len;
  BYTE CRC[2]; // not a WORD because ST was big-endian
};


struct TWD1772MFM {
  enum EWD1772MFM {NORMAL_CLOCK,FORMAT_CLOCK};
  WORD encoded;
  BYTE clock,data; 
  unsigned int data_last_bit:1;
  void Decode ();
  void Encode(int mode=NORMAL_CLOCK);
};


struct TWD1772Crc {
  DWORD crccnt;
  WORD crc;
  inline void Reset() {
/*  Contrary to what the doc states, the CRC Register isn't preset to ones 
    ($FFFF) prior to data being shifted through the circuit, but to $CDB4.
    This happens for each $A1 address mark (read or written), so the register
    value after $A1 is the same no matter how many address marks (Dragonflight). 
*/
    crc=0xCDB4;
  }
  inline void Add(BYTE data) {
    fdc_add_to_crc(crc,data); // we just call Steem's original function for now
  }
  bool Check(TWD1772IDField *IDField);
};



#if defined(SSE_WD1772_LL) // 3rd party

struct TWD1772Dpll {
  int GetNextBit(COUNTER_VAR &tm, int drive);
  void Reset(COUNTER_VAR when);
  void SetClock(const int &period);
  COUNTER_VAR latest_transition;
  COUNTER_VAR ctime;
  int delays[42];
  COUNTER_VAR write_start_time;
  int write_buffer[32];
  int write_position;
  WORD counter;
  WORD increment;
  WORD transition_time;
  BYTE history;
  BYTE slot;
  BYTE phase_add, phase_sub, freq_add, freq_sub;
};

#endif

struct TWD1772AmDetector {
#if defined(SSE_WD1772_LL)
  DWORD amdecode,aminfo,amisigmask;
  // we keep those here because it's 32bit and integrated in the logic:
  int dsr,dsrcnt; 
  BYTE amdatadelay,ammarkdist,ammarktype,amdataskip;
#endif
  BYTE nA1; // counter
  void Enable();
  bool Enabled;
  void Reset();
};


struct TWD1772 {

  // ENUM
  enum EWD1772 { CR_U=BIT_4,CR_M=BIT_4,CR_H=BIT_3,CR_V=BIT_2,CR_E=BIT_2,
    CR_R1=BIT_1,CR_P=BIT_1,CR_R0=BIT_0,CR_A0=BIT_0,
    CR_I3=BIT_3,CR_I2=BIT_2,CR_I1=BIT_1,CR_I0=BIT_0,
    CR_STEP=(BIT_6|BIT_5),CR_STEP_IN=BIT_6,CR_STEP_OUT=CR_STEP,
    CR_SEEK=BIT_4,CR_RESTORE=0,
    CR_TYPEII_WRITE=BIT_5,CR_TYPEIII_WRITE=BIT_4,CR_TYPEIII_READ_ADDRESS=0xC0,
/*  We identify various phases in the Western Digital flow charts, so that we
    know where to "jump" next.
    We have two delay sources: current algorithm processing and index pulses.
    This could correspond to two different programs running in parallel. (?)
*/
    WD_READY=0,
    WD_TYPEI_SPINUP,
    WD_TYPEI_SPUNUP, // spunup must be right after spinup
    WD_TYPEI_SEEK,
    WD_TYPEI_STEP_UPDATE,
    WD_TYPEI_STEP,
    WD_TYPEI_STEP_PULSE,
    WD_TYPEI_CHECK_VERIFY,
    WD_TYPEI_HEAD_SETTLE,
    WD_TYPEI_FIND_ID,
    WD_TYPEI_READ_ID, // read ID must be right after find ID
    WD_TYPEI_TEST_ID, // test ID must be right after read ID
    WD_TYPEII_SPINUP,
    WD_TYPEII_SPUNUP, // spunup must be right after spinup
    WD_TYPEII_HEAD_SETTLE, //head settle must be right after spunup
    WD_TYPEII_FIND_ID,
    WD_TYPEII_READ_ID, // read ID must be right after find ID
    WD_TYPEII_TEST_ID, // test ID must be right after read ID
    WD_TYPEII_FIND_DAM,
    WD_TYPEII_READ_DATA,
    WD_TYPEII_READ_CRC,
    WD_TYPEII_CHECK_MULTIPLE,
    WD_TYPEII_WRITE_DAM,
    WD_TYPEII_WRITE_DATA,
    WD_TYPEII_WRITE_CRC,
    WD_TYPEIII_SPINUP,
    WD_TYPEIII_SPUNUP, // spunup must be right after spinup
    WD_TYPEIII_HEAD_SETTLE, //head settle must be right after spunup
    WD_TYPEIII_IP_START, // start read/write
    WD_TYPEIII_FIND_ID,
    WD_TYPEIII_READ_ID, // read ID must be right after find ID
    WD_TYPEIII_TEST_ID,
    WD_TYPEIII_READ_DATA,
    WD_TYPEIII_WRITE_DATA,
    WD_TYPEIII_WRITE_DATA2, // CRC is 1 byte in RAM -> 2 bytes on disk
    WD_TYPEIV_4, // $D4
    WD_TYPEIV_8, // $D8
    WD_MOTOR_OFF
  };

  //FUNCTIONS
  int CommandType(int command=-1); // I->IV
  BYTE IORead(BYTE Line);
  void IOWrite(BYTE Line,BYTE io_src_b);
  void Reset();
  void NewCommand(BYTE command);
  void Drq(bool state); //no default to make it clearer
  void Irq(bool state);
  void Motor(bool state);
  void Read(); // sends order to drive
  void StepPulse();
  int MsToCycles(int ms);
  void Write(); // sends order to drive
  void WriteCR(BYTE io_src_b); //horrible TODO
  void OnUpdate();
  // called by drive or by image
  void OnIndexPulse(int Id, bool image_triggered); 
#if defined(SSE_WD1772_LL)
  bool ShiftBit(int bit);
#endif
  TWD1772() {
    ZeroMemory(this,sizeof(TWD1772));
  }

  // DATA
  int prg_phase;
  COUNTER_VAR update_time; // when do we need to come back?
  WORD ByteCount; // guessed
  // definition is outside the class but objects belong to the class
  TWD1772IDField IDField; // to R/W those fields
  TWD1772Crc CrcLogic;
#if defined(SSE_WD1772_LL)
  TWD1772Dpll Dpll;
#endif
  TWD1772AmDetector Amd; // not the processor
  TWD1772MFM Mfm;
  BYTE cr;  // command
  BYTE str; // status
  BYTE tr;  // track
  BYTE sr;  // sector
  BYTE dr;  // data
  BYTE dsr; // shift register - official
  bool F7_escaping;
  BYTE n_format_bytes; // to examine sequences before ID
  BYTE StatusType; // guessed
  BYTE InterruptCondition; // guessed
  BYTE IndexCounter; // guessed
  BYTE TimeOut;
  BYTE old_cr;
  COUNTER_VAR current_time;
/*  Lines (pins). Some are necessary (eg direction), others not
    really yet (eg write_gate).
    TODO put them all in then (fun)
*/
  struct TWD1772Lines {
    bool drq;
    bool irq;
    bool motor;
    bool direction;
    bool track0;  
    bool step; 
    bool read;
    bool write;
    bool write_protect;
    bool write_gate;
    bool ip;
#if defined(SSE_DISK_GHOST)
    bool CommandWasIntercepted; // pseudo
#endif
  }Lines;

};

#pragma pack(pop)

#endif//#ifndef WD1772_H
