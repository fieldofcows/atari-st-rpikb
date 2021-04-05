extern "C"
{
#include "6301/6301.h"
}

#include "acia.h"
#include "ikbd.h"
#include <string.h>
#include <iostream>
#include <queue>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>

#define TRACE_LOG printf

TMC6850 acia[2];
THD6301 Ikbd;
BYTE ST_Key_Down[128];
int mousek;
BYTE stick[8];
extern "C"
{
    RPIConfig SSEConfig;
}

bool keyhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}

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

//#pragma warning (default: 4701)

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

void keyboard_buffer_write(BYTE src)
{
    /*  Function keyboard_buffer_write() receive bytes from the fake or the true
    6301 emu, or from shortcuts or macros, and sets then in an agenda or sets up
    an event to take care of the 6301->ACIA delay.
*/
}

void handle_send_to_st() {
    if (hd6301_check_for_tx_byte()) {
        BYTE bt = hd6301_read_tx_byte();
        printf("6301 -> ST %X\n", bt);
    }

}

std::queue<BYTE> rx_from_st;

void handle_rx_from_st() {
    if (!rx_from_st.empty()) {
        BYTE bt = rx_from_st.front();
        printf("ST -> 6301 %X\n", bt);
        rx_from_st.pop();
        Ikbd.rdrs = bt;
        hd6301_receive_byte(bt);        
    }
}

int main()
{
    Ikbd.Init();
    Ikbd.ResetChip(1);

    // ST -> IKBD: send bytes to emulator via sci.c:sci_in()
    // IKBD -> ST: Keyboard scan codes queued in ST_Key_Down[]

    int state = 0;
    COUNTER_VAR cpu = 0;
    time_t t = time(NULL);

    while (1) {
        hd6301_run_cycles(cpu += HD6301_CYCLE_DIVISOR);
        handle_send_to_st();
        handle_rx_from_st();

        if ((state == 0) && (cpu > 100000 * HD6301_CYCLE_DIVISOR)) {
            state = 1;
            rx_from_st.push(0x80);
        }
        if ((state == 1) && (cpu > 200000 * HD6301_CYCLE_DIVISOR)) {
            state = 2;
            rx_from_st.push(0x01);
        }
        if (keyhit()) {
            printf("Char=%c\n", getchar());
            //if (getchar() == 'g') {
             //   ST_Key_Down[35] = 1;
            //}
            ST_Key_Down[35] = 1;
            t = time(NULL);
        }
        else if (ST_Key_Down[35] && ((time(NULL) - t) > 0)) {
            ST_Key_Down[35] = 0;
        }
    }



    return 0;
}