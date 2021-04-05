#pragma once
#ifndef THD6301_H
#define THD6301_H

#pragma pack(push, 8)

struct THD6301 {
  //DATA 
  // from former ikbd struct so it is compatible with v3.2
  // BYTE instead of bool for C compatibility
  int command_read_count,command_parameter_counter;
  BYTE command;
  BYTE command_param[8];
  BYTE mouse_button_press_what_message;
  int mouse_mode;
  int joy_mode;
  int abs_mouse_max_x,abs_mouse_max_y;
  int cursor_key_mouse_pulse_count_x,cursor_key_mouse_pulse_count_y;
  int relative_mouse_threshold_x,relative_mouse_threshold_y;
  int abs_mouse_scale_x,abs_mouse_scale_y;
  int abs_mouse_x,abs_mouse_y;
  BYTE mouse_upside_down;
  BYTE send_nothing;
  int duration;
  BYTE clock[6];
  DWORD cursor_key_joy_time[6];
  DWORD cursor_key_joy_ticks[4];
  BYTE port_0_joy;
  int abs_mousek_flags;
  BYTE resetting;
  int psyg_hack_stage;
  int clock_vbl_count;
  WORD load_memory_address;
  BYTE ram[128]; 
  int reset_121A_hack,reset_0814_hack,reset_1214_hack;
  int joy_packet_pos,mouse_packet_pos;
  // end former ikbd struct
  COUNTER_VAR ChipCycles,MouseNextTickX,MouseNextTickY;
  COUNTER_VAR hd6301_vbl_cycles,time_of_tdr_to_tdrs;
  COUNTER_VAR current_m68_cycle; // will lag or be forward
  int MouseCyclesPerTickX, MouseCyclesPerTickY;
  short MouseVblDeltaX; // must keep separate for true emu
  short MouseVblDeltaY;
  BYTE Initialised; // we do need a rom
  BYTE Crashed; // oops
  BYTE click_x,click_y; // current click
  // lower case because uppercase are constants in 6301 emu itself
  BYTE rdr,rdrs,tdr,tdrs; 
#if defined(SSE_IKBDI) 
  BYTE LastCommand;
  BYTE CurrentParameter; //0-5
  BYTE nParameters; //0-6
  BYTE Parameter[6]; // max 6
  BYTE CustomProgram;
  BYTE CurrentCommand;
#endif
  enum EProgramTag {
    CUSTOM_PROGRAM_NONE,
    CUSTOM_PROGRAM_LOADING,
    CUSTOM_PROGRAM_LOADED,
    CUSTOM_PROGRAM_RUNNING
  }custom_program_tag;
  //FUNCTIONS
#ifdef __cplusplus //isolate member functions, for C it's just POD
  THD6301();
  ~THD6301();
  void ResetChip(int Cold);
  void ResetProgram();
  void Init();
  void Vbl();
#if defined(SSE_IKBDI) 
  void InterpretCommand(BYTE ByteIn);
#if defined(SSE_DEBUG)
  void ReportCommand();
#endif
#endif
#endif//c++?
};

#pragma pack(pop)

#endif
