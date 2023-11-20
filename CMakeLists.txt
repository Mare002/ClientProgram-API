cmake_minimum_required(VERSION 3.17)
project(uloha3 C)

set(CMAKE_C_STANDARD 99)

add_executable(uloha3 uloha3.c)

link_libraries(ws2_32 wsock32)
target_link_libraries(uloha3 wsock32 ws2_32)
