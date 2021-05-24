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
#pragma once

// GPIO assignments for display
#define SSD1306_SDA         8
#define SSD1306_SCL         9
#define SSD1306_I2C         i2c0
#define SSD1306_ADDR        0x3c
#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      64

// GPIO assignments for UI buttons
#define GPIO_BUTTON_LEFT    18
#define GPIO_BUTTON_MIDDLE  17
#define GPIO_BUTTON_RIGHT   16

// GPIO assignments for serial connection to Atari ST
#define UART_TX             4
#define UART_RX             5
#define UART_DEVICE         uart1

// Joystick 1
#define JOY1_UP             10
#define JOY1_DOWN           11
#define JOY1_LEFT           12
#define JOY1_RIGHT          13
#define JOY1_FIRE           14

// Joystick 0
#define JOY0_UP             19
#define JOY0_DOWN           20
#define JOY0_LEFT           21
#define JOY0_RIGHT          22
#define JOY0_FIRE           26
