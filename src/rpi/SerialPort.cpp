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
#include <fcntl.h>
#include <asm-generic/ioctls.h>
#include <asm-generic/termbits.h>
#include <sys/ioctl.h>
#include <unistd.h>

// We flag the buffer as 'empty' if it has less than this amount of bytes queued. This
// provides a small queue that optimises the serial port performance. Important for
// smooth mouse handling.
#define BUFFER_SIZE 8

extern "C" {
    int tcdrain(int filedes);
}

// The HD6301 in the ST communicates at 7812 baud. For some reason, configuring the RPi to use
// 8000 baud results in the correct baud rate of 7812 (measured on oscilloscope). It would be
// interesting to determine why.
#define CUSTOM_BAUD 8000

SerialPort::~SerialPort() {
    close();
}

SerialPort& SerialPort::instance() {
    static SerialPort serial;
    return serial;
}

void SerialPort::handle_send() {
    while (thread_run) {
        // Make sure the TX buf is empty before locking the mutex
        tcdrain(handle);
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock, [this]{ return !thread_run || !tx_buf.empty(); });
        // Dequeue a single byte. We will allow another byte to be queued
        // whilst we block sending this one
        while (!tx_buf.empty()) {
            unsigned char data = tx_buf.front();
            tx_buf.pop();
            // Unlock so more data can be queued whilst we are sending
            lock.unlock();

            write(handle, &data, 1);
            lock.lock();
        }

        // No more data if here. Mutex is locked
        lock.unlock();
    }
}

void SerialPort::open() {
    if (handle == -1) {
        handle = ::open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (handle == -1) {
            throw new SerialPortException("Could not open /dev/ttyAMA0");
        }
        // Setup the port parameters
        configure();
        // Start the thread that is used to send data over the serial port
        thread_run = true;
        serial_thread = std::thread(&SerialPort::handle_send, this);
    }
}

void SerialPort::close() {
    if (handle != -1) {
        thread_run = false;
        cond.notify_one();
        serial_thread.join();
        ::close(handle);
        handle = -1;
    }
}

void SerialPort::send(const unsigned char data) {
    std::lock_guard<std::mutex> lock(mutex);
    tx_buf.push(data);
    cond.notify_one();
}

bool SerialPort::recv(unsigned char& data) const {
    int count = read(handle, &data, 1);
    return count > 0;
}

void SerialPort::configure() {
    struct termios2 tio;
    if (ioctl(handle, TCGETS2, &tio) == -1) {
        throw new SerialPortException("Could not read serial port configuration");
    }

    tio.c_cflag = BOTHER | CS8 | CLOCAL | CREAD;
    tio.c_iflag = IGNPAR;
    tio.c_oflag = 0;
    tio.c_lflag = 0;
    tio.c_ispeed = CUSTOM_BAUD;
    tio.c_ospeed = CUSTOM_BAUD;

    if (ioctl(handle, TCSETS2, &tio) == -1) {
        throw new SerialPortException("Could not write serial port configuration");
    }
}

bool SerialPort::send_buf_empty() const {
    std::lock_guard<std::mutex> lock(mutex);
    return tx_buf.size() < BUFFER_SIZE;
}

void serial_send(unsigned char data) {
    SerialPort::instance().send(data);
}
