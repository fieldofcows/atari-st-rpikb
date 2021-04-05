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

DOMAIN: Rendering
FILE: sound.h
DESCRIPTION: Declarations for sound output.
struct TLMC1992
---------------------------------------------------------------------------*/

#pragma once
#ifndef INITSOUND_DECLA_H
#define INITSOUND_DECLA_H

#include <easystr.h>
#include <dsp/dsp.h>
#ifdef WIN32
#include <dsound.h>
#endif


#if (SCREENS_PER_SOUND_VBL == 1) // in conditions.h
#define MOD_PSG_BUF_LENGTH %psg_buf_length
#else
#define MOD_PSG_BUF_LENGTH %psg_buf_length
extern int sys_time_of_last_sound_vbl ;
#endif


enum ESound {SOUND_PSG_HLF_NONE,SOUND_PSG_HLF_EMULATED,SOUND_PSG_HLF_CHIP,
SOUND_PSG_HLF_MONITOR};


HRESULT InitSound();
void SoundRelease();
HRESULT Sound_Start(),Sound_Stop();
void SoundCreateDynamicBuffer();
HRESULT SoundStartBuffer(int flatlevel1,int flatlevel2);
HRESULT SoundLockBuffer(DWORD Start,DWORD Len,LPVOID *lpDatAdr1,
            DWORD *lpLockLength1,LPVOID *lpDatAdr2,DWORD *lpLockLength2);
void SoundUnlock(LPVOID DatAdr1,DWORD LockLength1,LPVOID DatAdr2,
                    DWORD LockLength2);
HRESULT SoundError(char *ErrorText,HRESULT DErr);
DWORD SoundGetTime();
void sound_record_open_file();
void sound_record_close_file();
void SoundChangeVolume();
bool SoundActive();
HRESULT Sound_VBL();


#if !defined(SSE_NO_INTERNAL_SPEAKER)
extern void SoundStopInternalSpeaker();
extern bool sound_internal_speaker;
#endif


#ifdef WIN32
Str SoundLogError(Str,HRESULT);
extern IDirectSoundBuffer *PrimaryBuf,*SoundBuf;
#endif


#ifdef UNIX

#define XS_PA 1
#define XS_RT 2
#define XS_PULSE 3

// Enough for 500ms at 100Khz in 16-bit stereo
#define X_SOUND_BUF_LEN_BYTES (50000*4)

extern void internal_speaker_sound_by_period(int);
void XSoundInitBuffer(int,int);

#ifndef NO_PORTAUDIO
#include <portaudio/include/portaudio.h>
#define PA_ONLY(s) s
HRESULT PA_Init();
void PA_Release();
DWORD PA_GetTime();
HRESULT PA_StartBuffer(int flatlevel1,int flatlevel2);
bool PA_IsPlaying();
int PA_Callback(const void*,void *pOutBuf,unsigned long Samples, const
              PaStreamCallbackTimeInfo* OutTime,PaStreamCallbackFlags, 
              void*);
void PA_ChangeVolume();
void PA_FreeBuffer(bool Immediate);              
HRESULT PA_Stop(bool Immediate);
extern int pa_output_buffer_size;
extern bool pa_init;
extern PaDeviceIndex pa_out_dev;
extern PaSampleFormat pa_format;
#else
#define PA_ONLY(s)
#endif

#ifndef NO_RTAUDIO
#include <rtaudio/RtAudio.h>
#define RT_ONLY(s) s
HRESULT Rt_Init();
DWORD Rt_GetTime();
void Rt_Release();
int Rt_Callback(void *pOutBuf, void*, unsigned int bufferSize, double, 
                RtAudioStreamStatus, void*);
HRESULT Rt_StartBuffer(int flatlevel1,int flatlevel2);
bool Rt_IsPlaying();
void Rt_ChangeVolume();
void Rt_FreeBuffer(bool);
HRESULT Rt_Stop(bool Immediate);
extern RtAudio *rt_audio;
extern int rt_unsigned_8bit;
extern int rt_buffer_size,rt_buffer_num;
extern RtAudio *rt_audio;
extern int rt_unsigned_8bit;
#else
#define RT_ONLY(s)
#endif

#if defined(SSE_UNIX_PULSEAUDIO)

#include <pulse/pulseaudio.h>

HRESULT Pulse_Init();
DWORD  Pulse_GetTime();
void Pulse_Release();
DWORD Pulse_GetTime();
HRESULT Pulse_StartBuffer(int flatlevel1,int flatlevel2);
bool Pulse_IsPlaying();
void Pulse_ChangeVolume();
void Pulse_FreeBuffer(bool Immediate);              
HRESULT Pulse_Stop(bool Immediate);

int Pulse_DeviceList(char *DevName);
extern bool pulse_init;
extern pa_stream *stream;
extern pa_context *context;
extern pa_channel_map channel_map;
extern int channel_map_set;
extern char *stream_name,*client_name,*pulse_device;
#endif

extern int x_sound_lib;
extern EasyStr sound_device_name;
extern int console_device;
extern int sound_buf_pointer;
extern int console_device,x_sound_lib;
extern BYTE x_sound_buf[X_SOUND_BUF_LEN_BYTES+16];

#endif//UNIX


#ifdef SHOW_WAVEFORM

extern void draw_waveform();
extern int temp_waveform_display_counter;
#define MAX_temp_waveform_display_counter psg_buf_length
extern BYTE temp_waveform_display[DEFAULT_SOUND_BUFFER_LENGTH];
extern DWORD temp_waveform_play_counter;

#endif


extern FILE *Wav_file;
extern int *psg_channels_buf;
extern DWORD psg_last_play_cursor;
extern DWORD psg_last_write_time;
extern DWORD psg_time_of_start_of_buffer;
extern DWORD psg_time_of_last_vbl_for_writing,psg_time_of_next_vbl_for_writing;
extern int psg_n_samples_this_vbl;
extern int sound_freq,sound_comline_freq,sound_chosen_freq;
extern DWORD sound_record_start_time; //by timer variable = timeGetTime()
extern BYTE psg_write_n_screens_ahead;
extern int SoundVolume;
extern BYTE UseSound;
extern DWORD psg_channels_buf_len;
extern int psg_buf_pointer[3];
extern int psg_buf_length;
extern DWORD SoundBufStartTime;

#if defined(SSE_YM2149_LL)
extern const WORD ym_low_pass_max;
#endif

extern bool DSOpen,TrySound,sound_low_quality;
extern BYTE sound_num_channels,sound_num_bits,sound_bytes_per_sample;
extern bool sound_click_at_start;
extern bool sound_record;
extern BYTE psg_hl_filter;
extern bool ste_sound_on_this_screen;
#if defined(SSE_SOUND_ENFORCE_RECOM_OPT)
extern const bool sound_write_primary;
extern const BYTE sound_time_method;
#else
extern bool sound_write_primary;
extern BYTE sound_time_method ;
#endif


struct TLMC1992 {
  TLMC1992() {
    memset(this,0,sizeof(TLMC1992));
  }
  void Process(int channel,int &val);
  void Reset(bool Cold);
  COUNTER_VAR StartTime;
  WORD Mask,Data;
  BYTE mixer,volume;
  BYTE bass;
  BYTE treble;
  BYTE volume_l,volume_r;
  BYTE top_val_l,top_val_r;
  TIirLowShelf MicrowireBass[2];
  TIirHighShelf MicrowireTreble[2];
  TIirVolume MicrowireVolume[2];
  BYTE old_top_val_l,old_top_val_r;
};

#endif//#ifndef INITSOUND_DECLA_H
