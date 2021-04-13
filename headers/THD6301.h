#pragma once
#ifndef THD6301_H
#define THD6301_H

#pragma pack(push, 8)

struct THD6301 {
  //DATA 
  WORD load_memory_address;
  COUNTER_VAR ChipCycles;
  COUNTER_VAR MouseNextTickX;
  COUNTER_VAR MouseNextTickY;
  COUNTER_VAR current_m68_cycle; // will lag or be forward
  short MouseVblDeltaX; // must keep separate for true emu
  short MouseVblDeltaY;
  BYTE Initialised; // we do need a rom
  BYTE Crashed; // oops
  BYTE click_x,click_y; // current click
  // lower case because uppercase are constants in 6301 emu itself
  BYTE rdr,rdrs,tdr,tdrs; 
  //FUNCTIONS
#ifdef __cplusplus //isolate member functions, for C it's just POD
  THD6301();
  ~THD6301();
  void ResetChip(int Cold);
  void Init();
#endif//c++?
};

#pragma pack(pop)

#endif
