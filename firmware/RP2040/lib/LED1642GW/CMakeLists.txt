cmake_minimum_required(VERSION 3.13)

# Set your project name here
project(led_driver_example C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize Pico SDK
include(pico_sdk_import.cmake)
pico_sdk_init()

# Create executable
add_executable(led_driver_example
    main.cpp
    led1642gw.cpp
)

# Include directories
target_include_directories(led_driver_example PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Link with the Pico SDK libraries
target_link_libraries(led_driver_example
    pico_stdlib
    hardware_spi
)

# Enable USB output and disable UART for stdio
pico_enable_stdio_usb(led_driver_example 1)
pico_enable_stdio_uart(led_driver_example 0)

# Create the .uf2 file
pico_add_extra_outputs(led_driver_example)
