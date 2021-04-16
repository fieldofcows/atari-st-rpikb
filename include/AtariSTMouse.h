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
#include <stdint.h>

#pragma once

#ifdef __cplusplus 

class AtariSTMouse {
private:
    AtariSTMouse() = default;

public:
    static AtariSTMouse& instance();

    /**
     * Set the speed of the mouse. The value is transformed into a frequency at which the
     * mouse quadrature encoder value rotates. The units are fairly arbritrary but range from
     * 0 for stationary up to a maximum of about +/- 50. Make sure the USB mouse routine scales the
     * values accordingly.
     */
    void set_speed(int64_t cpu_cycles, int x, int y);

    const int get_x_cycles() const;
    const int get_y_cycles() const;

    const int64_t get_next_x_cycle() const;
    void set_next_x_cycle(int64_t val);
    const int64_t get_next_y_cycle() const;
    void set_next_y_cycle(int64_t val);

private:
    void set_speed_internal(int64_t cpu_cycles, int speed, int& cycles, int64_t& next_cycle);

private:
    // Number of HD6301 clock cycles between each rotation of the mouse state.
    // The sign is used to indicate the direction.
    int x_cycles = 0;
    int y_cycles = 0;

    // When this CPU clock cycle is reached, the corresponding mouse state register
    // is rotated.
    int64_t next_x_cycle = 0;
    int64_t next_y_cycle = 0;
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