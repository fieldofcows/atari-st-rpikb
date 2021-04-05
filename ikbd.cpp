#include "6301/6301.h"
#include "ikbd.h"
#include "options.h"
#include <string.h>
#include <iostream>
#include <queue>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>

#define TRACE_LOG printf

THD6301 Ikbd;
BYTE ST_Key_Down[128];
int mousek;
BYTE stick[8];
extern "C" { RPIConfig IKBDConfig; }

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