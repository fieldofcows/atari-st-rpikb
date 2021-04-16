# Atari ST Raspberry Pi IKDB Emulator
# Copyright (C) 2021 Roy Hopkins
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

cmake_minimum_required(VERSION 3.13)

project(rpi6301)

add_executable(rpi6301
    src/rpiikbd.cpp
    src/st_key_lookup_gb.cpp
    src/AtariSTMouse.cpp
    src/SerialPort.cpp
    src/HidInput.cpp
    src/util.cpp
    6301/6301.c
)
target_include_directories(
    rpi6301 PRIVATE 
    6301
    include
)
#add_definitions(-DUNIX -DRPI -DTRACE_6301)
add_definitions(-DUNIX -DRPI)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-int")
target_link_libraries(rpi6301 rt pthread)