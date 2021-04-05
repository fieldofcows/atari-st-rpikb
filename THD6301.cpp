#include "6301/6301.h"
#include "ikbd.h"
#include <string.h>
#include <iostream>

#define TRACE_LOG printf

THD6301::THD6301()
{
    memset(this, 0, sizeof(THD6301));
}

THD6301::~THD6301()
{
#if defined(SSE_HD6301_LL)
    hd6301_destroy(); // calling the C 6301 function
#endif
}

void THD6301::Init()
{ // called in 'main'
    Initialised = Crashed = 0;
    BYTE *pram = hd6301_init();
    if (!pram)
        return;
    const int romlen = 4096;
    int rombase = 256; // miniram
    FILE *fp = fopen("/mnt/c/src/st_emulator/rpi6301/rom/HD6301V1ST.img", "r+b");
    if (!fp)
    {
        std::cerr << "Could not open ROM file" << std::endl;
        return;
    }
    fread(pram + rombase, 1, romlen, fp);
    fclose(fp);
    Initialised = 1;
}

void THD6301::ResetChip(int Cold)
{
    TRACE_LOG("6301 Reset chip %d\n", Cold);
#if defined(SSE_IKBDI)
    CustomProgram = CUSTOM_PROGRAM_NONE;
    ResetProgram();
#endif
#if defined(SSE_HD6301_LL)
    hd6301_reset(Cold);
#endif
    if (Cold) // real cold
    {
        memset(ST_Key_Down, 0, sizeof(ST_Key_Down));
    }
}

void THD6301::Vbl()
{ // this is called in run.cpp right after IKBD_VBL()
    hd6301_vbl_cycles = 0;
    click_x = click_y = 0;

    // RDH: MouseVblDeltaX and MouseVblDeltaY should be set to show the mouse
    // movement between each call to this function
}

void THD6301::ResetProgram() {
    TRACE_LOG("6301 Reset ST program\n");
    LastCommand = CurrentCommand = 0xFF;
    CurrentParameter = 0;
    nParameters = 0;
}

