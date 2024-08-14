add_library(I2C
    I2C_Device.cpp
)

target_link_libraries(I2C
    pico_stdlib
    pico_stdio
    scheduler
)

target_include_directories(I2C PUBLIC
    ${CMAKE_CURRENT_LIST_DIR})