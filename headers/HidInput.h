#pragma once

#ifdef __cplusplus
#include <stdexcept>
#include <vector>

class HidInputException: public std::runtime_error {
public:
    HidInputException(const std::string& what) : std::runtime_error(what) { }
};

class HidInput {
private:
    HidInput();

public:
    static HidInput& instance();

    /**
     * Open the serial port.
     * Throws an exception if the keyboard or mouse cannot be opened.
     */
    void open(const std::string& kbdev, const std::string& mousedev);

    void handle_keyboard();
    void handle_mouse(const int64_t cpu_cycles);
    void reset();

    unsigned char keydown(const unsigned char code) const;
    int mouse_buttons() const; 

private:
    int keyboard_handle = -1;
    int mouse_handle = -1;
    std::vector<unsigned char> key_states;
    int mouse_state = 0;
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

#ifdef __cplusplus
}
#endif
