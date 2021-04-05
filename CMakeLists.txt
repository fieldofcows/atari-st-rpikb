cmake_minimum_required(VERSION 3.16)

project(rpi6301)

find_package(Curses REQUIRED)

add_executable(rpi6301
    ikbd.cpp
    6301/6301.c
)
target_include_directories(
    rpi6301 PRIVATE 
    6301
    headers
)
add_definitions(-DUNIX -DRPI)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-int")
target_link_libraries(rpi6301 ncurses)