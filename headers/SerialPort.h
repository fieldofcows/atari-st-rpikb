#include <vector>
#include <stdexcept>

#pragma once

class SerialPortException: public std::runtime_error {
public:
    SerialPortException(const std::string& what) : std::runtime_error(what) { }
};

class SerialPort {
private:
    SerialPort() = default;

public:
    static SerialPort& instance();

    /**
     * Open the serial port.
     * Throws an exception if the port cannot be opened.
     */
    void open();

    /**
     * Send data to the Atari ST over the serial port
     */
    void send(const std::vector<unsigned char>& data) const;

    /**
     * Attempt to receive data from the Atari ST over the serial port.
     * This function does not block. If no data is waiting then nothing
     * is added to the data vector
     */
    void recv(std::vector<unsigned char>& data) const;

private:
    void configure();

private:
    int     handle = -1;
};
