add_library(scheduler
    scheduler.cpp
)

target_link_libraries(scheduler
    pico_stdlib
    hardware_irq
)

target_include_directories(scheduler PUBLIC
    ${CMAKE_CURRENT_LIST_DIR})