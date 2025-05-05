add_library(lm5066
    lm5066.cpp
)

target_link_libraries(lm5066
    pico_stdlib
    hardware_irq
    hardware_i2c
)

target_include_directories(lm5066 PUBLIC
    ${CMAKE_CURRENT_LIST_DIR})
