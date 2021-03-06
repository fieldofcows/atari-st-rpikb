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

#ifdef __cplusplus 
#include <stdexcept>
#include "UserInterface.h"

class SerialPortException: public std::runtime_error {
public:
    SerialPortException(const std::string& what) : std::runtime_error(what) { }
};

class SerialPort {
private:
    SerialPort() = default;

public:
    ~SerialPort();

    static SerialPort& instance();

    /**
     * Open the serial port.
     * Throws an exception if the port cannot be opened.
     */
    void open();

    /**
     * Set UI for logging
     */
    void set_ui(UserInterface& ui);

    /**
     * Close the serial port if it was previously opened
     */
    void close();

    /**
     * Send data to the Atari ST over the serial port
     */
    void send(const unsigned char data);

    /**
     * Attempt to receive data from the Atari ST over the serial port.
     * This function does not block.
     * Returns true if a byte was received, otherwise returns false
     */
    bool recv(unsigned char& data) const;

    /**
     * Query whether the transmit buffer is empty.
     * This can be used to synchronise a sender with the serial transmit rate. Although
     * this class can handle an arbitrarily sized tx buffer we might want to ensure
     * data is queued no fast than it can be sent.
     */
    bool send_buf_empty() const;

private:
    void configure();
private:
    UserInterface*              ui = nullptr;
};

extern "C" {
#endif

void serial_send(unsigned char data);

#ifdef __cplusplus
}
#endif
