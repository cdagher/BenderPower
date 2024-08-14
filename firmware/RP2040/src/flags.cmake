if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(FLAGS
    "#define USB_DEBUG")
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/flags.h)
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/flags.h FLAGS_)
else()
    set(FLAGS_ "")
endif()

if (NOT "${FLAGS}" STREQUAL "${FLAGS_}")
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/flags.h "${FLAGS}")
endif()