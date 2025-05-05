cmake_minimum_required(VERSION 3.13)

# Set your project name here
project(led_driver_example C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Create executable
add_library(led1642gw
    led1642gw.cpp
)

# Include directories
target_include_directories(led1642gw PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Link with the Pico SDK libraries
target_link_libraries(led1642gw
    pico_stdlib
    hardware_spi
)

