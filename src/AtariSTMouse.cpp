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
#include "AtariSTMouse.h"
#include <math.h>
#include "util.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

AtariSTMouse& AtariSTMouse::instance() {
    static AtariSTMouse mouse;
    return mouse;
}

void AtariSTMouse::set_speed(int64_t cpu_cycles, int x, int y) {
    set_speed_internal(cpu_cycles, x, x_cycles, next_x_cycle);
    set_speed_internal(cpu_cycles, y, y_cycles, next_y_cycle);
}

void AtariSTMouse::set_speed_internal(int64_t cpu_cycles, int speed, int& cycles, int64_t& next_cycle) {
    cycles = 0;
    next_cycle = 0;
    if (speed != 0) {
        double freq = MIN((abs(speed) - 1) * 15.0 + 100.0, 1500.0);
        freq = (speed > 0) ? freq : -freq;
        cycles = (1000.0 * 1000.0) / freq;
        next_cycle = cpu_cycles + abs(cycles);
    }
}

const int AtariSTMouse::get_x_cycles() const {
    return x_cycles;
}

const int AtariSTMouse::get_y_cycles() const {
    return y_cycles;
}

const int64_t AtariSTMouse::get_next_x_cycle() const {
    return next_x_cycle;
}

void AtariSTMouse::set_next_x_cycle(int64_t val) {
    next_x_cycle = val;
}

const int64_t AtariSTMouse::get_next_y_cycle() const {
    return next_y_cycle;
}

void AtariSTMouse::set_next_y_cycle(int64_t val) {
    next_y_cycle = val;
}

void mouse_tick(int64_t cpu_cycles, int* x_counter, int* y_counter) {
    // Get the current direction and speed
    AtariSTMouse& ms = AtariSTMouse::instance();
    const int x_cycles = ms.get_x_cycles();
    const int y_cycles = ms.get_y_cycles();

    // See if the CPU clock cycle has exceeded the next cycle counts
    int64_t next_x_cycle = ms.get_next_x_cycle();
    int64_t next_y_cycle = ms.get_next_y_cycle();

    if ((x_cycles != 0) && (cpu_cycles >= next_x_cycle)) {
        *x_counter = (x_cycles > 0) ? _rotr(*x_counter, 1) : _rotl(*x_counter, 1);
        next_x_cycle += abs(x_cycles);
    }
    if ((y_cycles != 0) && (cpu_cycles >= next_y_cycle))
    {
        *y_counter = (y_cycles > 0) ? _rotr(*y_counter, 1) : _rotl(*y_counter, 1);
        next_y_cycle += abs(y_cycles);
    }
    
    // Update the next target clock cycles
    ms.set_next_x_cycle(next_x_cycle);
    ms.set_next_y_cycle(next_y_cycle);
}
