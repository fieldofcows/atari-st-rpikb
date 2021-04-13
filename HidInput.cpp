#include "HidInput.h"
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include "st_key_lookup.h"
#include "AtariSTMouse.h"

HidInput::HidInput() {
    key_states.resize(128);
    std::fill(key_states.begin(), key_states.end(), 0);
}

HidInput& HidInput::instance() {
    static HidInput hid;
    return hid;
}

void HidInput::open(const std::string& kbdev, const std::string& mousedev) {
    keyboard_handle = ::open(kbdev.c_str(), O_RDONLY | O_NONBLOCK);
    mouse_handle = ::open(mousedev.c_str(), O_RDONLY | O_NONBLOCK);
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
                unsigned char st_key = st_key_lookup_gb[evts[i].code];
                if (st_key & 0x7f) {
                    key_states[st_key] = evts[i].value;
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

unsigned char st_keydown(const unsigned char code){
    return HidInput::instance().keydown(code);
}

int st_mouse_buttons() {
    return HidInput::instance().mouse_buttons();
}