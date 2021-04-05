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
FILE: psg.h
DESCRIPTION: Declarations for the Yamaha YM2149 PSG (Programmable Sound
Chip).
struct TYM2149
---------------------------------------------------------------------------*/

#pragma once
#ifndef SSEYM2149_H
#define SSEYM2149_H

#include <sound.h>
#include <dsp/FIR-filter-class/filt.h> // 3rd party, AntiAlias filter


#define PSGR_TONE_PERIOD_LOW_A      0
#define PSGR_TONE_PERIOD_HIGH_A     1
#define PSGR_TONE_PERIOD_LOW_B      2
#define PSGR_TONE_PERIOD_HIGH_B     3
#define PSGR_TONE_PERIOD_LOW_C      4
#define PSGR_TONE_PERIOD_HIGH_C     5
#define PSGR_NOISE_PERIOD           6
#define PSGR_MIXER                  7
#define PSGR_AMPLITUDE_A            8
#define PSGR_AMPLITUDE_B            9
#define PSGR_AMPLITUDE_C           10
#define PSGR_ENVELOPE_PERIOD_LOW   11
#define PSGR_ENVELOPE_PERIOD_HIGH  12
#define PSGR_ENVELOPE_SHAPE        13
#define PSGR_PORT_A                14
#define PSGR_PORT_B                15


/*  In v3.5.1, TYM2149 was only used for drive management (drive, side).
    In v3.7.0, sound functions were introduced (sampled soundchip, more realistic).
    In v3.9.2, it harbours an alternative PSG emu based on MAME.
*/

#pragma pack(push, 8)

struct TYM2149 {
  enum EYM2149 {NO_VALID_DRIVE=0xFF,NUM_CHANNELS=3};
  // FUNCTIONS
  TYM2149();
  ~TYM2149();
  BYTE CurrentDrive(); // may return NO_VALID_DRIVE
  void CheckFreeboot();
  void FreeFixedVolTable();
  bool LoadFixedVolTable(bool check_dmamix=false);
  void Reset();
  void Restore();
#if defined(SSE_YM2149_LL)
  void psg_write_buffer(DWORD to_t, bool vbl=false);
#endif
  // DATA
  WORD *p_fixed_vol_3voices; // WORD[16*16*16] = 4096
#if defined(SSE_YM2149_LL)
  double ym2149_cycles_per_sample;
  Filter* AntiAlias;
  COUNTER_VAR m_cycles; // added
  COUNTER_VAR time_at_vbl_start,time_of_last_sample;
  int frame_samples;
  int m_count[NUM_CHANNELS];
  int m_count_noise;
  int m_count_env;
  int m_rng;
  DWORD m_env_volume;
  BYTE m_output[NUM_CHANNELS];
  BYTE m_env_step_mask;
  char m_env_step;
  BYTE m_hold,m_alternate,m_attack,m_holding;
  BYTE m_prescale_noise;
  BYTE m_vol_enabled[NUM_CHANNELS];
#endif
  BYTE SelectedDrive; //0/1 (use CurrentDrive() to check validity)
  BYTE SelectedSide;  //0/1
};

#pragma pack(pop)

void psg_write_buffer(int abc,DWORD to_t);
void psg_set_reg(int reg,BYTE old_val,BYTE &new_val);
extern BYTE psg_reg[16];
extern int psg_voltage,psg_dv;
extern const int psg_flat_volume_level[16];
extern const int psg_envelope_level[8][64];
extern DWORD psg_tone_start_time[TYM2149::NUM_CHANNELS];
extern DWORD psg_envelope_start_time;
extern BYTE psg_reg_select,psg_reg_data;
extern bool written_to_env_this_vbl;

#endif//SSEYM2149_H
