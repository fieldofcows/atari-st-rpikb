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
char st_key_lookup_gb[] = {
    0,	 // (Nothing)
    1,	 // Esc
    2,	 // 1
    3,	 // 2
    4,	 // 3
    5,	 // 4
    6,	 // 5
    7,	 // 6
    8,	 // 7
    9,	 // 8
    10,	 // 9
    11,	 // 0
    12,	 // - (ß)
    13,	 // `
    14,	 // Backspace
    15,	 // Tab
    16,	 // q
    17,	 // w
    18,	 // e
    19,	 // r
    20,	 // t
    21,	 // y (z)
    22,	 // u
    23,	 // i
    24,	 // o
    25,	 // p
    26,	 // [ (ü)
    27,	 // ] (+)
    28,	 // Enter
    29,	 // Control
    30,	 // a
    31,	 // s
    32,	 // d
    33,	 // f
    34,	 // g
    35,	 // h
    36,	 // j
    37,	 // k
    38,	 // l
    39,	 // ; (ö)
    40,	 // ' (ä)
    43,	 // ~ (Mapped to ~) PC:^
    42,	 // Lshift
    41,	 // #
    44,	 // z (y)
    45,	 // x
    46,	 // c
    47,	 // v
    48,	 // b
    49,	 // n
    50,	 // m
    51,	 // ,
    52,	 // .
    53,	 // / (-)
    54,	 // Rshift
    102, // N* (PC:55)
    56,	 // Alternate (left)
    57,	 // Space
    58,	 // CapsLock
    59,	 // F1
    60,	 // F2
    61,	 // F3
    62,	 // F4
    63,	 // F5
    64,	 // F6
    65,	 // F7
    66,	 // F8
    67,	 // F9
    68,	 // F10
    0,	 // Num (PC:69)
    0,	 // Rollen (PC:70)
    103, // N7 (PC:71)
    104, // N8 (PC:72)
    105, // N9 (PC:73)
    74,	 // N-
    106, // N4 (PC:75)
    107, // N5 (PC:76)
    108, // N6 (PC:77)
    78,	 // N+
    109, // N1 (PC:79)
    110, // N2 (PC:80)
    111, // N3 (PC:81)
    112, // N0 (PC:82)
    113, // N. (PC:83)
    0,	 // (Not used)
    0,	 // (Not used)
    96,	 // < (PC:86)
    99,	 // F11 (PC:87) mapped to NUM (
    100, // F12 (PC:88) mapped to NUM )
    0,	 // (Not used)
    0,	 // (Not used)
    0,	 // (Not used)
    0,	 // (Not used)
    0,	 // (Not used)
    0,	 // (Not used)
    0,	 // (Not used)
    114, // NEnter (PC:96)
    29,	 // Control (PC:97 right CTRL)
    101, // N/ (PC:98)
    0,	 // Druck (PC:99)
    56,	 // Alt (PC:100 Alt GR right)
    0,	 // (Not used)
    71,	 // Pos1 (Clr/Home) (PC:102)
    72,	 // Arrow up (PC:103)
    98,	 // Pg up (PC:104) mapped to Help
    75,	 // Arrow left (PC:105)
    77,	 // Arrow right (PC:106)
    0,	 // Ende (PC:107)
    80,	 // Arrow down (PC:108)
    97,	 // Pg down (PC:109) mapped to Undo
    82,	 // Insert (PC:110)
    83,	 // Delete: (PC:111)
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0};
