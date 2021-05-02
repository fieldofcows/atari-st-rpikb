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
char st_key_lookup_hid_gb[] = {
    0, // 0x00  No key pressed
    0, // 0x01  Keyboard Error Roll Over - used for all slots if too many keys are pressed ("Phantom key")
    0, // 0x02  Not used
    0, // 0x03  Not used
    30, // 0x04  Keyboard a and A
    48, // 0x05  Keyboard b and B
    46, // 0x06  Keyboard c and C
    32, // 0x07  Keyboard d and D
    18, // 0x08  Keyboard e and E
    33, // 0x09  Keyboard f and F
    34, // 0x0a  Keyboard g and G
    35, // 0x0b  Keyboard h and H
    23, // 0x0c  Keyboard i and I
    36, // 0x0d  Keyboard j and J
    37, // 0x0e  Keyboard k and K
    38, // 0x0f  Keyboard l and L
    50, // 0x10  Keyboard m and M
    49, // 0x11  Keyboard n and N
    24, // 0x12  Keyboard o and O
    25, // 0x13  Keyboard p and P
    16, // 0x14  Keyboard q and Q
    19, // 0x15  Keyboard r and R
    31, // 0x16  Keyboard s and S
    20, // 0x17  Keyboard t and T
    22, // 0x18  Keyboard u and U
    47, // 0x19  Keyboard v and V
    17, // 0x1a  Keyboard w and W
    45, // 0x1b  Keyboard x and X
    21, // 0x1c  Keyboard y and Y
    44, // 0x1d  Keyboard z and Z
    2, // 0x1e  Keyboard 1 and !
    3, // 0x1f  Keyboard 2 and @
    4, // 0x20  Keyboard 3 and #
    5, // 0x21  Keyboard 4 and $
    6, // 0x22  Keyboard 5 and %
    7, // 0x23  Keyboard 6 and ^
    8, // 0x24  Keyboard 7 and &
    9, // 0x25  Keyboard 8 and *
    10, // 0x26  Keyboard 9 and (
    11, // 0x27  Keyboard 0 and )
    28, // 0x28  Keyboard Return (ENTER)
    1, // 0x29  Keyboard ESCAPE
    14, // 0x2a  Keyboard DELETE (Backspace)
    15, // 0x2b  Keyboard Tab
    57, // 0x2c  Keyboard Spacebar
    12, // 0x2d  Keyboard - and _
    13, // 0x2e  Keyboard = and +
    26, // 0x2f  Keyboard [ and {
    27, // 0x30  Keyboard ] and }
    0, // 0x31  Keyboard \ and |
    41, // 0x32  Keyboard Non-US # and ~
    39, // 0x33  Keyboard ; and :
    40, // 0x34  Keyboard ' and "
    43, // 0x35  Keyboard ` and ~
    51, // 0x36  Keyboard , and <
    52, // 0x37  Keyboard . and >
    53, // 0x38  Keyboard / and ?
    58, // 0x39  Keyboard Caps Lock
    59,// 0x3a  Keyboard F1
    60, // 0x3b  Keyboard F2
    61, // 0x3c  Keyboard F3
    62, // 0x3d  Keyboard F4
    63, // 0x3e  Keyboard F5
    64, // 0x3f  Keyboard F6
    65, // 0x40  Keyboard F7
    66, // 0x41  Keyboard F8
    67, // 0x42  Keyboard F9
    68, // 0x43  Keyboard F10
    99, // 0x44  Keyboard F11
    100, // 0x45  Keyboard F12
    0, // 0x46  Keyboard Print Screen
    0, // 0x47  Keyboard Scroll Lock
    0, // 0x48  Keyboard Pause
    82, // 0x49  Keyboard Insert
    71, // 0x4a  Keyboard Home
    98, // 0x4b  Keyboard Page Up
    83, // 0x4c  Keyboard Delete Forward
    0, // 0x4d  Keyboard End
    97, // 0x4e  Keyboard Page Down
    77, // 0x4f  Keyboard Right Arrow
    75, // 0x50  Keyboard Left Arrow
    80, // 0x51  Keyboard Down Arrow
    72, // 0x52  Keyboard Up Arrow
    0, // 0x53  Keyboard Num Lock and Clear
    101, // 0x54  Keypad /
    102, // 0x55  Keypad *
    74, // 0x56  Keypad -
    78, // 0x57  Keypad +
    114, // 0x58  Keypad ENTER
    109, // 0x59  Keypad 1 and End
    110, // 0x5a  Keypad 2 and Down Arrow
    111, // 0x5b  Keypad 3 and PageDn
    106, // 0x5c  Keypad 4 and Left Arrow
    107, // 0x5d  Keypad 5
    108, // 0x5e  Keypad 6 and Right Arrow
    103, // 0x5f  Keypad 7 and Home
    104, // 0x60  Keypad 8 and Up Arrow
    105, // 0x61  Keypad 9 and Page Up
    112, // 0x62  Keypad 0 and Insert
    113, // 0x63  Keypad . and Delete
    96, // 0x64  Keyboard Non-US \ and |
    0, // 0x65  Keyboard Application
    0, // 0x66  Keyboard Power
    0, // 0x67  Keypad =
    0, // 0x68  Keyboard F13
    0, // 0x69  Keyboard F14
    0, // 0x6a  Keyboard F15
    0, // 0x6b  Keyboard F16
    0, // 0x6c  Keyboard F17
    0, // 0x6d  Keyboard F18
    0, // 0x6e  Keyboard F19
    0, // 0x6f  Keyboard F20
    0, // 0x70  Keyboard F21
    0, // 0x71  Keyboard F22
    0, // 0x72  Keyboard F23
    0, // 0x73  Keyboard F24
    0, // 0x74
    0, // 0x75
    0, // 0x76
    0, // 0x77
    0, // 0x78
    0, // 0x79
    0, // 0x7a
    0, // 0x7b
    0, // 0x7c
    0, // 0x7d
    0, // 0x7e
    0, // 0x7f
};