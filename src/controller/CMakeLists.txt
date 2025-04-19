cmake_minimum_required(VERSION 3.16)

add_library(controller INTERFACE)

target_include_directories(controller INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)