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

#ifdef __cplusplus
#include <stdexcept>
#include <vector>
#include "UserInterface.h"

class HidInputException: public std::runtime_error {
public:
    HidInputException(const std::string& what) : std::runtime_error(what) { }
};

class HidInput {
private:
    HidInput();

public:
    static HidInput& instance();

    void set_ui(UserInterface& ui);

    /**
     * Open the input devices
     * Throws an exception if the keyboard or mouse cannot be opened.
     */
    void open(const std::string& kbdev, const std::string& mousedev, const std::string joystickdev = "");

    void handle_keyboard();
    void handle_mouse(const int64_t cpu_cycles);
    void handle_joystick();

    void reset();

    unsigned char keydown(const unsigned char code) const;
    int mouse_buttons() const; 
    unsigned char joystick() const;
    bool mouse_enabled() const;

private:
    int keyboard_handle = -1;
    int mouse_handle = -1;
    int joystick_handle = -1;
    std::vector<unsigned char> key_states;
    int mouse_state = 0;
    unsigned char joystick_state = 0;
    bool mouse_en = true;
};

extern "C" {
#endif

/**
 * Return the key states of the given Atari ST keyboard scancode. The scancode
 * must be in the range 0-127
 */
unsigned char st_keydown(const unsigned char code);

/**
 * Return the current state of the mouse buttons
 */
int st_mouse_buttons();

/**
 * Return a bitfield representing the joystick state
 */
unsigned char st_joystick();

/**
 * Return 1 if the mouse is enabled or 0 if they joystick is enabled
 */
int st_mouse_enabled();

#ifdef __cplusplus
}
#endif
