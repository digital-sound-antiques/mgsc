cmake_minimum_required(VERSION 3.4)

set(CMAKE_C_FLAGS "-O3 -Wall")

if(NOT TARGET kmz80)
	add_subdirectory(modules/kmz80)
endif()

include_directories(modules/kmz80)

add_library(mgsc STATIC src/mgsc.cpp src/bdos.cpp src/z80.cpp src/ram64k.cpp)
target_link_libraries(mgsc kmz80)

add_executable(mgsc-exe src/main.cpp)
target_link_libraries(mgsc-exe mgsc)

set_target_properties(mgsc-exe PROPERTIES OUTPUT_NAME mgsc)

