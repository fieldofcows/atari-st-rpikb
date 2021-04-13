#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "st_key_lookup.h"
#include "6301.h"
#include "THD6301.h"
#include "options.h"
#include "cpu.h"
#include "util.h"
#include "AtariSTMouse.h"
#include "SerialPort.h"
#include "HidInput.h"

THD6301 Ikbd;
BYTE stick[8];
extern "C"
{
    RPIConfig IKBDConfig;
}

void handle_send_to_st() {
    if (hd6301_check_for_tx_byte()) {
        std::vector<unsigned char> data;
        data.push_back(hd6301_read_tx_byte());
        //printf("6301 -> ST %X\n", bt);
        SerialPort::instance().send(data);
    }
}

void handle_rx_from_st() {
    std::vector<unsigned char> data;
    SerialPort::instance().recv(data);
    for (auto const& bt: data) {
        printf("ST -> 6301 %X\n", bt);
        Ikbd.rdrs = bt;
        hd6301_receive_byte(bt);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: rpi6301 kb_device mouse_device\n");
        return -1;
    }

    // Open the keyboard, mouse and serial port
    SerialPort::instance().open();
    HidInput::instance().open(argv[1], argv[2]);

    // Initialise the keyboard controller
    Ikbd.Init();
    Ikbd.ResetChip(1);

    // Keep track of time so we can get a fairly accurate clock cycle
    timespec last_time;
    timespec ms_time;
    unsigned long ms_count = 0;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &last_time);
    ms_time.tv_nsec = 0;

    int count = 0;

    while (1) {
        // Calculate the time the last cycle took
        timespec cur_time;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cur_time);
        timespec diff_time = diff(last_time, cur_time);
        last_time = cur_time;
        ms_time.tv_nsec += diff_time.tv_nsec;

        // We execute 1000 cycles every millisecond
        if (ms_time.tv_nsec > 1000 * 1000) {
            ++count;
            hd6301_run_clocks(1000);
            handle_send_to_st();
            handle_rx_from_st();
            HidInput::instance().handle_keyboard();
            ms_time.tv_nsec = 0;
            ++ms_count;

            // 10ms handler
            if ((ms_count % 10) == 0) {
                HidInput::instance().handle_mouse(cpu.ncycles);
            }
        }
    }
}
