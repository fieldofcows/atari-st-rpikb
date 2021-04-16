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
#include "HidInput.h"
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include "st_key_lookup.h"
#include "AtariSTMouse.h"

// Mouse toggle key is set to Scroll Lock
#define TOGGLE_MOUSE_MODE 70

HidInput::HidInput() {
    key_states.resize(128);
    std::fill(key_states.begin(), key_states.end(), 0);
}

HidInput& HidInput::instance() {
    static HidInput hid;
    return hid;
}

void HidInput::open(const std::string& kbdev, const std::string& mousedev, const std::string joystickdev) {
    keyboard_handle = ::open(kbdev.c_str(), O_RDONLY | O_NONBLOCK);
    mouse_handle = ::open(mousedev.c_str(), O_RDONLY | O_NONBLOCK);
    if (!joystickdev.empty()) {
        joystick_handle = ::open(joystickdev.c_str(), O_RDONLY | O_NONBLOCK);
        if (joystick_handle == -1) {
            throw new HidInputException("Could not open the joystick device");
        }
        if (ioctl(joystick_handle, EVIOCGRAB, 1) != 0) {
            throw new HidInputException("Could not get exclusive use of the joystick device");
        }
    }

    if (keyboard_handle == -1) {
        throw new HidInputException("Could not open the keyboard device");
    }
    if (mouse_handle == -1) {
        throw new HidInputException("Could not open the mouse device");
    }
    if (ioctl(keyboard_handle, EVIOCGRAB, 1) != 0) {
        throw new HidInputException("Could not get exclusive use of the keyboard device");
    }
    if (ioctl(mouse_handle, EVIOCGRAB, 1) != 0) {
        throw new HidInputException("Could not get exclusive use of the mouse device");
    }
}

void HidInput::handle_keyboard() {
    struct input_event evts[64];
    // Read any pending events
    int count = read(keyboard_handle, evts, sizeof(evts));
    if (count != -1) {
        count /= sizeof(struct input_event);
        for (int i = 0; i < count; ++i) {
            // Look for a keyboard event
            if ((evts[i].type == EV_KEY) && (evts[i].code < 128)) {
                if (evts[i].value && (evts[i].code == TOGGLE_MOUSE_MODE)) {
                    mouse_en = !mouse_en;
                    printf("Mouse %s, Joystick 0 %s\n", 
                            mouse_en ? "active" : "inactive", mouse_en ? "inactive" : " active");
                }
                else {
                    unsigned char st_key = st_key_lookup_gb[evts[i].code];
                    if (st_key & 0x7f) {
                        key_states[st_key] = evts[i].value;
                    }
                }
            }
        }
    }
}

void HidInput::handle_mouse(const int64_t cpu_cycles) {
    int num_events = 0;
    static int max_events = 0;
    struct input_event evts[100];

    int val_x = 0;
    int val_y = 0;

    // Read any pending events
    int count = read(mouse_handle, evts, sizeof(evts));
    if (count != -1) {
        count /= sizeof(struct input_event);
        for (int i = 0; i < count; ++i) {
            // An EV_KEY event for a mouse represents the mouse buttons
            if (evts[i].type == EV_KEY) {
                if (evts[i].code == BTN_LEFT)
                    mouse_state = (mouse_state & 0xfd) | (evts[i].value ? 2 : 0);
                if (evts[i].code == BTN_RIGHT)
                    mouse_state = (mouse_state & 0xfe) | (evts[i].value ? 1 : 0);
            }
            // Check for mouse movement
            if (evts[i].type == EV_REL) {
                if (evts[i].code == 0) {
                    val_x += evts[i].value;
                }
                else if (evts[i].code == 1) {
                    val_y += evts[i].value;
                }
            }
        }
    }
    AtariSTMouse::instance().set_speed(cpu_cycles, val_x, val_y);
}

void HidInput::handle_joystick() {
    int joystick = 1;

    if (joystick_handle != -1) {
        struct input_event evts[64];
        // Read any pending events
        int count = read(joystick_handle, evts, sizeof(evts));
        if (count != -1) {
            count /= sizeof(struct input_event);
            for (int i = 0; i < count; ++i) {
                // Axis switches
                if (evts[i].type == EV_ABS) {
                    int bit;
                    if (evts[i].code == ABS_Y) {
                        bit = 0;
                    } else {
                        bit = 2;
                    }
                    if (joystick == 1) {
                        bit += 4;
                    }
                    // Up and left have a value < 0x80 (0 for digital)
                    // Down and right have a value > 0x80 (0xff for digital)
                    joystick_state &= ~(0x3 << bit);
                    if (evts[i].value < 0x80) {
                        joystick_state |= 1 << bit;
                    }
                    else if (evts[i].value > 0x80) {
                        joystick_state |= 1 << (bit + 1);
                    }
                }
                // Buttons
                else if (evts[i].type == EV_KEY) {
                    // The joystick buttons share the mouse button state
                    if ((evts[i].code == BTN_TRIGGER) || (evts[i].code == BTN_THUMB) ||
                        (evts[i].code == BTN_THUMB2) || (evts[i].code == BTN_TOP)) {
                        if (joystick == 0)
                            mouse_state = (mouse_state & 0xfd) | (evts[i].value ? 2 : 0);
                        else
                            mouse_state = (mouse_state & 0xfe) | (evts[i].value ? 1 : 0);
                    }
                }
            }
        }
    }
}


void HidInput::reset() {
     std::fill(key_states.begin(), key_states.end(), 0);   
}

unsigned char HidInput::keydown(const unsigned char code) const {
    if (code < 128) {
        return key_states[code];
    }
    return 0;
}

int HidInput::mouse_buttons() const {
    return mouse_state;
}

unsigned char HidInput::joystick() const {
    return joystick_state;
}

bool HidInput::mouse_enabled() const {
    return mouse_en;
}

unsigned char st_keydown(const unsigned char code){
    return HidInput::instance().keydown(code);
}

int st_mouse_buttons() {
    return HidInput::instance().mouse_buttons();
}

unsigned char st_joystick() {
    return HidInput::instance().joystick();
}

int st_mouse_enabled() {
    return HidInput::instance().mouse_enabled() ? 1 : 0;
}
