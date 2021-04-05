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
    hd6301_destroy(); // calling the C 6301 function
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
    hd6301_reset(Cold);
    if (Cold) // real cold
    {
        memset(ST_Key_Down, 0, sizeof(ST_Key_Down));
    }
}

void THD6301::Vbl()
{ // this is called in run.cpp right after IKBD_VBL()
    click_x = click_y = 0;

    // RDH: MouseVblDeltaX and MouseVblDeltaY should be set to show the mouse
    // movement between each call to this function
}

