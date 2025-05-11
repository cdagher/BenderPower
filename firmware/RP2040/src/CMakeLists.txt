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

add_library(board INTERFACE)

target_sources(board
   INTERFACE
   ${CMAKE_CURRENT_LIST_DIR}/board.h
   ${CMAKE_CURRENT_LIST_DIR}/flags.h
   ${CMAKE_CURRENT_LIST_DIR}/version.h
)


target_include_directories(board
   INTERFACE
   ${CMAKE_CURRENT_LIST_DIR}
)



add_library(shell "")

target_sources(shell
   PRIVATE
   ${CMAKE_CURRENT_LIST_DIR}/shell.cpp
   PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/shell.h
)

target_include_directories(shell
   PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(shell
   pico_stdlib
   hardware_pio
   ads1015
   led1642gw
   lm5066
)


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
    hardware_i2c
    hardware_spi
    lm5066
    ads1015
    led1642gw
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Copy ELF, HEX, and UF2 files to the top-level build directory after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:${PROJECT_NAME}>
        ${CMAKE_BINARY_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.uf2
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.uf2
   COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf
   COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
   COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.dis
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dis
)

# Enable usb and uart outputs
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
