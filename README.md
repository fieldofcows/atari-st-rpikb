# Atari ST Raspberry Pi IKBD Emulator

This project allows you to use a Raspberry PI to emulate the HD6301 microcontroller that is used as the intelligent keyboard controller for the Atari ST/STe/TT series of computers. This is useful if for example you have a Mega ST that is missing its keyboard. The emulator provides the ability to use a USB keyboard, mouse and joystick with the ST.

The code has so far been successfully tested on the following Raspberry Pi devices:

* Raspberry Pi 4 4Gb
* Raspberry Pi Zero W (with Waveshare USB hub hat)

## How it works
The Atari ST keyboard uses an HD6301 microcontroller which contains a ROM implementing the [Intelligent Keyboard (ikbd) Protocol](https://github.com/ggnkua/Atari_ST_Sources/blob/master/Docs/KEYBOARD.TXT). This translates a key matrix and mouse and joystick buttons and movements into a serial data stream to the Atari ST. The serial link is bi-directional - the ikbd supports a number of commands that can be sent by the Atari ST to configure and query the state of the devices, query the real-time clock and also to transfer bytes of code/date into the HD6301 RAM which can then be executed by the controller itself.

Rather than providing an implementation of the ikbd protocol, this project builds on existing emulation code to actually emulate the HD6301 microcontroller and the hardware configuration of the Atari ST keyboard, including the mouse/joystick ports. The original Atari keyboard ROM is then loaded by the emulator, providing an accurate emulation of a real Atari keyboard.

The USB HID capabilities of the Raspberry Pi are used to allow connection of a keyboard, mouse and a USB joystick. The software translates events from these devices into simulated I/O states on the HD6301 ports. So, for example, the USB mouse movement events are translated into quadrature pulses on I/O lines at the required frequency to match the mouse movement.

Theoretically, this emulation approach allows games and demos that take advantage of the programmability of the HD6301 to work correctly but this has not been tested (yet). In particular, a number of demos that use they keyboard controller require accurate timing which may not be emulated correctly.

## Connecting the Raspberry Pi to the Atari
Need:
* RJ12 cable (6 wires)
* Bi-directional Logic Level Converter
* Raspberry Pi


## Configuring the Raspberry Pi
[TBD]

## Acknowledgements
This project has been pieced together from code extracted from [Steem SSE](https://sourceforge.net/projects/steemsse/). All of the work of wiring up the keyboard functions to the HD6301 CPU is credited to Steem SSE. This project contains a stripped-down version of this interface, connecting it to the Raspberry Pi's serial port.

Steem itself uses the HD6301 emulator provided by sim68xx developed by Arne Riiber. The original website for this seems to have gone but an archive can be found [here](http://www.oocities.org/thetropics/harbor/8707/simulator/sim68xx/).
