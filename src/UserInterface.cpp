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
#include "UserInterface.h"
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "config.h"

#define DEBOUNCE_COUNT 10

enum BUTTONS {
    BUTTON_LEFT,
    BUTTON_MIDDLE,
    BUTTON_RIGHT
};

UserInterface::UserInterface() {
}

ssd1306_t   disp;

void UserInterface::init() {
    // Setup the I2C interface to the display
    i2c_init(SSD1306_I2C, 400000);
    gpio_set_function(SSD1306_SDA, GPIO_FUNC_I2C);
    gpio_set_function(SSD1306_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SSD1306_SDA);
    gpio_pull_up(SSD1306_SCL);

    // Initialise the display library
    ssd1306_init(&disp, SSD1306_WIDTH, SSD1306_HEIGHT, SSD1306_ADDR, SSD1306_I2C);

    // Setup GPIO for buttons
    btn_gpio[0] = GPIO_BUTTON_LEFT;
    btn_gpio[1] = GPIO_BUTTON_MIDDLE;
    btn_gpio[2] = GPIO_BUTTON_RIGHT;
    for (int i = 0; i < 3; ++i) {
        gpio_init(btn_gpio[i]);
        gpio_set_dir(btn_gpio[i], GPIO_IN);
        gpio_pull_up(btn_gpio[i]);
    }

    // Read from NV storage
    int mouse_speed = settings.get_settings().mouse_speed;
    if (mouse_speed < MOUSE_MIN) {
        mouse_speed = MOUSE_MIN;
        settings.get_settings().mouse_speed = mouse_speed;
    }
    if (mouse_speed > MOUSE_MAX) {
        mouse_speed = MOUSE_MAX;
        settings.get_settings().mouse_speed = mouse_speed;
    }

    serial_tm = get_absolute_time();
}

void UserInterface::usb_connect_state(int kb, int mouse, int joy) {
    if ((num_kb != kb) || (num_mouse != mouse) || (num_joy != joy)) {
        dirty = true;
    }
    num_kb = kb;
    num_mouse = mouse;
    num_joy = joy;
}

int8_t UserInterface::get_mouse_speed() {
    return settings.get_settings().mouse_speed;
}

uint8_t UserInterface::get_joystick() {
    return settings.get_settings().joy_device;
}

uint8_t UserInterface::get_mouse_enabled() {
    return settings.get_settings().mouse_enabled;
}

void UserInterface::set_mouse_enabled(uint8_t en) {
    settings.get_settings().mouse_enabled = en;
    settings.write();
    dirty = true;
}


void UserInterface::update_serial() {
    uint8_t y = 0;
    ssd1306_clear(&disp);
    for (auto it : serial_lines) {
        ssd1306_draw_string(&disp, 0, y, 1, (char*)it.c_str());
        y += 9;
    }
    ssd1306_draw_string(&disp, 24, 27, 1, (char*)"ST <-> Kbd");
}

void UserInterface::update_status() {
    char buf[32];
    ssd1306_clear(&disp);
    sprintf(buf, "USB Keyboard  %d", num_kb);
    ssd1306_draw_string(&disp, 0, 0, 1,  buf);
    sprintf(buf, "USB Mouse     %d", num_mouse);
    ssd1306_draw_string(&disp, 0, 9, 1,  buf);
    sprintf(buf, "USB Joystick  %d", num_joy);
    ssd1306_draw_string(&disp, 0, 18, 1, buf);
    sprintf(buf, "%s", settings.get_settings().mouse_enabled ? "Mouse enabled" : "Joy 0 enabled");
    ssd1306_draw_string(&disp, 0, 27, 1, buf);
}

void UserInterface::update_mouse() {
    char buf[32];
    ssd1306_draw_string(&disp, 0, 45, 1, "Mouse speed");
    sprintf(buf, "----------------");
    buf[settings.get_settings().mouse_speed - MOUSE_MIN] = '*';
    ssd1306_draw_string(&disp, 0, 54, 1, buf);
}

void UserInterface::update_joy(int index) {
    char buf[32];
    sprintf(buf, "Joy %d: %s", index, (settings.get_settings().joy_device & (1 << index)) ? "DSub" : "USB");
    ssd1306_draw_string(&disp, 0, 54, 1, buf);
}

void UserInterface::handle_buttons() {
    for (int i = 0; i < 3; ++i) {
        bool state = gpio_get(btn_gpio[i]);
        if (!state) {
            // The <= means we go one past DEBOUNCE_COUNT and latch there until the button is released.
            if (btn_count[i] <= DEBOUNCE_COUNT) {
                if (++btn_count[i] == DEBOUNCE_COUNT) {
                    on_button_down(i);
                }
            }
        }
        else {
            btn_count[i] = 0;
        }
    }
}

void UserInterface::on_button_down(int i) {
    // Middle button changes page
    if (i == BUTTON_MIDDLE) {
        int pg = (int)page;
        pg = ((pg + 1) % (PAGE_SERIAL + 1));
        page = (PAGE)pg;
        dirty = true;
    }
    else if (i == BUTTON_LEFT) {
        if (page == PAGE_MOUSE) {
            if (settings.get_settings().mouse_speed > MOUSE_MIN) {
                --settings.get_settings().mouse_speed;
                settings.write();
                dirty = true;
            }
        }
        else if ((page == PAGE_JOY0) || (page == PAGE_JOY1)) {
            settings.get_settings().joy_device ^= (1 << (page - PAGE_JOY0));
            settings.write();
            dirty = true;
        }
    }
    else if (i == BUTTON_RIGHT) {
        if (page == PAGE_MOUSE) {
            if (settings.get_settings().mouse_speed < MOUSE_MAX) {
                ++settings.get_settings().mouse_speed;
                settings.write();
                dirty = true;
            }
        }
        else if ((page == PAGE_JOY0) || (page == PAGE_JOY1)) {
            settings.get_settings().joy_device ^= (1 << (page - PAGE_JOY0));
            settings.write();
            dirty = true;
        }
    }
}

void UserInterface::update() {
    handle_buttons();

    if (dirty) {
        dirty = false;

        if (page == PAGE_MOUSE) {
            update_status();
            update_mouse();
        }
        else if (page == PAGE_JOY0) {
            update_status();
            update_joy(0);
        }
        else if (page == PAGE_JOY1) {
            update_status();
            update_joy(1);
        }
        else if (page == PAGE_SERIAL) {
            absolute_time_t tm = get_absolute_time();
            if (absolute_time_diff_us(serial_tm, tm) >= (500 * 1000)) {
                serial_tm = tm;
                update_serial();
            }
            else {
                // Not time yet
                dirty = true;
            }
        }
        if (!dirty) {
            ssd1306_show(&disp);
        }
    }
}

void UserInterface::serial(bool send, uint8_t data) {
    char buf[32];
    sprintf(buf, "%s%02X", send ? "              " : "", data);

    serial_lines.push_back(std::string(buf));
    while (serial_lines.size() > 7) {
        serial_lines.pop_front();
    }
    if (page == PAGE_SERIAL) {
        dirty = true;
    }
}
