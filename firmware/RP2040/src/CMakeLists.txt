include(version.cmake)
# Add a custom command that produces version.h, plus
# a dummy output that's not actually produced, in order
# to force version.cmake to always be re-run before the build
ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/version.h
               ${CMAKE_CURRENT_BINARY_DIR}/_version.h
        COMMAND ${CMAKE_COMMAND} -P
                ${CMAKE_CURRENT_SOURCE_DIR}/version.cmake)

include(flags.cmake)
# Add a custom command that produces version.h, plus
# a dummy output that's not actually produced, in order
# to force version.cmake to always be re-run before the build
ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/flags.h
               ${CMAKE_CURRENT_BINARY_DIR}/_flags.h
        COMMAND ${CMAKE_COMMAND} -P
                ${CMAKE_CURRENT_SOURCE_DIR}/flags.cmake)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    main.cpp
)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_pwm
    hardware_irq
    hardware_gpio
    hardware_adc
    lm5066
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Enable usb, disable uart outputs
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)