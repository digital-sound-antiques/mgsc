cmake_minimum_required(VERSION 3.4)

set(CMAKE_C_FLAGS "-O3 -Wall")

add_subdirectory(modules/kmz80)

include_directories(modules/kmz80)

file(GLOB SOURCES src/*.cpp)

add_executable(mgsc ${SOURCES})
target_link_libraries(mgsc kmz80)

