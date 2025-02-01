cmake_minimum_required(VERSION 3.20.0)

set(BOARD rpi_pico)

find_package(Zephyr)
project(my_zephyr_app)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_sources(app PRIVATE src/main.c)
