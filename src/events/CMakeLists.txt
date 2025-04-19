cmake_minimum_required(VERSION 3.16)

add_library(events INTERFACE)

target_include_directories(events INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
