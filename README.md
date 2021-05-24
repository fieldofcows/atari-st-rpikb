# Atari ST RP2040 IKBD Emulator

This project allows you to use a RP2040 microcontroller to emulate the HD6301 controller that is used as the intelligent keyboard controller for the Atari ST/STe/TT series of computers. This is useful if for example you have a Mega ST that is missing its keyboard. The emulator provides the ability to use a USB keyboard, mouse and joysticks with the ST.

This project has been built specifically for the Raspberry Pi Pico development board but it should be simple to modify it to use any RP2040 based
board that includes a USB host capable connector and enough I/O for the external connections.

The emulator displays a simple user interface on an OLED display. This is entirely optional and you can build a working version without it but
it is certainly useful to show successful connection of your USB devices as well as to allow the mouse speed to be tweaked and to view the data
flowing between the emulator and the Atari ST.

The emulator supports both USB and Atari ST compatible joysticks, supported a maximum of two joysticks at a time. Using the user interface
you can select whether the USB joystick or Atari joystick are assigned to Joysticks 0 and 1.

## How it works
The Atari ST keyboard contains an HD6301 microcontroller that can be programmed by the Atari TOS or by user applications to read the keyboard, mouse and joysticks. The keyboard is connected to the Atari via a serial interface. Commands can be sent from the Atari to the keyboard and the keyboard sends mouse movements, keystrokes and joystick states to the Atari.

Instead of writing code to handle the serial protocol between the Atari and the keyboard, this project provides a full emulation of the HD6301 microcontroller and the hardware connected to it. This means that it appears to the Atari as a real keyboard, and can be customised and programmed by software like a real keyboard, providing maximum compatibility.

The RP2040 USB host port is used to connect a keyboard, mouse and joysticks using a USB hub. These are translated into an emulation of the relevant device and fed into the emulated HD6301 control registers, allowing the HD6301 to determine how to communicate this with the Atari.

## Building the emulator
The emulator is configured as per the schematic below.

![Schematic](schematic.png)

All of the external components except the level shifter are optional - you do not need to include the display and buttons if you are happy to hardcode mouse acceleration and joystick assignment settings in code. Also, if you only plan to use USB joysticks then you can omit the DB-9 connectors.

The level shifter is required as the Atari uses 5V logic over the serial connection whereas the Pico uses 3.3V logic. You can possible get away with leaving UART_RX disconnected and connect UART_TX to the Atari without a level shifter but many games and applications will not work like this as they send commands to the IKBD/emulator.


## Using the emulator
If you build the emulator as per the schematic, the Pico is powered directly from the Atari 5V supply. The Pico boots immediately but USB enumeration can take a few seconds. Once this is complete, the emulator is fully operational.

The user interface has 4 pages that are rotated between by pressing the middle UI button. The first three pages all show the number of connected USB devices at the top but allow configuration of an option below. The pages in order are:

1. USB Status + Mouse speed. Left and right buttons change allow the mouse speed to be altered.
   
   ![Mouse speed](mouse.jpg)

2. USB Status + Joystick 0 assignment. Left and right buttons toggle between USB joystick and DB-9 joystick.
   
   ![Joystick 0](joy0.jpg)

3. USB Status + Joystick 1 assignment. Left and right buttons toggle between USB joystick and DB-9 joystick.
   
   ![Joystick 1](joy1.jpg)

4. Serial data Tx/Rx between emulator and Atari. Data received from the Atari is on the left, data sent to the Atari is on the right.
   
   ![Comms](comms.jpg)

The serial data page should only be used for ensuring the connection works. Displaying the page slows down the emulator and you may seem some mouse lag whilst it is active.

The real ST keyboard has a single DB-9 socket which is shared between the mouse and Joystick 0. The emulator allows you to have a mouse and joystick plugged in simultaneously but you need to select whether the mouse or joystick 0 is active. This can be toggled by pressing the Scroll Lock button on the keyboard. The current mode is shown on any of the status pages on the OLED display.
## Known limitations
The RP2040 USB host implementation seems to contain a number of bugs. This repository contains a patched branch of the TinyUSB code to workaround many of these issues, however there are still some limitations and occasional issues as summarised below:

* Occasionally on startup USB devices are not enumerated. You need to restart the emulator to try again.
* The emulator supports only a single USB hub. So, if you use a keyboard that has a hub built in then that must be connected directly to the Pico and not plugged into another hub.

## Acknowledgements
This project has been pieced together from code extracted from [Steem SSE](https://sourceforge.net/projects/steemsse/). All of the work of wiring up the keyboard functions to the HD6301 CPU is credited to Steem SSE. This project contains a stripped-down version of this interface, connecting it to the Raspberry Pi's serial port.

Steem itself uses the HD6301 emulator provided by sim68xx developed by Arne Riiber. The original website for this seems to have gone but an archive can be found [here](http://www.oocities.org/thetropics/harbor/8707/simulator/sim68xx/).

The code to handle the OLED display is Copyright (c) 2021 David Schramm and taken from https://github.com/daschr/pico-ssd1306.
