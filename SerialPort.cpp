#include "SerialPort.h"
#include <fcntl.h>
#include <asm-generic/ioctls.h>
#include <asm-generic/termbits.h>
#include <sys/ioctl.h>
#include <unistd.h>

// The HD6301 in the ST communicates at 7812 baud. For some reason, configuring the RPi to use
// 8000 baud results in the correct baud rate of 7812 (measured on oscilloscope). It would be
// interesting to determine why.
#define CUSTOM_BAUD 8000

SerialPort& SerialPort::instance() {
    static SerialPort serial;
    return serial;
}

void SerialPort::open() {
    if (handle == -1) {
        handle = ::open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (handle == -1) {
            throw new SerialPortException("Could not open /dev/ttyAMA0");
        }
        // Setup the port parameters
        configure();
    }
}

void SerialPort::send(const std::vector<unsigned char>& data) const {
    write(handle, data.data(), data.size());
}

void SerialPort::recv(std::vector<unsigned char>& data) const {
    unsigned char buf[1];
    int count = read(handle, buf, sizeof(buf));
    if (count != -1) {
        // Append any newly received data to the end of the current vector
        data.insert(data.end(), buf, &buf[count]);
    }
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
