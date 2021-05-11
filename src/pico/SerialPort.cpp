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
#include "SerialPort.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "config.h"

#define UART_ID uart1
// The HD6301 in the ST communicates at 7812 baud
#define BAUD_RATE 7812
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We flag the buffer as 'empty' if it has less than this amount of bytes queued. This
// provides a small queue that optimises the serial port performance. Important for
// smooth mouse handling.
#define BUFFER_SIZE 8

SerialPort::~SerialPort() {
    close();
}

SerialPort& SerialPort::instance() {
    static SerialPort serial;
    return serial;
}

void SerialPort::open() {
    uart_init(UART_DEVICE, 2400);
    gpio_set_function(UART_TX, GPIO_FUNC_UART);
    gpio_set_function(UART_RX, GPIO_FUNC_UART);
    int actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // No hardware flow control
    uart_set_hw_flow(UART_ID, false, false);

    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // We don't want to use the FIFO otherwise we get mouse lag as the serial comms drains.
    uart_set_fifo_enabled(UART_ID, false);
}

void SerialPort::set_ui(UserInterface& ui) {
    this->ui = &ui;
}

void SerialPort::close() {
}

void SerialPort::send(const unsigned char data) {
    uart_putc(UART_ID, data);
    if (ui) {
        ui->serial(true, data);
    }
}

bool SerialPort::recv(unsigned char& data) const {
    if (uart_is_readable(UART_ID)) {
        data = uart_getc(UART_ID);
        if (ui) {
            ui->serial(false, data);
        }
        return true;
    }
    return false;
}

void SerialPort::configure() {
}

bool SerialPort::send_buf_empty() const {
    return uart_is_writable(UART_ID);
}

void serial_send(unsigned char data) {
    SerialPort::instance().send(data);
}
