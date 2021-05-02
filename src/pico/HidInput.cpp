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
#include "st_key_lookup.h"
#include "AtariSTMouse.h"
#include "bsp/board.h"
#include "tusb.h"

// Mouse toggle key is set to Scroll Lock
#define TOGGLE_MOUSE_MODE 70

static int kbdev = 0;
static int mousedev = 0;

extern "C" {


CFG_TUSB_MEM_SECTION static hid_mouse_report_t mouse_report;
CFG_TUSB_MEM_SECTION static hid_keyboard_report_t keyboard_report;

void tuh_hid_keyboard_mounted_cb(uint8_t dev_addr) {
    printf("A Keyboard device (address %d) is mounted\r\n", dev_addr);
    kbdev = dev_addr;
    tuh_hid_keyboard_get_report(dev_addr, &keyboard_report);
}

void tuh_hid_keyboard_unmounted_cb(uint8_t dev_addr) {
    printf("A Keyboard device (address %d) is unmounted\r\n", dev_addr);
    kbdev = 0;
}

// invoked ISR context
void tuh_hid_keyboard_isr(uint8_t dev_addr, xfer_result_t event) {
    (void) dev_addr;
    (void) event;
}

void tuh_hid_mouse_mounted_cb(uint8_t dev_addr) {
    printf("A Mouse device (address %d) is mounted\r\n", dev_addr);
    mousedev = dev_addr;
}

void tuh_hid_mouse_unmounted_cb(uint8_t dev_addr) {
    printf("A Mouse device (address %d) is unmounted\r\n", dev_addr);
    mousedev = 0;
}

// invoked ISR context
void tuh_hid_mouse_isr(uint8_t dev_addr, xfer_result_t event) {
    (void) dev_addr;
    (void) event;
}

}

HidInput::HidInput() {
    key_states.resize(128);
    std::fill(key_states.begin(), key_states.end(), 0);
}

HidInput& HidInput::instance() {
    static HidInput hid;
    return hid;
}

void HidInput::open(const std::string& kbdev, const std::string& mousedev, const std::string joystickdev) {
}

void HidInput::handle_keyboard() {
    if (kbdev && tuh_hid_keyboard_is_mounted(kbdev)) {
        if (!tuh_hid_keyboard_is_busy(kbdev)) {
            /*
                            bool const is_shift =
                        p_new_report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT);
                uint8_t ch = keycode2ascii[p_new_report->keycode[i]][is_shift ? 1 : 0];
            */

            // Translate the USB HID codes into ST keys that are currently down
            char st_keys[6];
            for (int i = 0; i < 6; ++i) {
                if ((keyboard_report.keycode[i] > 0) && (keyboard_report.keycode[i] < 128)) {
                    st_keys[i] = st_key_lookup_hid_gb[keyboard_report.keycode[i]];
                }
                else {
                    st_keys[i] = 0;
                }
            }
            // Go through all ST keys and update their state
            for (int i = 1; i < key_states.size(); ++i) {
                bool down = false;
                for (int j = 0; j < 6; ++j) {
                    if (st_keys[j] == i) {
                        down = true;
                        break;
                    }
                }
                key_states[i] = down ? 1 : 0;
            }

            // Trigger the next report
            tuh_hid_keyboard_get_report(kbdev, &keyboard_report);
        }
    }
}

void HidInput::handle_mouse(const int64_t cpu_cycles) {
    if (mousedev && tuh_hid_mouse_is_mounted(mousedev)) {
        if (!tuh_hid_mouse_is_busy(mousedev)) {
            /*
            for (int i = 0; i < sizeof(mouse_report); ++i) {
                printf("%02X ", ((unsigned char*)&mouse_report)[i]);
            }
            printf("\n");
            */
            // Update button state
            mouse_state = (mouse_state & 0xfd) | ((mouse_report.buttons & MOUSE_BUTTON_LEFT) ? 2 : 0);
            mouse_state = (mouse_state & 0xfe) | ((mouse_report.buttons & MOUSE_BUTTON_RIGHT) ? 1 : 0);
            //printf("mouse_state = %d, buttons = %d\n", mouse_state, mouse_report.buttons);

            // Mouse vectors
            val_x = mouse_report.x;
            val_y = mouse_report.y;

            // Some mice don't handle the -127 to 127 limit very will when using the boot protocol.
            // If we see a rapid transition in direction then assume a variable wrap around.
            if ((val_x < 0) && (last_x > 45)) {
                val_x = 127;
            }
            else if ((val_x > 0) && (last_x < -45)) {
                val_x = -127;
            }
            if ((val_y < 0) && (last_y > 45)) {
                val_y = 127;
            }
            else if ((val_y > 0) && (last_y < -45)) {
                val_y = -127;
            }
            last_x = val_x;
            last_y = val_y;

            // Trigger the next report
            tuh_hid_mouse_get_report(mousedev, &mouse_report);
        }
    }
    if (cpu_cycles != 0) {
        if ((val_x != 0) || (val_y != 0)) {
            //printf("%d %d\n", val_x, val_y);
        }
        AtariSTMouse::instance().set_speed(val_x, val_y);
        val_x = 0;
        val_y = 0;
    }
}

void HidInput::handle_joystick() {
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
