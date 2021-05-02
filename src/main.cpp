/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "6301.h"
#include "cpu.h"
#include "util.h"
#include "bsp/board.h"
#include "tusb.h"
#include "HidInput.h"
#include "SerialPort.h"
#include "AtariSTMouse.h"

#define ROMBASE     256
#define CYCLES_PER_LOOP 1000

extern unsigned char rom_HD6301V1ST_img[];
extern unsigned int rom_HD6301V1ST_img_len;

/**
 * Read a byte from the physical serial port and pass
 * it to the HD6301
 */
static void handle_rx_from_st() {
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
    memcpy(pram + ROMBASE, rom_HD6301V1ST_img, rom_HD6301V1ST_img_len);
}

void core1_entry() {
    // Initialise the HD6301
    setup_hd6301();
    hd6301_reset(1);

    unsigned long count = 0;
    absolute_time_t tm = get_absolute_time();
    while (true) {
        count += CYCLES_PER_LOOP;
        // Update the tx serial port status based on our serial port handler
        hd6301_tx_empty(1);

        hd6301_run_clocks(CYCLES_PER_LOOP);

        if ((count % 1000000) == 0) {
            //printf("Cycles = %lu\n", count);
            printf("CPU cycles = %llu\n", cpu.ncycles);
        }

        tm = delayed_by_us(tm, CYCLES_PER_LOOP);
        sleep_until(tm);
    }
}

int main() {
    //stdio_init_all();
    board_init();
    tusb_init();

    // Setup the UART and HID instance.
    SerialPort::instance().open();
    HidInput::instance().reset();

    // The second CPU core is dedicated to the HD6301 emulation.
    multicore_launch_core1(core1_entry);

    absolute_time_t ten_ms = get_absolute_time();
    while (true) {
        absolute_time_t tm = get_absolute_time();

        AtariSTMouse::instance().update();

        // 10ms handler
        if (absolute_time_diff_us(ten_ms, tm) >= 10000) {
            ten_ms = tm;

            tuh_task();
            HidInput::instance().handle_keyboard();
            handle_rx_from_st();
            HidInput::instance().handle_mouse(cpu.ncycles);
            HidInput::instance().handle_joystick();
        }
        else {
            //HidInput::instance().handle_mouse(0);
        }
    }
    return 0;
}
