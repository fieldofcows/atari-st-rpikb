cmake_minimum_required(VERSION 3.13)

project(rpi6301)

add_executable(rpi6301
    THD6301.cpp
    rpiikbd.cpp
    st_key_lookup_gb.cpp
    6301/6301.c
)
target_include_directories(
    rpi6301 PRIVATE 
    6301
    headers
)
add_definitions(-DUNIX -DRPI -DTRACE_6301)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-int")
target_link_libraries(rpi6301 rt pthread)