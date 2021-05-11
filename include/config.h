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
#pragma once

// GPIO assignments for display
#define SSD1306_SDA         8
#define SSD1306_SCL         9
#define SSD1306_I2C         i2c0
#define SSD1306_ADDR        0x3c
#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      64

// GPIO assignments for UI buttons
#define GPIO_BUTTON_LEFT    20
#define GPIO_BUTTON_MIDDLE  19
#define GPIO_BUTTON_RIGHT   18

// GPIO assignments for serial connection to Atari ST
#define UART_TX             4
#define UART_RX             5
#define UART_DEVICE         uart1