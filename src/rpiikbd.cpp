/*
 * Atari ST Raspberry Pi IKDB Emulator
 * Copyright (C) 2021 Roy Hopkins
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "st_key_lookup.h"
#include "6301.h"
#include "cpu.h"
#include "util.h"
#include "AtariSTMouse.h"
#include "SerialPort.h"
#include "HidInput.h"

#define ROMBASE     256
#define ROMLEN      4096

/**
 * Read a byte from the physical serial port and pass
 * it to the HD6301
 */
void handle_rx_from_st() {
    if (!hd6301_sci_busy()) {
        unsigned char data;
        if (SerialPort::instance().recv(data)) {
            //printf("ST -> 6301 %X\n", data);
            hd6301_receive_byte(data);
        }
    }
}

/**
 * Prepare the HD6301 and load the ROM file
 */
void setup_hd6301() {
    BYTE* pram = hd6301_init();
    if (!pram) {
        printf("Failed to initialise HD6301\n");
        exit(-1);
    }
    FILE* fp = fopen("HD6301V1ST.img", "r+b");
    if (!fp) {
        printf("Could not open ROM file\n");
        exit(-1);
    }
    if (fread(pram + ROMBASE, 1, ROMLEN, fp) != ROMLEN) {
        printf("Failed to read ROM file\n");
        exit(-1);
    }
    fclose(fp);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: rpi6301 kb_device mouse_device [joystick_device] \n");
        return -1;
    }

    // Open the keyboard, mouse and serial port
    SerialPort::instance().open();
    HidInput::instance().open(argv[1], argv[2], (argc < 4) ? "" : argv[3]);

    // Initialise the HD6301
    setup_hd6301();
    hd6301_reset(1);
    HidInput::instance().reset();

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

            // Update the tx serial port status based on our serial port handler
            hd6301_tx_empty(SerialPort::instance().send_buf_empty() ? 1 : 0);

            HidInput::instance().handle_keyboard();
            ms_time.tv_nsec = 0;
            ++ms_count;

            // 10ms handler
            if ((ms_count % 10) == 0) {
                handle_rx_from_st();
                HidInput::instance().handle_mouse(cpu.ncycles);
                HidInput::instance().handle_joystick();
            }
        }
    }
}
