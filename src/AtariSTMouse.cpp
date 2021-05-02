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
#include <stdio.h>

#define MOUSE_MASK 0x33333333

int tick_count = 0;
int toggle_count = 0;

AtariSTMouse& AtariSTMouse::instance() {
    static AtariSTMouse mouse;
    return mouse;
}

AtariSTMouse::AtariSTMouse() {
    gpio_init(14);
    gpio_init(15);
    gpio_set_dir(14, GPIO_OUT);
    gpio_set_dir(15, GPIO_OUT);
    gpio_set_pulls(14, true, true);
    gpio_set_pulls(15, true, true);

    x_reg = MOUSE_MASK;
    y_reg = MOUSE_MASK;
    
    // internal mouse state is random: A239/Jumping Jackson, Droid SE WIP
    x_reg = _rotl(x_reg, rand() % 16);
    y_reg = _rotl(x_reg, rand() % 16);

    last_x_us = last_y_us = get_absolute_time();
}

void AtariSTMouse::update() {
    // Get the current time to see if we need to update the next cycle.
    absolute_time_t tm = get_absolute_time();
    // If either axis is currently moving then see if we've exceeded the next rotation time
    if (x_period_us != 0) {
        // Find out what time we should rotate at
        absolute_time_t cycle_time = delayed_by_us(last_x_us, abs(x_period_us));
        if (absolute_time_diff_us(tm, cycle_time) < 0) {
            // Time to cycle
            last_x_us = tm;
            x_reg = (x_period_us > 0) ? _rotr(x_reg, 1) : _rotl(x_reg, 1);
            gpio_put(14, x_reg & 1);
            gpio_put(15, (x_reg & 2) ? 1 : 0);
            ++toggle_count;
        }
    }
    if (y_period_us != 0) {
        // Find out what time we should rotate at
        absolute_time_t cycle_time = delayed_by_us(last_y_us, abs(y_period_us));
        if (absolute_time_diff_us(tm, cycle_time) < 0) {
            // Time to cycle
            last_y_us = tm;
            y_reg = (y_period_us > 0) ? _rotr(y_reg, 1) : _rotl(y_reg, 1);
        }
    }
}

void AtariSTMouse::set_speed(int x, int y) {
    set_speed_internal(x, x_period_us);
    set_speed_internal(y, y_period_us);
}

void AtariSTMouse::set_speed_internal(int speed, int& period) {
    if (speed == 0) {
        period = 0;
    }
    else {
        #define MAX_SPEED 50000.0
        #define MIN_SPEED 500
        period = (int)((MAX_SPEED / speed) * 1.0);
        if ((speed > 0) && (period < MIN_SPEED)) {
            period = MIN_SPEED;
        }
        else if ((speed < 0) && (period > -MIN_SPEED)) {
            period = -MIN_SPEED;
        }
    }
}

const int AtariSTMouse::get_x_reg() const {
    return x_reg;
}

const int AtariSTMouse::get_y_reg() const{
    return y_reg;
}

void mouse_tick(int64_t cpu_cycles, int* x_counter, int* y_counter) {
    *x_counter = AtariSTMouse::instance().get_x_reg();
    *y_counter = AtariSTMouse::instance().get_y_reg();
}
