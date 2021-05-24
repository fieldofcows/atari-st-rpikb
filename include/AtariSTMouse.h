/*
 * Atari ST RP2040 IKBD Emulator
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
#include <stdint.h>
#include "pico/stdlib.h"

#pragma once

#ifdef __cplusplus 

class AtariSTMouse {
private:
    AtariSTMouse();

public:
    static AtariSTMouse& instance();

public:

    /**
     * Set the speed of the mouse. The value is transformed into a frequency at which the
     * mouse quadrature encoder value rotates. The units are fairly arbritrary but range from
     * 0 for stationary up to a maximum of about +/- 50. Make sure the USB mouse routine scales the
     * values accordingly.
     */
    void set_speed(int x, int y);

    /**
     * Periodic update function. Call as fast as possible.
     */
    void update();

    const int get_x_reg() const;
    const int get_y_reg() const;

private:
    void set_speed_internal(int speed, int& period);

private:
    // Time in microseconds between each rotation of the mouse state.
    // The sign is used to indicate the direction.
    int x_period_us = 0;
    int y_period_us = 0;

    // The last time each register was rotated
    absolute_time_t last_x_us;
    absolute_time_t last_y_us;

    // The mouse registers
    unsigned int x_reg;
    unsigned int y_reg;
};

extern "C" {
#endif

/**
 * This needs to be called by the code that reads the hardware register
 * in ireg.c. It rotates the x and y mouse quadrature encoder simulators
 * to simulate mouse movement
 */
void mouse_tick(int64_t cpu_cycles, int* x_counter, int* y_counter);

#ifdef __cplusplus 
}
#endif