cmake_minimum_required(VERSION 3.10)

project(circuit-simulator)

set(DEVICE_BASE ${CMAKE_CURRENT_SOURCE_DIR}/devices)
set(CORE_BASE ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(INTERFACE_BASE ${CMAKE_CURRENT_SOURCE_DIR}/interfaces)

include_directories(${DEVICE_BASE})
include_directories(${CORE_BASE})
include_directories(${INTERFACE_BASE})

add_compile_options(-fpermissive)
add_subdirectory(src)
add_subdirectory(devices)
