
add_library(ads1015 "")

target_sources(ads1015

   PRIVATE
   ${CMAKE_CURRENT_LIST_DIR}/ads1015.cpp

   PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/ads1015.h

)

target_include_directories(ads1015
   PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}
)
